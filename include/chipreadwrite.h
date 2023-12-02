#ifndef CHIPREADWRITE_H
#define CHIPREADWRITE_H

#include <Arduino.h>

extern char DATABITS[8];

void setOutputPin(unsigned short int a, unsigned short int deflevel);
void setChipEnable(unsigned short int enabled);
void LatchPulse(unsigned short int);
void setAddr(unsigned int addr);
byte readEEPROM(int addr);
#endif