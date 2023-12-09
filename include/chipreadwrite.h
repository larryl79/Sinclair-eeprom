#ifndef CHIPREADWRITE_H
#define CHIPREADWRITE_H

#include <Arduino.h>

void setOutputPin(unsigned short int a, unsigned short int deflevel);
void setChipEnable(unsigned short int enabled);
void LatchPulse(unsigned short int);
void setAddr(unsigned int addr);
byte readEEPROM(int addr);
void ShowAddress(int addr);
void ListEEPROM();
void Copy_to_bin_file();

extern bool DEBUG;
extern bool _ADDRSHOW;
extern bool _TEXTSHOW;
extern char DATABITS[8];

#endif