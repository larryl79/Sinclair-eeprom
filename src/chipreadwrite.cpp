#include <chipreadwrite.h>
#include <SPIFFS.h>
#include <SimpleHeartBeat.h>  // Heartbeat led lib

extern HeartBeat heartbeat;
char INTEXT[18]  = {};

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

void LatchPulse(unsigned short int pin) {   // gives a pulse for a shift register to "execute" your serial data on the parallel side.
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
    setChipEnable(false);
        
    // set addrss 1ST, then chipenable
    shiftOut(SHIFT_DATA, SHIFT_CLK, 1 , highByte(addr));
    shiftOut(SHIFT_DATA, SHIFT_CLK, 1 , lowByte(addr));
    
    //shiftOut(SHIFT_DATA, SHIFT_CLK, 1, addr >> 8);
    //shiftOut(SHIFT_DATA, SHIFT_CLK, 1, addr);
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
  char charValBuffer[9];
  if (_ADDRSHOW) {
    Serial.print("");
    Serial.print("0x");
    sprintf(charValBuffer, "%05X", addr);
    Serial.print(charValBuffer);
    Serial.print(": ");
  }
}


void ListEEPROM() {

unsigned int addr=0;
unsigned short int wn=0;

Serial.println();

for (int addr=0; addr <= 0x1FFF; addr++ ) {    //8K
//for (int addr=0x00000000; addr <= 0x00003FFF; addr++ ) {    //16K
  yield();
  heartbeat.ledtoggle();
  if (wn == 0) { ShowAddress(addr); }
  
  char databyte=readEEPROM(addr);

  if ( databyte < 16) { Serial.print("0"); }     // print leading 0 to HEX number if below of 10
  Serial.print(databyte, HEX);                   // print byte in HEX
  Serial.print(" ");
  
  if ( _TEXTSHOW && ( databyte < 16 || databyte > 250 ) ) { INTEXT[wn]= '.'; } else { INTEXT[wn]= databyte; }    // build 16 byte ascii text of bytes
  if ( _TEXTSHOW && wn>=15 ) {      // show ascii text of 16 bytes 
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

void Copy_to_bin_file() {
  unsigned int addr=0;
  unsigned short int wn=0;

  Serial.println();
  Serial.println("Copy Chip to file.");
  Serial.println();

  SPIFFS.remove("/dump.bin");
  File binfile = SPIFFS.open("/dump.bin", "w+");

  if(binfile) {
    Serial.println("Writing to \"dump.bin\" file.");
    Serial.println();
    for (int addr=0; addr <= 0x1FFF; addr++ ) {    //8K
    //for (int addr=0; addr <= 0x3FFF; addr++ ) {    //16K
      heartbeat.ledtoggle();
      yield();
      if (wn == 0) { ShowAddress(addr); }

      char databyte=readEEPROM(addr);
      binfile.print(char(databyte));

      if ( databyte < 16) { Serial.print("0"); }     // print leading 0 to HEX number if below of 10
      Serial.print(databyte, HEX);                   // print byte in HEX
      Serial.print(" ");
  
      if ( _TEXTSHOW && ( databyte < 16 || databyte > 250 ) ) { INTEXT[wn]= '.'; } else { INTEXT[wn]= char(databyte); }    // build 16 byte ascii text of bytes
      if ( _TEXTSHOW && wn>=15 ) {      // show ascii text of 16 bytes 
        Serial.print("  |  ");
        Serial.print(INTEXT);
      }

    wn++;

    if (wn>=16) {
      Serial.println();
      wn=0;
      }
    }
  }
  else {
  Serial.println("Error opening \"dump.bin\" for writing.");  
  }

binfile.close();
}


/*  example for later create a random name.
void listAllFiles()
{
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while(file)
    {
    Serial.print("FILE: ");
    Serial.println(file.name());
    file = root.openNextFile();
    }
  }
  */
