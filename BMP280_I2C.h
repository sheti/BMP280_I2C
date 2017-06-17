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
		uint32_t getPressure();
		int32_t getTemperature();
		int32_t temperature;
	private:
		uint16_t dig_T1;
		int16_t dig_T2;
		int16_t dig_T3;
		uint16_t dig_P1;
		int16_t dig_P2;
		int16_t dig_P3;
		int16_t dig_P4;
		int16_t dig_P5;
		int16_t dig_P6;
		int16_t dig_P7;
		int16_t dig_P8;
		int16_t dig_P9;
		int32_t t_fine;
		void readCalibrationData();
		void write8(byte _addr, byte _val);
		byte read8(byte _addr);
		uint16_t read16(byte _addr);
		uint16_t read16LE(byte _addr);
		uint32_t read24(byte _addr);
		uint32_t read32(byte _addr);
		byte *read(byte _addr, byte n);
};
#endif
