#include "screen.h"

void sendText(char *x){
	while(*x != '\0'){
		sendData(*x);
		x++;
	}
}

void sendCommand(uint8_t x){
	PORTA  &= ~RS;
	sendByte(x);
}

void sendData(uint8_t x){
	PORTA |= RS;
	sendByte(x);
}

void sendByte(uint8_t x){
	_delay_ms(1); // czekaj przed wyslaniem kazdego bajtu
	sendHi(x);
	sendLo(x);
}

void sendHi(uint8_t x){
	PORTA |= E;
	PORTA = (PORTA&(RS|UNUSED)) | E | (x & 0xF0);
	PORTA &= ~E;
}

void sendLo(uint8_t x){
	PORTA |= E;
	PORTA = (PORTA&(RS|UNUSED)) | E | (x<<4 & 0xF0);
	PORTA &= ~E;
}

void ScreenOn()
{
	sendCommand(0x0F);
}

void SetCoursorOnFirstRow()
{
	sendCommand(0x80); 
}

void SetCoursorOnSecondRow()
{
	sendCommand(0xA9);
}

void ClearDisplay()
{
	sendCommand(0x01);
}

void ReturnHome()
{
	sendCommand(0x02);
}

void printint( unsigned short int x)
{
	char tmp;
	unsigned char tmp2[8];
	short i = 0;
	while (x!=0)
	{
		tmp = x % 10;
		x /= 10;
		tmp2[i]=tmp + 48;
		i++;
	}
	tmp2[i]=NULL;
	i--;
	while (i>=0)
	{
		sendData(tmp2[i]);
		i--;
	}
}

void InitScreen()
{
	DDRA = 0xFF;
	_delay_ms(50);
	int i;
	PORTA = ~RS;
	for(i=0;i<3;i++){
		sendLo(0x03);
		_delay_ms(5);
	}
	sendLo(0x02);
	_delay_ms(5);
	sendCommand(40);
	sendCommand(0x08);
	sendCommand(0x01);
	_delay_ms(5);
	sendCommand(0x06);
	_delay_ms(50);

	ScreenOn();
	SetCoursorOnFirstRow();
}
