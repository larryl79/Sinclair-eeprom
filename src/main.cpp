//my program
#include <main.h>

#define DEBUG false
#define SHOWADDR true

#define Q1 36   // VP, input only pin on ESP32
#define Q2 39   // VN, input only pin on ESP32
#define Q3 34   // input only pin on ESP32
#define Q4 35   // input only pin on ESP32
#define Q5 32
#define Q6 33
#define Q7 25
#define Q8 26

#define CE 5   // Chip select #  chip enable pin


uint16_t ADDR = 0; 
char DATABITS[8] = { Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8 };

#define SHIFT_DATA 21    // D2 #define SerDat14 21
#define SHIFT_CLK 18     // D3  SRCLK11
#define SHIFT_LATCH 19    // D4  RegCLK12
// #define EEPROM_D0 5
// #define EEPROM_D7 12
// #define WRITE_EN 13

#include "chipreadwrite.h"

extern byte readEEPROM(int addr);

/************************************************************** setup ********************************************/
void setup() {
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.println("Booting...");
  Serial.println();
  Serial.print("Flash Size: "); Serial.print(ESP.getFlashChipSize()/1024); Serial.println("Mb");
  Serial.print("Running core: "); Serial.println(xPortGetCoreID());
  Serial.printf("Current freq : %u Mhz\r\n",ESP.getCpuFreqMHz());
  Serial.println(F("START " __FILE__));
  Serial.println();
  
  //set chipenable
  setOutputPin(CE,HIGH);
  //pinMode(CE, OUTPUT);
  //digitalWrite(CE, HIGH);
  Serial.print(".");
  setOutputPin(SHIFT_DATA, LOW);
  Serial.print(".");
  setOutputPin(SHIFT_CLK,LOW);
  Serial.print(".");
  setOutputPin(SHIFT_LATCH,LOW);
  Serial.print(".");
  //setOutputPin(EEPROM_D0,LOW);
  // Serial.print(".");
  // setOutputPin(EEPROM_D7,LOW);
  // Serial.print(".");
  // setOutputPin(WRITE_EN,HIGH);   // future feature
Serial.print(".");

Serial.print("datapins");
for (int n = 0; n < 7;  n++ )
  {
    pinMode(DATABITS[n], INPUT);
    Serial.print(".");
  }

Serial.println("Boot finished.");
delay(500);

 //for (int addr=0; addr < 0x1FFF; addr++ ) {
  int addr=0;
  int wn=0;
    Serial.println();
    if (SHOWADDR) {
      Serial.print("0x0");
      Serial.print(addr,HEX);
      Serial.print(": ");
    }
//for (int addr=0; addr <= 0x1FFF; addr++ ) {    //8K
for (int addr=0; addr <= 0x3FFF; addr++ ) {    //16K
  if (wn>=16) {
    Serial.println();
    if (SHOWADDR) {
      Serial.print("0x");
      Serial.print(addr,HEX);
      Serial.print(": ");
      }
    wn=0;
    }
  char databyte=readEEPROM(addr);
  if ( databyte < 16) { Serial.print("0"); }
  Serial.print(databyte, HEX);
  Serial.print(" ");
  wn++;

  } 

}

void loop() {

  
  delay(5000);

}
