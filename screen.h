#ifndef _SCREEN_
#define _SCREEN_

#include "globals.h"
#include <avr/io.h>
#include <util/delay.h>

#define _PORT_ PORTA
#define _DDR_ DDRA


void sendText(char *x);

void sendCommand(uint8_t x);

void sendData(uint8_t x);

void sendByte(uint8_t x);

void sendHi(uint8_t x);

void sendLo(uint8_t x);

void screenOn();

void setCoursorOnFirstRow();

void setCoursorOnSecondRow();

void clearDisplay();

void returnHome();

void printint( unsigned short int x);

void initScreen();

#endif // SCREEN