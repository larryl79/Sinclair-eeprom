// #define ESP32_RTOS                      //Enable RTOS
//my program
#include <Arduino.h>
#include <main.h>
#include <FS.h>               // filesystem lib
#include <SPIFFS.h>           // SPIFFS  SPI filesystem lib
#include <helper_wifi.h>      // wifi stuff
#include <helper_ftp.h>       // FTP servrer
#include <SimpleHeartBeat.h>  // Heartbeat led lib
#include <serialcommand.h>    // keyperss commands trough serail console
#include <chipreadwrite.h>    // ROM managiigng stuff


/****************************************  Cahngable configuration *************************************/
bool DEBUG = false;
bool SHOWADDR = true ;
bool SHOWTEXT = true ;

#define Q1 36   // VP, input only pin on ESP32
#define Q2 39   // VN, input only pin on ESP32
#define Q3 34   // input only pin on ESP32
#define Q4 35   // input only pin on ESP32
#define Q5 32
#define Q6 33
#define Q7 25
#define Q8 26

#define CE 5   // Chip Enable / Chip select #

#define SHIFT_DATA 21    // D2 #define SerDat14 21
#define SHIFT_CLK 18     // D3  SRCLK11
#define SHIFT_LATCH 19    // D4  RegCLK12

// future configs
// #define EEPROM_D0 5
// #define EEPROM_D7 12
// #define WRITE_EN 13



/**************************************** Do not cahnge this configs ***********************************/
static String swversion = "0.2";
bool RunOnce = false;
char DATABITS[8] = { Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8 };

HeartBeat heartbeat(2);

/************************************************************** setup ********************************************/
void setup() {
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.println("Romwizard on ESP. Booting... V"+swversion);
  Serial.println();

  heartbeat.ledon();

  Serial.print("Flash Size: "); Serial.print(ESP.getFlashChipSize()/1024); Serial.println("Mb");
  Serial.print("Running core: "); Serial.println(xPortGetCoreID());
  Serial.printf("Current freq : %u Mhz\r\n",ESP.getCpuFreqMHz());
  Serial.println(F("START " __FILE__));
  Serial.println();

  Serial.println("Setup wifi");
  Wifi_Init();
  Serial.println("Setup Simple FTP server.");
  ftp_init();

  
  Serial.print(".");
  setOutputPin(CE,HIGH); //set chipenable pin on ESP32
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
for (int n = 0; n <= 7;  n++ )
  {
    pinMode(DATABITS[n], INPUT);
    Serial.print(".");
  }
Serial.println();
Serial.println("Boot finished.");
Serial.println();
delay(500);


/************************************************************************** program after setup *******************************/
}

void loop() {
  if (RunOnce == false) {
    RunOnce = true;
    // ListEEPROM();
    }
  yield();
  ftp_run();

  
  heartbeat.blink(1000);
  keycommand();



}
