#include "I2CSensor.h"

I2CSensor::I2CSensor() {
	address = I2C_ADDRESS_DEFAULT;
}

I2CSensor::I2CSensor(uint8_t address_in) {
	address = address_in;
}

bool I2CSensor::isAlive() {
	i2cBus.beginTransmission(address);
	if(i2cBus.endTransmission() == 0) {
		return true;   
	}
	return false;
}

bool I2CSensor::writeRegister(uint8_t reg, uint8_t val) {
	return writeRegisterBase(reg, &val, 1);
}

bool I2CSensor::writeRegisterPageInt32(uint8_t reg, uint32_t val) {
	// sensor needs to support page mode (automatic address increment)
	uint8_t const LEN = 4;
	uint8_t val_array[LEN];
	
	for(uint8_t i = 0; i < LEN; i++) {
		val_array[i] = (val >> (i*8)) & 0xff;
	}
	
	return writeRegisterBase(reg, val_array, LEN);
}

bool I2CSensor::writeRegisterBase(uint8_t reg, uint8_t vals[], uint16_t len) {
	// sensor needs to support page mode (automatic address increment)
	i2cBus.beginTransmission(address);
	i2cBus.write(reg);
	
	for(uint16_t i = 0; i < len; i++) {
		i2cBus.write(vals[i]);
	}
	 
	if(i2cBus.endTransmission() != 0) {
		return false;
	}
	return true;
}

uint8_t I2CSensor::readRegister(uint8_t reg, bool &error) {
	uint8_t const LEN = 1;
	uint8_t val_array[LEN];
	
	// important: do not reset error here, e.g. IMU_BMX160 works under this assumption
	if(!readRegisterBase(&reg, 1, val_array, LEN)) {
		error = true;
		return 0x0;
	}
	return val_array[0];
}

uint32_t I2CSensor::readRegisterPageInt32(uint8_t reg, bool &error) {
	// sensor needs to support page mode (automatic address increment)
	uint8_t const LEN = 4;
	uint8_t vals[LEN];
	uint32_t result = 0;
	uint32_t temp = 0;
	error = false;
	
	if(!readRegisterBase(&reg, 1, vals, LEN)) {
		error = true;
		return 0x0;
	}
	for(uint8_t i = 0; i < LEN; i++) {
		temp = (uint32_t) vals[i];
		result |= temp << (i*8);
	}
	return result;
}

bool I2CSensor::readRegisterPageArray(uint8_t reg, uint8_t vals[], uint16_t len) {
	// sensor needs to support page mode (automatic address increment)
	return readRegisterBase(&reg, 1, vals, len);
}

bool I2CSensor::readRegisterBase(uint8_t regs[], uint8_t reg_len, uint8_t vals[], uint16_t len) {
	i2cBus.beginTransmission(address);
	for(uint8_t i = 0; i < reg_len; i++) {
		i2cBus.write(regs[i]);
	}
	if(i2cBus.endTransmission() != 0) {
		return false;
	}
	i2cBus.requestFrom(address, len);
	for(uint16_t i = 0; i < len; i++) {
		vals[i] = i2cBus.read();
	}
	// NOT NEEDED! after removal not tested with ATTINY and stuff
	/*if(i2c.endTransmission() != 0) {
		return false;
	}*/
	return true;
}

bool I2CSensor::readRegisterBit(uint8_t reg, uint8_t bit, bool &error) {
	uint8_t reg_result = readRegister(reg, error);
	return HelperFunctions::getBit(reg_result, bit);	
}
