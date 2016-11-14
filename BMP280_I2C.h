#ifndef BMP280_I2C_h
#define BMP280_I2C_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define BMP280_I2C_ADDR 0x77 //0x77 default I2C address
#define BMP280_REGISTER_CHIPID 0xD0
#define BMP280_REGISTER_CONTROL 0xF4
#define BMP280_REGISTER_TEMPDATA 0xFA
#define BMP280_REGISTER_PRESSUREDATA 0xF7

class BMP280_I2C {
	public:  
		BMP280_I2C();
		bool init();
		unsigned long getPressure();
		long getTemperature();
		long temperature;
	private:
		word dig_T1;
		short dig_T2;
		short dig_T3;
		word dig_P1;
		short dig_P2;
		short dig_P3;
		short dig_P4;
		short dig_P5;
		short dig_P6;
		short dig_P7;
		short dig_P8;
		short dig_P9;
		long t_fine;
		void readCalibrationData();
		void write8(byte _addr, byte _val);
		byte read8(byte _addr);
		word read16(byte _addr);
		word read16LE(byte _addr);
		unsigned long read24(byte _addr);
		unsigned long read32(byte _addr);
		byte *read(byte _addr, byte n);
};
#endif
