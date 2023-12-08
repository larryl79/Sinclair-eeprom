#include <chipreadwrite.h>

extern short int DEBUG;
extern short int SHOWADDR;
extern short int SHOWTEXT;
char INTEXT[15]  = {};

extern void ShowAddress(int Addr);

#define Q1 36   // VP, input only pin on ESP32
#define Q2 39   // VN, input only pin on ESP32
#define Q3 34   // input only pin  on ESP32
#define Q4 35   // input only pin  on ESP32
#define Q5 32
#define Q6 33
#define Q7 25
#define Q8 26

#define CE 5   // Chip Enable / Chip select #

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
    for (int n = 7; n >= 0; n-- ) {
      data = (data << 1 ) + ( digitalRead ( DATABITS[n] ) == 0 ? 0 : 1 ) ;
      }
   
return data;
}

void ShowAddress(int addr) {
  char charValBuffer[6];
  if (SHOWADDR) {
    Serial.print("0x");
    sprintf(charValBuffer, "%05X", addr);
    Serial.print(charValBuffer);
    Serial.print(": ");
  }
}


void ListEEPROM() {

int addr=0;
int wn=0;
int i =0;

Serial.println();
// ShowAddress(addr);

for (int addr=0; addr <= 0x1FFF; addr++ ) {    //8K
//for (int addr=0x00000000; addr <= 0x00003FFF; addr++ ) {    //16K
  if (wn == 0) { ShowAddress(addr); }
  
  char databyte=readEEPROM(addr);

  if ( databyte < 16) { Serial.print("0"); }     // print leading 0 to HEX number if below of 10
  Serial.print(databyte, HEX);                   // print byte in HEX
  Serial.print(" ");
  
  if ( SHOWTEXT && ( databyte < 16 || databyte > 250 ) ) { INTEXT[wn]= '.'; } else { INTEXT[wn]= databyte; }    // build 16 byte ascii text of bytes
  if ( SHOWTEXT && wn>=15 ) {      // show ascii text of 16 bytes 
    Serial.print("  |  ");
    Serial.print(INTEXT);
    }

  wn++;

  if (wn>=16) {
    Serial.println();
    wn=0;
    }

  }

 Serial.println(); 
}