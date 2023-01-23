#include "EEPROM_CAT.h"

EEPROM_CAT::EEPROM_CAT() {
	address = EEPROM_CAT24M01_ADDRESS; // default: use first page, dynamically switch if higher address area is selected
}

void EEPROM_CAT::swapAddress(uint32_t adr) {
	if(adr < EEPROM_CAT24M01_MEM_SIZE / 2) { // 2^16, use first page
		address = EEPROM_CAT24M01_ADDRESS;
	}
	else { // use second page (different I2C address)
		address = EEPROM_CAT24M01_ADDRESS_2;
	}	
}

bool EEPROM_CAT::readMemory(uint32_t adr, uint8_t result_arr[], uint32_t len) {
	// read wraps around automatically
	const uint8_t REG_LEN = 2;
	uint8_t regs[REG_LEN];
	if(adr + len > EEPROM_CAT24M01_MEM_SIZE) {
		return false;
	}
	swapAddress(adr);
	
	regs[0] = (adr >> 8) & 0xff;
	regs[1] = (adr >> 0) & 0xff;

	return readRegisterBase(regs, REG_LEN, result_arr, len);
}

bool EEPROM_CAT::writeMemory(uint32_t adr, uint8_t value_arr[], uint32_t len) {
	return writeMemoryInternal(adr, value_arr, len, false);
}

bool EEPROM_CAT::clearMemory(uint32_t from, uint32_t len, uint8_t val) {
	return writeMemoryInternal(from, &val, len, true);
}

bool EEPROM_CAT::busyWriting() {
	uint8_t regs[2] = { 0x0, 0x0 };
	uint8_t val = 0x0;
	
	// using last used address (EEPROM_CAT24M01_ADDRESS or EEPROM_CAT24M01_ADDRESS_2), hence not changing
	return (!readRegisterBase(regs, 2, &val, 1)); // getting data @address 0, no ACK, then still writing
}

bool EEPROM_CAT::writeMemoryInternal(uint32_t adr, uint8_t value_arr[], uint32_t leftForWriting, uint8_t writeSingleValue) {
	uint16_t protectionCount = 6000UL;
	uint16_t pageStillFits;
	uint8_t oneWriteCycleLen;
	bool finished = false;
	uint32_t arrayIndex = 0;
	uint8_t writeBuffer[EEPROM_CAT24M01_MAX_WRITE_LENGTH+1];
	uint8_t reg;
	
	if((adr + leftForWriting > EEPROM_CAT24M01_MEM_SIZE) || (leftForWriting == 0)) {
		return false;
	}
	
	while(!finished) {
		// calculate how much to write
		pageStillFits = (uint16_t) (EEPROM_CAT24M01_PAGE_SIZE - (adr % EEPROM_CAT24M01_PAGE_SIZE)); // warning: page size maximum 2^16 = 65536 
		if(pageStillFits > EEPROM_CAT24M01_MAX_WRITE_LENGTH) { // page still fits more than 30 Bytes
			if(leftForWriting <= EEPROM_CAT24M01_MAX_WRITE_LENGTH) { // all remaining data can go into the next 30 Bytes
				oneWriteCycleLen = leftForWriting;
				finished = true;
			}
			else { // write 30 Bytes, but next iteration necessary
				oneWriteCycleLen = EEPROM_CAT24M01_MAX_WRITE_LENGTH;
			}
		}
		else { // page fits less than 30 Bytes, cannot write full length
			if(leftForWriting <= pageStillFits) { // all remaining data can go into less than 30 Bytes
				oneWriteCycleLen = leftForWriting;
				finished = true;
			}
			else { // write until page full, but next iteration necessary
				oneWriteCycleLen = pageStillFits; // warning, assigning 16bit integer to 8bit, should be fine becauce pageStillFits <= 30
			}
		}
		
		// write
		swapAddress(adr);
		reg = (adr >> 8) & 0xff; // first part of address to write
		writeBuffer[0] = (adr >> 0) & 0xff; // second part of address = first byte to send
		for(uint8_t i = 0; i < oneWriteCycleLen; i++) { // append data
			if(writeSingleValue) {
				writeBuffer[i+1] = value_arr[0]; // for clearing memory with one value
			}
			else {
				writeBuffer[i+1] = value_arr[arrayIndex+i];
			}
		}
		if(!writeRegisterBase(reg, writeBuffer, oneWriteCycleLen+1)) {
			return false;
		}
	
		while(busyWriting()) { ; } // wait until write is done
		
		// decrement for next cycle
		leftForWriting -= oneWriteCycleLen;
		adr += oneWriteCycleLen;
		arrayIndex += oneWriteCycleLen;
		protectionCount--;
		if(protectionCount == 0) { // timeout
			return false;
		}
	}
	return true;
}
