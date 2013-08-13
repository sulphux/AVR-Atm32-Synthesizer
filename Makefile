OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
F_CPU?=16000000UL
SAMPLES_NO?=128UL
DEVICE?=atmega32
DEVICE2?=m32
PROGRAMMER?=usbasp

all: Synthesizer.hex

Synthesizer.hex: Synthesizer.elf
	avr-objcopy -O ihex -R .eeprom $< $@

Synthesizer.stripped.hex: Synthesizer.stripped.elf
	avr-objcopy -O ihex -R .eeprom $< $@

Synthesizer.elf: $(OBJS)
	avr-gcc $^ -mmcu=$(DEVICE) -Os -Wall -o $@

Synthesizer.stripped.elf: Synthesizer.elf
	cp $< $@
	avr-strip $@

%.o: %.c
	avr-gcc -c -mmcu=$(DEVICE) -Os -Wall $< -o $@ -DF_CPU=$(F_CPU) -DSAMPLES_NO=$(SAMPLES_NO) -std=gnu99

%.o: %.c %.h
	avr-gcc -c -mmcu=$(DEVICE) -Os -Wall $< -o $@ -DF_CPU=$(F_CPU) -DSAMPLES_NO=$(SAMPLES_NO) -std=gnu99

clean:
	@rm -rvf *.o Synthesizer.elf || /true

write: Synthesizer.hex
	su -c "avrdude -p $(DEVICE2) -c $(PROGRAMMER) -U flash:w:$<"

write-stripped: Synthesizer.stripped.hex
	su -c "avrdude -p $(DEVICE2) -c $(PROGRAMMER) -U flash:w:$<"


.PHONY: all clean write