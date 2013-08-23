
/*
 * Synthesizer.c
 *
 * Created: 2013-06-01 00:52:30
 *  Author: Cichy
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include <math.h>

#include "globals.h"
#include "freq.h"
#include "screen.h"
#include "waves.h"
#include "initial_functions.h"


const float freqTL PROGMEM = SAMPLE_SIZE/SAMPLE_RATE;

volatile bool pressed = false;
volatile bool generating = false;

volatile float decay = 0.5;
volatile uint16_t delay_m = 500;
volatile uint16_t delay_Samples; 
volatile uint8_t volume = 127;

volatile uint8_t buffor[BUFF_SIZE];
volatile uint16_t buff_ptr = 0;

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
volatile uint8_t key;

uint8_t convertKey(void)
{
    uint8_t tmp = 0;
	if (key > 96)
		tmp = key - 32;
	return tmp;
}

void my_delay(long del)
{
	delay = del;
	while (delay--);
}

volatile float indexINC = 0.0;
volatile float index = 0.0;



uint8_t buff(uint8_t phase)
{
	return buffor[phase];
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

void play_arp(uint8_t note, uint8_t vol, uint8_t type, uint16_t length, uint8_t density)
{
    uint16_t len;
    len = 0;

    while (len < length)
    {
        play_note(note, vol, type, density);
        len+=density;
        play_note(note+12, vol, type, density);
        len+=density;
        play_note(note+24, vol, type, density);
        len+=density;
    }
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
	initScreen();
	initPWM();
	initCounter2();
	initUsart();
	_delay_ms(50);

	DDRD = 0x03;
	DDRC = 0b00000011;

	PORTC = 0x01;

	key = 0;	
	note = 0;
	WaveType = 1;
	
	volume = 127;

	//indexINC = pgm_read_float(&freqTL) * pgm_read_byte(&notes[note]);
	sei();
	unsigned int test_speed = 100;
    for(;;)
    {
    	if(pressed == 1)
    	{
    		turnUsartOff();
            PORTC ^= 0x03;
            if (key > 40)
            {
                uint8_t readedKey = key;
                key = 0;
                if (readedKey > 96) readedKey -= 32;
                readedKey -= 40;

                if (readedKey >= 0 && readedKey < 60) readedKey = pgm_read_byte(&key_map[readedKey]);
                else readedKey = 0;

                turnC2on();
                play_arp(readedKey, 50, 1, 50, 10);
                turnC2off();
            }
            pressed = 0;
            turnUsartOn();
    	}else
    	{
    		play_pause(1);
    	}
    	//*/
    	////////////////////////////
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
		/*/ // <--
		y=0;
		test = 0;
		for (;y<10;y++)
		{
			x=0;
			for (;x<2;x++)
			{
				uint8_t diff = 12*x + test;
				play_note(z + diff, 50, 3, 3);
				play_note(z+2 + diff, 50, 3, 3);
				play_note(z+7 + diff, 50, 3, 3);
			}
			if (y>8) test += 2;
			PORTC ^=0x03;
			z++;
			
			if (z>40)z=30;
		}
		//*/
		///////////////////////////////
		// DELAY TEST                //
		///////////////////////////////
		/*
		play_note(40, 6, 0, 100);
		play_pause(100);
		my_delay(1000);
		//*/
	}
}



ISR(TIMER2_COMP_vect)
{

	OCR0 = lastsample;

	if (WaveType < 4)
	{
        lastsample = pgm_read_float(&waves[WaveType][(int)index])*volume; // choose one type of "wave-tables"
		lastsample = (lastsample) + (1<<7);
		if (index >= SAMPLE_SIZE)
		{
			index -= SAMPLE_SIZE;
		}
	}
	else 
		lastsample = (1<<7);
	
	index += indexINC;

}

ISR(USART_RXC_vect)
{
    //turnC2off();

    key = UDR;

    // DEBUG
    #ifdef DEBUG
    UDR = key;
    returnHome();
    printint(key);
    sendText("  ");
    #endif

    pressed = 1;
}
