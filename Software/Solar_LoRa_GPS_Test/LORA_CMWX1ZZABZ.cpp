#include "LORA_CMWX1ZZABZ.h"

LORA_CMWX1ZZABZ::LORA_CMWX1ZZABZ() {
	
}


void LORA_CMWX1ZZABZ::initAndJoin() {
	// choose OTAA!!
	// examples: https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0/tree/master/libraries/LoRaWAN/examples
	
	/*
	// EXAMPLE SKETCH GRASSHOPPER
	LoRaWAN.begin(US915);
    LoRaWAN.setADR(false);
    LoRaWAN.setDataRate(1);
    LoRaWAN.setTxPower(10);
    LoRaWAN.setSubBand(2); // for TTN, U915/US915 networks have 64+8 channels. Typical gateways support only 8 (9) channels. Hence it's a good idea to pick the proper channel subset via select via LoRaWAN.setSubBand()
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
	*/
	
	//LoRaWAN.setDutyCycle(false); // TEST: EU868/IN865 have duty cycle restrictions. For debugging it makes sense to disable those via setDutyCycle(false);
	
	LoRaWAN.getDevEui(devEui, 18);
	
  LoRaWAN.begin(EU868);
	
	// settings
	LoRaWAN.setAntennaGain(2.0);	
	LoRaWAN.addChannel(1, 868300000, 0, 6); // from TTN OTAA example
	
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
}

bool LORA_CMWX1ZZABZ::joined() {
	return LoRaWAN.joined();
}

bool LORA_CMWX1ZZABZ::isBusy() {
	return LoRaWAN.busy();
}

uint8_t LORA_CMWX1ZZABZ::sendByte() {
	if(LoRaWAN.busy()) {
		return LORA_ERROR_BUSY;
	}
	if(!LoRaWAN.joined()) {
		return LORA_ERROR_JOINED;
	}

	LoRaWAN.beginPacket(3);
	LoRaWAN.write(0x00);
	LoRaWAN.write(0x01);
	LoRaWAN.write(0x02);
	LoRaWAN.write(0x03);
	LoRaWAN.endPacket(); // if TRUE then CONFIRMED packages!!!

	return LORA_NO_ERROR;
}
