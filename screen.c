#include "screen.h"


void sendText(char *x){
	while(*x != '\0'){
		sendData(*x);
		x++;
	}
}

void sendCommand(uint8_t x){
	_PORT_  &= ~RS;
	sendByte(x);
}

void sendData(uint8_t x){
	_PORT_ |= RS;
	sendByte(x);
}

void sendByte(uint8_t x){
	_delay_ms(1); // czekaj przed wyslaniem kazdego bajtu
	sendHi(x);
	sendLo(x);
}

void sendHi(uint8_t x){
	_PORT_ |= E;
	_PORT_ = (_PORT_&(RS|UNUSED)) | E | (x & 0xF0);
	_PORT_ &= ~E;
}

void sendLo(uint8_t x){
	_PORT_ |= E;
	_PORT_ = (_PORT_&(RS|UNUSED)) | E | (x<<4 & 0xF0);
	_PORT_ &= ~E;
}

void screenOn()
{
	sendCommand(0x0F);
}

void setCoursorOnFirstRow()
{
	sendCommand(0b10000000); 
}

void setCoursorOnSecondRow()
{
	sendCommand(0b11000000);
}

void clearDisplay()
{
	sendCommand(0x01);
}

void returnHome()
{
	_delay_ms(1);
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

void initScreen()
{
	_DDR_ = 0xFF;
	_delay_ms(50);
	int i;
	_PORT_ = ~RS;
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

	screenOn();
	setCoursorOnFirstRow();
}

void generateCharacter(uint8_t newChar[], uint8_t cgAddress)
{
	sendCommand(0x40 + cgAddress*8);
	sendData(newChar[0]);
	sendData(newChar[1]);
	sendData(newChar[2]);
	sendData(newChar[3]);
	sendData(newChar[4]);
	sendData(newChar[5]);
	sendData(newChar[6]);
	sendData(newChar[7]);
	sendCommand(0x80);
}

void putChar(char c)
{
	sendData(c);
}
