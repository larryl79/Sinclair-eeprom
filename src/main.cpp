//my program
#include <main.h>
#define DEBUG false
#define SHOWADDR false

#define Q0 36   // VP, input only pin
#define Q1 39   // VN, input only pin
#define Q2 34   // input only pin
#define Q3 35   // input only pin
#define Q4 32
#define Q5 33
#define Q6 25
#define Q7 26

#define CE 5   // Chip select #


uint16_t ADDR = 0; 
char DATABITS[8] = { Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7 };

#define SHIFT_DATA 21    // D2 #define SerDat14 21
#define SHIFT_CLK 18     // D3  SRCLK11
#define SHIFT_LATCH 19    // D4  RegCLK12
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

void setOutputPin(unsigned short int a, unsigned short int deflevel) {
  pinMode(a, OUTPUT);
  digitalWrite(a, deflevel);
}

void LatchPulse(unsigned short int pin){
  digitalWrite(pin, LOW);
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

void setChipEnable(unsigned short int enabled){
    digitalWrite(CE, enabled ? 0 : 1);           // true transform to low, false transform to high
    if (DEBUG) {
        Serial.print("Chipenable status:");
        Serial.println(enabled ? true : false);
    }
}

void setAddr(unsigned int addr){
    setChipEnable(true);
    //shiftOut(SHIFT_DATA, SHIFT_CLK, 1 , 0x0);
    //shiftOut(SHIFT_DATA, SHIFT_CLK, 1, 0x0);
    shiftOut(SHIFT_DATA, SHIFT_CLK, 1, addr >> 8);
    shiftOut(SHIFT_DATA, SHIFT_CLK, 1, addr);
    LatchPulse(SHIFT_LATCH);
    setChipEnable(true);
}

byte readEEPROM(int addr){
    setAddr(addr);
    byte data=0;
    for (int n = 7; n >= 0; n-= 1 )
  {
    data = (data << 1 ) + digitalRead (DATABITS[n]);
  }
   
return data;
}


/************************************************************** setup ********************************************/
void setup() {
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.println("booting");
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
  setOutputPin(WRITE_EN,HIGH);
Serial.print(".");

Serial.print("datapins");
for (int n = 0; n < 7;  n++ )
  {
    pinMode(DATABITS[n], INPUT);
    Serial.print(".");
  }

Serial.println("Boot finished.");

 //for (int addr=0; addr < 0x1FFF; addr++ ) {
  int addr=0;
  int wn=0;
    Serial.println();
    if (SHOWADDR) {
      Serial.print("0x0");
      Serial.print(addr,HEX);
      Serial.print(": ");
    }
for (int addr=0; addr <= 0x1FFF; addr++ ) {    //8K
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
