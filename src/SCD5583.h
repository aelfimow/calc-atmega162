#ifndef SCD5583_H
#define SCD5583_H

#include "config_types.h"
#include <avr/pgmspace.h>
#include <string.h>

#define RST_PIN         PINA0
#define LOAD_0_PIN      PINA1
#define LOAD_1_PIN      PINA2
#define SDCLK_PIN       PINA3
#define SDATA_PIN       PINA4

#define SDCLK_T     1

extern const uint8 char_table[] PROGMEM;

uint8 disp_wr(uint8, uint8);
uint8 disp_wrline(void);
uint8 wait_loop(uint16);
uint8 disp_printf(const char *str);
uint8 clear_disp(void);

#endif
