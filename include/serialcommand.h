#ifndef SERIAL_COMMAND_H
#define SERIAL_COMMAND_H
#include <Arduino.h>
#include <iostream>
#include <..\private\private.h>

using namespace std;

void helpmsg();
void keycommand();

extern void listAllFiles();
extern void ListEEPROM();
extern void Copy_to_bin_file();

extern bool DEBUG;
extern bool _ADDRSHOW;
extern bool _TEXTSHOW;

int incomingByte = 0;
#endif

void keycommand()
{

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("");
    Serial.print("Command received: ");
    //Serial.println(incomingByte, DEC);
    Serial.println((char)(incomingByte));

    switch (incomingByte)
    {
    case 'R' :
      ESP.restart();
      break;
    
    case 'r' :
      Serial.println("Reset sensor");
      //sensor_reset();
      break;
  
    case 'l' :
      listAllFiles();
    break;

    case 'I' :
      Serial.print(AP_SSID);
      Serial.print("       Password: ");
      Serial.print(AP_PASS);
	
      Serial.print("       AP IP: ");
      Serial.println(WiFi.softAPIP());

      //connect WIfi-Client
      Serial.println("Intit Wifi...    SSID: " + String(CLIENTSSID) + "    Password: " + String(CLIENTPASSWORD));
      
      Serial.print("IP address: ");   Serial.println(WiFi.localIP());
      Serial.print("MASK: ");         Serial.println(WiFi.subnetMask());
      Serial.print("GATEWAY: ");      Serial.println(WiFi.gatewayIP());
      Serial.print("DNS: ");          Serial.println(WiFi.dnsIP());
      Serial.println("");
      break;

    case 'h' :
    //default:
       helpmsg();
      break;

  case '1' :
      ListEEPROM();
    break;

    case '2' :
      // Dupm Chip and copy to file
      Copy_to_bin_file();
    break;

    case '8' :
      // toggle show address
      _ADDRSHOW = !_ADDRSHOW;
      Serial.print("Show ADDR before HEX: ");
      _ADDRSHOW ? Serial.println("on") : Serial.println("off");
    break;

    case '9' :
      // toggle show text
      _TEXTSHOW = !_TEXTSHOW;
      Serial.print("Show text after HEX: ");
      _TEXTSHOW ? Serial.println("on") : Serial.println("off");
    break;


    }
  }
}

void helpmsg()
{
  Serial.println();
  Serial.println("char    command description");
  Serial.println();
  Serial.println("R       Reset ESP");
  Serial.println("l       list files in FS");
  Serial.println("I       IP configuration");
  Serial.println("h       this help");
  Serial.println();
  Serial.println("1       Dump ROM to console.");
  Serial.println("2       Dump ROM to binary file.");
  Serial.println("8       Show ADDR before HEX (toggle)");
  Serial.println("9       Show text after  HEX (toggle)");
  Serial.println();
}
