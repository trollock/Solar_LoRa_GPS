#include <STM32L0.h>
#include "MurataTagV20.h"
#include "EEPROM_CAT.h"
#include "LORA_CMWX1ZZABZ.h"

MurataTagV20 tag = MurataTagV20(); // the tag itself, including pin definitions and connection
EEPROM_CAT memory = EEPROM_CAT(); // EEPROM memory
LORA_CMWX1ZZABZ lora = LORA_CMWX1ZZABZ(); // lora interface of the Murata MCU

uint32_t sleepTime; // feedback for test function

bool testMemory(uint32_t writeAddress, uint32_t timestampUTC, uint32_t lat, uint32_t lon) {
  const uint16_t BUFFER_LEN = 12;
  uint8_t data[BUFFER_LEN] = { 0 };
  uint8_t j = 0;
  uint32_t temp = 0;

  // check if eeprom is alive
  if(!memory.isAlive()) { return false; }

  // fill data buffer
  data[j++] = (timestampUTC >> 24); data[j++] = (timestampUTC >> 16); data[j++] = (timestampUTC >> 8); data[j++] = timestampUTC; // 4 bytes timestamp
  data[j++] = (lat >> 24); data[j++] = (lat >> 16); data[j++] = (lat >> 8); data[j++] = lat; // 4 bytes lat
  data[j++] = (lon >> 24); data[j++] = (lon >> 16); data[j++] = (lon >> 8); data[j++] = lon; // 4 bytes lon

  // storing data
  if(!memory.writeMemory(writeAddress, data, BUFFER_LEN)) { return false; }

  // wait a bit for no reason
  delay(200);

  // reading data again
  if(!memory.readMemory(writeAddress, data, BUFFER_LEN)) { return false; }
  
  // check if written values are correct
  temp = HelperFunctions::arrayToUint32(&data[0]); // read timestampUTC
  if(temp != timestampUTC) { return false; }
  temp = HelperFunctions::arrayToUint32(&data[4]); // read lat
  if(temp != lat) { return false; }
  temp = HelperFunctions::arrayToUint32(&data[8]); // read lon
  if(temp != lon) { return false; }
  
  return true;
}

void setup() {
  tag.initPins(); // initialize default state of all pins
  //Serial1.begin(9600); // no TX pin

  i2cBus.begin();
  i2cBus.setClock(400000);

  if(testMemory(0UL, 1609320158UL, 1234567UL, 9999999UL)) {
    sleepTime = 1000; // success, sleep for 1s
  }
  else {
    sleepTime = 5000; // error, sleep for 5s
  }

  // LORA connect via OTAA
  /*lora.init();
  Serial1.print("Device EUI is: "); Serial1.println(lora.devEui);
  while(!lora.isConnected()) {
    delay(100);
  }
  Serial1.println("Connected!");
  
  // LORA send something
  Serial1.print("Sending LoRa Bytes, Result: ");
  Serial1.println(lora.sendByte());*/
}

void loop() {
  /*
  if(LoRaWAN.busy()) {
    //Serial1.print("B "); Serial1.print(LoRaWAN.busy()); Serial1.print(", J "); Serial1.println(LoRaWAN.joined());
    delay(50); // wait until finished
  }
  else {
    device.stop(10000);
  }
  */
  delay(1000); // do nothing
  tag.stop(sleepTime);
}
