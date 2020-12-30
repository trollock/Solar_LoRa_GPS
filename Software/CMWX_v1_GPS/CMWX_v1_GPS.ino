/* LoRa GPS Receiver (needs a better name)
rev 0.1 J R Shipley 26/12/2020
The state machine contains 4 basic parts - 1) sleep mode (slp_mode), 2) gps mode (gps_mode), 3) transmit mode (tx_mode) and 
receive mode (rx_mode).
We will use a set of different basic counters to determine how long it should attempt to obtain a gps fix 
In the future, it would be ideal component to implement a simplified version of a PID controller to determine the 
trajectory of the battery charge state, to make basic predictions on future charge states, and the amount of time
that should be dedicated to getting a fix.

Hardware changes 
rev 0.1 - changed the ZOE-M8Q RAM backup to use a dedicated I/O on the CMWX module, so we can turn off 
battery RAM backup if the ephemeris has expired.  This would reduce the power consumption from 18 to 2.2 uA when it
has been greater than 4 hours since the last ephemeris update.
  
*/ 
#include "STM32L0.h"
#include "LoRaWAN.h"
#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS

SFE_UBLOX_GPS myGPS;
// board definitions for the power and backup to the GPS module
#define GPS_PIN A2
#define GPS_RAM A3

//some basic control variables for the timers and timeouts
enum State_enum {SLEEP_MODE, GPS_MODE, TIMER_MODE, TX_MODE}; //add RX_MODE in the future so we can create a data TX schedule for stuff on EEPROM
uint8_t state = TIMER_MODE; 

unsigned long startTime = 0;
unsigned int slp_time = 720000;

uint8_t payload[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int32_t latitude = 0;
int32_t longitude = 0;
int16_t hdop = 0;
int8_t siv = 0;

unsigned long gps_timeout = 180000; //5 minutes = 300,000 milliseconds, demo 30 seconds

//LoRaWAN provisioning data goes here. This is sometim
const char *appEui = "70B3D57ED002292B";
const char *appKey = "0ED066D83836816BD2F26EDA497761AA";
const char *devEui = "004986D34A14DC08";

void setup() {
  //Serial.begin(115200);
  Wire.begin();
  init_pins();
  init_gps();
  init_lora();
  USBDevice.detach();
}

void loop() {
  switch (state) {
       case TIMER_MODE:
          init_gps();
          startTime = millis();
          state = GPS_MODE;
          break;
    
       case GPS_MODE:
          if (millis() - startTime <= gps_timeout || hdop > 5)
          {
            /*querying the ZOE-M*Q takes ~900 millisecs, regardless if long, lat, etc or just SIV, current is ~ 35.6 mA avg whereas non-query is 27.2 mA
            this means we're saving 8.4 mA, but only checking if we've got a fix every 5 seconds.  In other words, there is a tradeoff between being able 
            check all the time and turn off once we've got a fix versus staying on for an extra 5 (we get a fix at the beginning of the 5 second sleep period)
            if the fix takes longer than ~ 30 seconds, it is more efficient to use sleep and check the module every 5 seconds if it meets our fix criteria.
            I should implement an adidtional state for GPS fix power schedule based on cold versus hot fixes
            */ 
            longitude = myGPS.getLongitude();
            //Serial.print(F(" Long: "));
            //Serial.print(longitude);
            //Serial.print(F(" (degrees * 10^-7)"));
            
            latitude = myGPS.getLatitude();
            //Serial.print(F("Lat: "));
            //Serial.print(latitude);

            hdop = myGPS.getPDOP();
        
            siv = myGPS.getSIV();
            //Serial.print(F(" SIV: "));
            //Serial.print(siv);

            //Serial.print (" , "), Serial.println(millis() - startTime);
        
            STM32L0.stop(4000);
          } else {  
          digitalWrite(GPS_PIN, LOW);
          digitalWrite(GPS_RAM, LOW);
          state = TX_MODE;
          }
          break;
    
      case TX_MODE:
          // check to see if LoRa gateway is within range and amount of fixes stored to memory,
          delay(50);
          lora_tx(); 
          state = SLEEP_MODE;
          break;  

      case SLEEP_MODE:
          // set gps_ram pin HIGH if last fix successful, set LOW if last fix > 4 hours
          // turn off all peripherals etc...
          // set timer for sleep based on power levels and time of last fix / time before ephemeric expiration
          STM32L0.stop(slp_time);
          state = TIMER_MODE;
          break;    
  }
}

void init_pins (void) {
    pinMode(GPS_PIN, OUTPUT);
      digitalWrite(GPS_PIN, LOW);
    pinMode(GPS_RAM, OUTPUT);
      digitalWrite(GPS_RAM, LOW);
}

void init_lora (void) {
    LoRaWAN.begin(EU868);
    //LoRaWAN.setSubBand(2);
    LoRaWAN.setDutyCycle(false);
    LoRaWAN.setTxPower(14);
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
    Serial.println("JOIN( )");
}

void init_gps (void) {
    digitalWrite(GPS_PIN, HIGH);
    digitalWrite(GPS_RAM, HIGH);
    delay(500);
     
    if (myGPS.begin() == false){ //Connect to the Ublox module using Wire port
    //Serial.println(F("GPS module not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
    }
    
    myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    myGPS.saveConfiguration(); //Save the current settings to flash and BBR
}

void lora_tx (void) {
     if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
      /*
        Serial.print("TRANSMIT( ");
        Serial.print("TimeOnAir: ");
        Serial.print(LoRaWAN.getTimeOnAir());
        Serial.print(", NextTxTime: ");
        Serial.print(LoRaWAN.getNextTxTime());
        Serial.print(", MaxPayloadSize: ");
        Serial.print(LoRaWAN.getMaxPayloadSize());
        Serial.print(", DR: ");
        Serial.print(LoRaWAN.getDataRate());
        Serial.print(", TxPower: ");
        Serial.print(LoRaWAN.getTxPower(), 1);
        Serial.print("dbm, UpLinkCounter: ");
        Serial.print(LoRaWAN.getUpLinkCounter());
        Serial.print(", DownLinkCounter: ");
        Serial.print(LoRaWAN.getDownLinkCounter());
        Serial.println(" )");
      */
        
          payload[0] = (byte) ((longitude & 0xFF000000) >> 24 );
          payload[1] = (byte) ((longitude & 0x00FF0000) >> 16 );
          payload[2] = (byte) ((longitude & 0x0000FF00) >> 8  );
          payload[3] = (byte) ((longitude & 0X000000FF)       );
  
          payload[4] = (byte) ((latitude & 0xFF000000) >> 24 );
          payload[5] = (byte) ((latitude & 0x00FF0000) >> 16 );
          payload[6] = (byte) ((latitude & 0x0000FF00) >> 8  );
          payload[7] = (byte) ((latitude & 0X000000FF)       );

          payload[8] = (byte) ((hdop & 0x0000FF00) >> 8  );
          payload[9] = (byte) ((hdop & 0X000000FF)       );

          payload[10] = (byte)(siv);

        LoRaWAN.beginPacket();
        LoRaWAN.write(payload, sizeof(payload));
        LoRaWAN.endPacket();
    }
}
