#include "MurataTagV20.h"

// TODO: read temperature
// TODO: tryout disabling ADC0 and see if smaller current during active
// TODO: check out 26MHz instead of 40MHz XTAL?

MurataTagV20::MurataTagV20() {
	
}

void MurataTagV20::initPins() {
  pinMode(PIN_GPS, OUTPUT);
  digitalWrite(PIN_GPS, LOW);

  pinMode(PIN_GPS_RAM, OUTPUT);
  digitalWrite(PIN_GPS_RAM, LOW);	
}

void MurataTagV20::stop(uint16_t ms) {
	STM32L0.stop(ms);
}

uint16_t MurataTagV20::readSupplyVoltage() {
	// TODO
	return 0;
}
