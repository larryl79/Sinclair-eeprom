#include <chipreadwrite.h>

#define DEBUG false

#define Q1 36   // VP, input only pin on ESP32
#define Q2 39   // VN, input only pin on ESP32
#define Q3 34   // input only pin  on ESP32
#define Q4 35   // input only pin  on ESP32
#define Q5 32
#define Q6 33
#define Q7 25
#define Q8 26

#define CE 5   // Chip select #

#define SHIFT_DATA 21    // D2 #define SerDat14 21
#define SHIFT_CLK 18     // D3  SRCLK11
#define SHIFT_LATCH 19    // D4  RegCLK12
// #define EEPROM_D0 5
// #define EEPROM_D7 12
// #define WRITE_EN 13       // future feature

//extern WRITE_EN;


void setOutputPin(unsigned short int a, unsigned short int deflevel) {
  pinMode(a, OUTPUT);
  digitalWrite(a, deflevel);
}

void LatchPulse(unsigned short int pin) {
  digitalWrite(pin, LOW);
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

void setChipEnable(unsigned short int enabled) {
    digitalWrite(CE, enabled ? 0 : 1);           // true transform to low, false transform to high
    if (DEBUG) {
        Serial.print("Chipenable status:");
        Serial.println(enabled ? true : false);
    }
}

void setAddr(unsigned int addr) {
    // setChipEnable(true);
    //shiftOut(SHIFT_DATA, SHIFT_CLK, 1 , 0x0);
    //shiftOut(SHIFT_DATA, SHIFT_CLK, 1, 0x0);
    // set addrss 1ST, then chipenable
    shiftOut(SHIFT_DATA, SHIFT_CLK, 1, addr >> 8);
    shiftOut(SHIFT_DATA, SHIFT_CLK, 1, addr);
    LatchPulse(SHIFT_LATCH);
    setChipEnable(true);
}

byte readEEPROM(int addr) {
    setAddr(addr);
    byte data=0;
    for (int n = 0; n <= 7; n += 1 )
  {
    data = (data << 1 ) + ( digitalRead ( DATABITS[n] ) == 0 ? 0 : 1 ) ;
  }
   
return data;
}

