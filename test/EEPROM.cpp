// mess up

#include <arduino.h>

#define Q0 36   // VP, input only pin
#define Q1 39   // VN, input only pin
#define Q2 34   // input only pin
#define Q3 35   // input only pin
#define Q4 32
#define Q5 33
#define Q6 25
#define Q7 26

#define CE 5

#define SerDat 21
#define RegCLK 19
#define SRCLK  18

uint16_t ADDR = 0; 
char DATABITS[8] = { Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7 };


void setCS (bool on) {
  digitalWrite(CE, on ? LOW : HIGH);
}

void setaddr(int address) {

setCS(false);

   for (int i=0; i<13; i++) {
    //digitalWrite(ADDR[i], address & (1 << i) ? HIGH : LOW);
    }
setCS(true);
}

byte dataRead() {
  byte data;
  for (int i=0;i<7;i++)
{
bitWrite (data,i,digitalRead(DATABITS[i]));
}
return data;
}

void setup() {
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.println("booting");
  Serial.println();
  
  pinMode(CE, OUTPUT);
  digitalWrite(CE, HIGH);

for (int n = 0; n < 13;  )
  {
    pinMode(ADDR, OUTPUT);
    digitalWrite(ADDR, LOW);
  }
  
for (int n = 0; n < 7;  )
  {
    pinMode(DATABITS[n], INPUT);
  }
  

}

void loop() {

  for (int addr=0; addr < 0x1FFF; addr++ ) {
  Serial.print (addr);
  Serial.println();
  }
  

}
