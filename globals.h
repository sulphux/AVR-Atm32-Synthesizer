#ifndef _GLOBALS_
#define _GLOBALS_


#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

//////////////////////
/// D E F I N E S  ///
//////////////////////

// default Atmega32 CPU frequency
#define F_CPU 16000000UL

// Screen RS pin
#define RS 0b00000001
// Screen E pin
#define E 0b00000010
// Not used 2 pins on port A
#define UNUSED 0b00001100
// if avr-gcc doesn't see NULL is defined..
#ifndef NULL
#define NULL 0
#endif
// the same situation with 'true' and 'false'
#ifndef true
#define true 1
#endif
//---------------
#ifndef false 
#define false 0
#endif
//---------------
#define bool uint8_t
#define CDIV 857


// SAMPLING
// default size of sample table
#define SAMPLE_SIZE 256.0
// buffor size for dynamic sample effector 
#define BUFF_SIZE 1532
// default sample rate
#define SAMPLE_RATE 8000


// USART
#define BAUD_RATE 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD_RATE * 16UL))) -1)
/// DEBUG VALUES 

// ------------------------------------------------------------------
// two different modes which change sample rates for debugging
#define SET_MODE_2

#ifdef SET_MODE_1
#define MODE 1
#endif

#ifdef SET_MODE_2
#define MODE 2
#endif
// ------------------------------------------------------------------

#endif // _GLOBALS_