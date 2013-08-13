// default Atmega32 CPU frequency
#define F_CPU 16000000UL

// Screen RS pin
#define RS 0b00000001
// Screen E pin
#define E 0b00000010
// Not used 2 pins on port A
#define UNUSED 0b00001100
// if avr-gcc doesn't see NULL is defined..
#define NULL 0

#define CDIV 857

// default size of sample table
#define SAMPLE_SIZE 256.0
// buffor size for dynamic sample effector 
#define BUFF_SIZE 1532

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