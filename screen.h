#ifndef _SCREEN_
#define _SCREEN_

#include "globals.h"
#include <avr/io.h>
#include <util/delay.h>



void sendText(char *x);

void sendCommand(uint8_t x);

void sendData(uint8_t x);

void sendByte(uint8_t x);

void sendHi(uint8_t x);

void sendLo(uint8_t x);

void ScreenOn();

void SetCoursorOnFirstRow();

void SetCoursorOnSecondRow();

void ClearDisplay();

void ReturnHome();

void printint( unsigned short int x);

void InitScreen();

#endif // SCREEN