#ifndef I2CSensor_h
#define I2CSensor_h

#include <stdint.h>
#include "HelperFunctions.h"
#include "ArduinoWrapper.h" // needed for i2c object and I2C class definition

#define I2C_ADDRESS_DEFAULT			0x01
#define I2C_ARDUINO_BUFFER_LIMIT	32

class I2CSensor {
	public:
		I2CSensor();
		I2CSensor(uint8_t address);
		bool isAlive();
		
		bool writeRegister(uint8_t reg, uint8_t val); // uses writeRegisterBase
		bool writeRegisterPageInt32(uint8_t reg, uint32_t val); // uses writeRegisterBase
		bool writeRegisterBase(uint8_t reg, uint8_t vals[], uint16_t len);
		
		uint8_t readRegister(uint8_t reg, bool &error); // uses readRegisterBase
		uint32_t readRegisterPageInt32(uint8_t reg, bool &error); // uses readRegisterBase
		bool readRegisterPageArray(uint8_t reg, uint8_t vals[], uint16_t len); // uses readRegisterBase
		bool readRegisterBase(uint8_t regs[], uint8_t reg_len, uint8_t vals[], uint16_t len);
		
		bool readRegisterBit(uint8_t reg, uint8_t bit, bool &error);
	protected:
		uint8_t address;
};

#endif
