#ifndef HelperFunctions_h
#define HelperFunctions_h

#include <stdint.h>
#include <stdio.h>

class HelperFunctions {
	public:
    static uint16_t arrayToUint16(uint8_t *buffer);
    static uint32_t arrayToUint32(uint8_t *buffer);
		static uint8_t intToBCD(uint8_t num);
		static uint8_t BCDToInt(uint8_t num);
		static uint8_t setBit(uint8_t reg, uint8_t pos, bool val);
		static bool getBit(uint8_t reg, uint8_t pos);
		static uint32_t to32KHz(uint32_t ms);
		static void addInt16AsHexToCharArray(char *array, uint16_t startPosition, uint16_t value); // char array needs to be at least 4 bytes long (+ terminating zero)
		static void addInt32AsHexToCharArray(char *array, uint16_t startPosition, uint32_t value); // char array needs to be at least 8 bytes long (+ terminating zero)
};

#endif
