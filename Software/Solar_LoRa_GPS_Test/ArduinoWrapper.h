#ifndef ArduinoWrapper_h
#define ArduinoWrapper_h

#include <stdint.h>
#include "Arduino.h"
#include "Wire.h"
#include <LoRaWAN.h>

#define I2C_FREQ_HZ					400000

class ArduinoWrapper {
	public:
		static void delayWrapper(uint16_t d);
		static uint64_t millisWrapper();
};

class I2CBus {
	public:
		void begin();
		void setClock(uint32_t clock);
		void beginTransmission(uint8_t address);
		void write(uint8_t data);
		void write(const uint8_t *data, uint8_t size);
		uint8_t read();
		uint8_t endTransmission();
		uint8_t endTransmission(bool in);
		bool available();
		void requestFrom(uint8_t address, uint8_t quantity);	
};

extern I2CBus i2cBus;

#endif
