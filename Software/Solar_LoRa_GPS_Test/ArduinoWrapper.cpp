#include "ArduinoWrapper.h"

void ArduinoWrapper::delayWrapper(uint16_t d) {
	delay(d);
}

uint64_t ArduinoWrapper::millisWrapper() {
    return millis();
}

I2CBus i2cBus = I2CBus();

/** I2C Wrapper */
void I2CBus::begin() {
	Wire.begin();
}

void I2CBus::setClock(uint32_t clock) {
	Wire.setClock(clock);
}

void I2CBus::beginTransmission(uint8_t address) {
	Wire.beginTransmission(address);
}

void I2CBus::write(uint8_t data) {
	Wire.write(data);
}

void I2CBus::write(const uint8_t *data, uint8_t size) {
	Wire.write(data, size);
}

uint8_t I2CBus::read() {
	return Wire.read();
}

uint8_t I2CBus::endTransmission() {
	return Wire.endTransmission();
}

uint8_t I2CBus::endTransmission(bool in) {
	return Wire.endTransmission(in);
}

void I2CBus::requestFrom(uint8_t address, uint8_t quantity) {
	Wire.requestFrom(address, quantity);
}

bool I2CBus::available() {
	return Wire.available();
}
