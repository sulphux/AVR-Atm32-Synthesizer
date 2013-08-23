#include "initial_functions.h"

void initPWM()
{
	//Ustaw flagi by zainicjowac tryb PWM
	TCCR0|=_BV(WGM00)|_BV(WGM01)|_BV(COM01);
	
	 //bez preskalowania
	TCCR0|=_BV(CS00);
	
	// Noga PB3/OC0 ustawiana na wyjscie, sygnal wyjsciowy cyfrowy
	DDRB|=_BV(PB3);
}

void initCounter2()
{
	// ustaw flagi : tryb CTC
	TCCR2 |= _BV(WGM21)|_BV(COM21); 
	
	// ustaw preskaler oraz czestotliwosc przerwan
	if (MODE == 1)
	{
		TCCR2 |= _BV(CS20);
		OCR2 = 249; 
	}
	else if (MODE == 2)
	{
		TCCR2 |= _BV(CS21); 
		OCR2 = 30; 	
	}
	
	// ustaw flage przerwan dla licznika nr2
	TIMSK |= _BV(OCIE2);
	TIFR |= _BV(OCF2);
}

void turnC2on()
{
	TIMSK |= _BV(OCIE2);
	TIFR  |= _BV(OCF2);
}

void turnC2off()
{
	TIMSK &= ~(_BV(OCIE2));
	TIFR  &= ~(_BV(OCF2));
}

void initUsart()
{
	unsigned int baud_prescale = BAUD_PRESCALE;
	UBRRH = (baud_prescale >> 8);
	UBRRL = baud_prescale;
    UCSRC = (1 << URSEL)|(1 << UCSZ1)|(1 << UCSZ0);
    UCSRB |= (1 << RXCIE)|(1 << RXEN);
    #ifdef DEBUG
    UCSRB |= (1 << TXEN);
    #endif
}

void turnUsartOn()
{
    UCSRB = (1 << RXCIE)|(1 << RXEN);
    #ifdef DEBUG
    UCSRB |= (1 << TXEN);
    #endif
}

void turnUsartOff()
{
    #ifdef DEBUG
    UCSRB &= ~((1 << RXCIE)|(1 << RXEN)|(1 << TXEN));
    #else
    UCSRB &= ~((1 << RXCIE)|(1 << RXEN));
    #endif
}
