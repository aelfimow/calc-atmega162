CFLAGS  = -mmcu=atmega162
CFLAGS += -I.
CFLAGS += -gdwarf-2
CFLAGS += -DF_CPU=1000000UL
CFLAGS += -Os
CFLAGS += -pedantic
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -std=gnu99

LDFLAGS  = -Wl,-u,vfprintf
LDFLAGS += -lprintf_flt
LDFLAGS += -Wl,-u,vfscanf
LDFLAGS += -lscanf_flt
LDFLAGS += -lm

all: compile link hex sizecheck
	@echo No errors

compile:
	avr-gcc -c calc.c -o calc.o ${CFLAGS}
	avr-gcc -c input_buffer.c -o input_buffer.o ${CFLAGS}
	avr-gcc -c number_stack.c -o number_stack.o ${CFLAGS}
	avr-gcc -c SCD5583.c -o SCD5583.o ${CFLAGS}

link:
	avr-gcc calc.o input_buffer.o number_stack.o SCD5583.o --output calc.elf ${CFLAGS} ${LDFLAGS}

hex:
	avr-objcopy -O ihex -R .eeprom calc.elf calc.hex

sizecheck:
	avr-size -A calc.elf
