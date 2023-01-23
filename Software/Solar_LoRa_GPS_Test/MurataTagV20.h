#ifndef MurataTagV20_h
#define MurataTagV20_h

// STM32L0
#include <stdio.h>
#include <STM32L0.h>

// Arduino interface (can later be replaced by another GUI, to be more independent of Arduino)
#include "ArduinoWrapper.h"

// Helper libs
#include "HelperFunctions.h"

/** PIN definitions */
#define PIN_GPS               A2
#define PIN_GPS_RAM           A3
#define PIN_SERIAL1_RX        0

class MurataTagV20 {
	public:
		MurataTagV20();
		void initPins();
		uint16_t readSupplyVoltage();
		void stop(uint16_t ms);
};

#endif
