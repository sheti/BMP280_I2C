#include <Wire.h>
#include <BMP280_I2C.h>

BMP280_I2C::BMP280_I2C() { };

bool BMP280_I2C::init() {
	if (read8(BMP280_REGISTER_CHIPID) != 0x58)
		return false;

	readCalibrationData();
	write8(BMP280_REGISTER_CONTROL, 0x3F);
	return true;
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC. 
// t_fine carries fine temperature as global value
long BMP280_I2C::getTemperature() {
	long var1, var2;

	long adc_T = (long)read24(BMP280_REGISTER_TEMPDATA);
	adc_T >>= 4;

	var1  = ((((adc_T>>3) - ((long)dig_T1 <<1))) * ((long)dig_T2)) >> 11;

	var2  = (((((adc_T>>4) - ((long)dig_T1)) * ((adc_T>>4) - ((long)dig_T1))) >> 12) * ((long)dig_T3)) >> 14;

	t_fine = var1 + var2;
	temperature = (t_fine * 5 + 128) >> 8;
	return temperature;
}
// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
unsigned long BMP280_I2C::getPressure() {
	int64_t var1, var2, p;

	// Must be done first to get the t_fine variable set up
	getTemperature();

	long adc_P = read24(BMP280_REGISTER_PRESSUREDATA);
	adc_P >>= 4;

	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
	var2 = var2 + (((int64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p<<31) - var2)*3125) / var1;
	var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
	return (unsigned long)p;
}

void BMP280_I2C::readCalibrationData() {
	dig_T1 = (word)read16LE(0x88);
	dig_T2 = (short)read16LE(0x8A);
	dig_T3 = (short)read16LE(0x8C);
	dig_P1 = (word)read16LE(0x8E);
	dig_P2 = (short)read16LE(0x90);
	dig_P3 = (short)read16LE(0x92);
	dig_P4 = (short)read16LE(0x94);
	dig_P5 = (short)read16LE(0x96);
	dig_P6 = (short)read16LE(0x98);
	dig_P7 = (short)read16LE(0x9A);
	dig_P8 = (short)read16LE(0x9C);
	dig_P9 = (short)read16LE(0x9E);
}

void BMP280_I2C::write8(byte _addr, byte _val) {
	Wire.beginTransmission(BMP280_I2C_ADDR);   // start transmission to device 
	Wire.write(_addr); // send register address
	Wire.write(_val); // send value to write  
	Wire.endTransmission(); // end transmission
}

byte BMP280_I2C::read8(byte _addr) {
	byte *_buff = read(_addr, 1);
	byte _result = _buff[0];
	free(_buff);
	return _result;
}

word BMP280_I2C::read16LE(byte _addr) {
	word temp = read16(_addr);
	return (temp >> 8) | (temp << 8);
}

word BMP280_I2C::read16(byte _addr) {
	byte *_buff = read(_addr, 2);
	word _result = ((word)_buff[0] << 8) | (word)_buff[1];
	free(_buff);
	return _result;
}

unsigned long BMP280_I2C::read24(byte _addr) {
	byte *_buff = read(_addr, 3);
	unsigned long _result = ((unsigned long)_buff[0] << 16) | ((unsigned long)_buff[1] << 8) | (unsigned long)_buff[2];
	free(_buff);
	return _result;
}

unsigned long BMP280_I2C::read32(byte _addr) {
	byte *_buff = read(_addr, 4);
	unsigned long _result = ((unsigned long)_buff[0] << 24) | ((unsigned long)_buff[1] << 16) | ((unsigned long)_buff[2] << 80) | (unsigned long)_buff[3];
	free(_buff);
	return _result;
}

byte *BMP280_I2C::read(byte _addr, byte n) {
	byte *_buff = (byte *)malloc(sizeof(byte) * n);
	Wire.beginTransmission(BMP280_I2C_ADDR); // start transmission to device 
	Wire.write(_addr); // sends register address to read from
	Wire.endTransmission(); // end transmission
	  
	Wire.beginTransmission(BMP280_I2C_ADDR); // start transmission to device 
	Wire.requestFrom(BMP280_I2C_ADDR, n);// send data n-bytes read
	byte i = 0;
	while (Wire.available()) {
		_buff[i] = Wire.read(); // receive DATA
		i += 1;
	}
	Wire.endTransmission(); // end transmission
	return _buff;
}
