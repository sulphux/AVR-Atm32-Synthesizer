
/*
 * Synthesizer.c
 *
 * Created: 2013-06-01 00:52:30
 *  Author: Cichy
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include <math.h>
#include <avr/pgmspace.h>

#include "globals.h"
#include "freq.h"
#include "screen.h"
#include "waves.h"




float decay = 0.5;
uint16_t delay_m = 500;
uint16_t delay_Samples; 
volatile uint8_t volume = 127;

uint8_t buffor[BUFF_SIZE];
uint16_t buff_ptr = 0;

volatile unsigned long delay = 0;

struct l_played{
	uint8_t note;
	uint8_t vol; 
	uint8_t type;
	uint16_t length;
	} last_played;


volatile uint8_t lastsample;
volatile uint8_t WaveType;
volatile uint8_t note = 0;
volatile uint8_t x;
volatile uint8_t y;
volatile uint8_t z;

volatile void my_delay(long del)
{
	delay = del;
	while (delay--);
}

volatile float indexINC = 0.0;
volatile float index = 0.0;

const float freqTL PROGMEM = SAMPLE_SIZE/SAMPLE_RATE;

void InitPWM()
{
	//Ustaw flagi by zainicjowac tryb PWM
	TCCR0|=_BV(WGM00)|_BV(WGM01)|_BV(COM01);
	
	 //bez preskalowania
	TCCR0|=_BV(CS00);
	
	// Noga PB3/OC0 ustawiana na wyjscie, sygnal wyjsciowy cyfrowy
	DDRB|=_BV(PB3);
}


uint8_t buff(uint8_t phase)
{
	return buffor[phase];
}


void InitCounter()
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

void play_note(uint8_t note, uint8_t vol, uint8_t type, uint16_t length)
{
	indexINC = pgm_read_float(&freqTL) * pgm_read_float(&notes[note]);
	WaveType = type;
	uint16_t len;
	volume = vol;
	for(len=0;len<length;len++)
		my_delay(1);
}

void play_pause(uint16_t length)
{
		WaveType = 4;
		uint16_t i;
		for (i=0;i<length;i++)
		    my_delay(10);
}


unsigned short int test = 0;

int main(void)
{
	cli();
	
	InitScreen();
	InitPWM();
	InitCounter();

	_delay_ms(50);
	
	SetCoursorOnFirstRow();
	sendText("Witamy");
	SetCoursorOnSecondRow();
	sendText("w symulatorze ;)");
	
	DDRA = 0x0C;
	PORTA = (1<<3);
	note = 0;
	WaveType = 1;
	
	volume = 127;
	//indexINC = pgm_read_float(&freqTL) * pgm_read_byte(&notes[note]);
	
	
	sei();
	unsigned int test_speed = 100;
    for(;;)
    {
		/*/
		indexINC = pgm_read_float(&freqTL) * pgm_read_float(&notes[note]);	
		_delay_ms(1);
		PORTC ^= 0x03;
		note++;
		if (note == 84)
		{
			note = 0;
			WaveType++;
			if(WaveType==4)
			WaveType = 0;
		}
		//*/
		///////////////////////////////////////
		// WLAZL KOTEK TEST                  //
		///////////////////////////////////////
		/*
		int not = 0;
		const uint8_t melody[] =
		{42, 39, 39, 40, 37, 37, 35, 39, 42, 85, 85,
		 42, 39, 39, 40, 37, 37, 35, 39, 35, 85, 85,
		 35, 39, 39, 37, 40, 40, 35, 39, 42, 85, 85,
		 42, 39, 39, 40, 37, 37, 35, 39, 35, 85, 85 };
		const uint8_t pauses[] =
		 {1, 1, 1, 1, 1, 1, 2, 2, 1, 2, 4,
		  1, 1, 1, 1, 1, 1, 2, 2, 1, 2, 4,
		  1, 1, 1, 1, 1, 1, 2, 2, 1, 2, 4, 
		  1, 1, 1, 1, 1, 1, 2, 2, 1, 2, 4};
			
		for (;not< 44;not++)
		{
			//=======================
			if (melody[not]!=85)
			{
				play_note(melody[not], 20,x,test_speed/pauses[not]);
				play_pause(test_speed/pauses[not]);
			}
			else
				play_pause(test_speed/pauses[not]);

			//=======================	
		}
		x++;
		if(x==4)
		x = 0;
		//*/
		///////////////////////////////
		// SZALONE ARPEGGIO TEST :D  //
		///////////////////////////////		                       
		//*/ // <--
		y=0;
		test = 0;
		for (;y<10;y++)
		{
			x=0;
			for (;x<2;x++)
			{
				uint8_t diff = 12*x + test;
				play_note(z + diff, 40, 3, 3);
				play_note(z+3 + diff, 40, 3, 3);
				play_note(z+7 + diff, 40, 3, 3);
			}
			if (y>8) test += 3;
			PORTC ^=0x03;
			z++;
			
			if (z>40)z=30;
		}
		///*/
		///////////////////////////////
		// DELAY TEST                //
		///////////////////////////////
		/*
		play_note(49, 30, 0, 100);
		play_pause(100);
		my_delay(1000);
		PORTA ^= 0x0C;
		//*/
	}
}



ISR(TIMER2_COMP_vect)
{
	OCR0 = lastsample;
	//buffor[buff_ptr] = ((lastsample-128)*0.7+128);
	//buff_ptr++;
	if (WaveType < 4)
		lastsample = pgm_read_float(&waves[WaveType][(int)index])*volume+128; // choose one type of "wave-tables"
	else 
		lastsample = 127;
	index += indexINC;
	//if (buff_ptr>= BUFF_SIZE)buff_ptr = 0;
	
	if (index >= SAMPLE_SIZE)
	{
		index -= SAMPLE_SIZE;
	}
}