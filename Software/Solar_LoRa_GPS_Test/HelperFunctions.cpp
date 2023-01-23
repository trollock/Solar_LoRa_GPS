#include "HelperFunctions.h"

uint16_t HelperFunctions::arrayToUint16(uint8_t *buffer) {
  uint16_t result;
  result = (((uint16_t) buffer[0]) << 8) | (((uint16_t) buffer[1]));
  return result;
}

uint32_t HelperFunctions::arrayToUint32(uint8_t *buffer) {
  uint32_t result;
  result = (((uint32_t) buffer[0]) << 24) | (((uint32_t) buffer[1]) << 16) | (((uint32_t) buffer[2]) << 8) | (((uint32_t) buffer[3]));
  return result;
}

uint8_t HelperFunctions::intToBCD(uint8_t num) {
	uint8_t lw, up;
	lw = num % 10;
	up = num / 10;
	return lw | (up << 4);
}

uint8_t HelperFunctions::BCDToInt(uint8_t bcd) {
	uint8_t lw, up;
	lw = bcd & 0x0f;
	up = bcd >> 4;
	return (up * 10) + lw;
}

uint8_t HelperFunctions::setBit(uint8_t reg, uint8_t pos, bool val) {
	if(val) {
		return reg |= 1UL << pos;
	}
	return reg &= ~(1UL << pos);
}

bool HelperFunctions::getBit(uint8_t reg, uint8_t pos) {
	return (reg >> pos) & 1UL;
}

uint32_t HelperFunctions::to32KHz(uint32_t ms) {
	return ms / 31UL;
}

void HelperFunctions::addInt16AsHexToCharArray(char *array, uint16_t startPosition, uint16_t value) {
    const char hexchars[17] = "0123456789ABCDEF";
    /*if(strlen(array) < startPosition + 4) {
        return;
    }*/
    array[startPosition+0] = hexchars[(value >> 12) & 0xF];
    array[startPosition+1] = hexchars[(value >> 8) & 0xF];
    array[startPosition+2] = hexchars[(value >> 4) & 0xF];
    array[startPosition+3] = hexchars[(value >> 0) & 0xF];
}

void HelperFunctions::addInt32AsHexToCharArray(char *array, uint16_t startPosition, uint32_t value) {
    const char hexchars[17] = "0123456789ABCDEF";
    /*if(strlen(array) < startPosition + 4) {
        return;
    }*/
	array[startPosition+0] = hexchars[(value >> 28) & 0xF];
    array[startPosition+1] = hexchars[(value >> 24) & 0xF];
    array[startPosition+2] = hexchars[(value >> 20) & 0xF];
    array[startPosition+3] = hexchars[(value >> 16) & 0xF];
	
    array[startPosition+4] = hexchars[(value >> 12) & 0xF];
    array[startPosition+5] = hexchars[(value >> 8) & 0xF];
    array[startPosition+6] = hexchars[(value >> 4) & 0xF];
    array[startPosition+7] = hexchars[(value >> 0) & 0xF];
}
