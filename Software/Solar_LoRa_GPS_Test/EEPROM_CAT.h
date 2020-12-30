#ifndef EEPROM_CAT_h
#define EEPROM_CAT_h

#include "I2CSensor.h"
#include <stdint.h>
	
#define EEPROM_CAT24M01_ADDRESS					    0x50
#define EEPROM_CAT24M01_ADDRESS_2				    0x51 		      // second page
#define EEPROM_CAT24M01_MEM_SIZE				    131072 		    // 17 bit
#define EEPROM_CAT24M01_PAGE_SIZE				    256			      // important for write-wrapping
#define EEPROM_CAT24M01_MEM_VAL_UNWRITTEN		0xFF
#define EEPROM_CAT24M01_MAX_WRITE_LENGTH		30			      // CAT24M01 supports 256, Wire.h Arduino lib limits buffer to 32 bytes!

class EEPROM_CAT : public I2CSensor {
	public:
		EEPROM_CAT();
		bool readMemory(uint32_t adr, uint8_t result_arr[], uint32_t len);
		bool writeMemory(uint32_t adr, uint8_t value_arr[], uint32_t len); // uses writeMemoryInternal
		bool clearMemory(uint32_t from, uint32_t len, uint8_t val); // full erase takes 60 seconds, uses writeMemoryInternal
	private:
		bool busyWriting();
		void swapAddress(uint32_t adr);
		bool writeMemoryInternal(uint32_t adr, uint8_t value_arr[], uint32_t leftForWriting, uint8_t writeSingleValue);
};

#endif
