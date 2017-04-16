#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>

#include "SCD5583.h"
#include "config_types.h"
#include "input_buffer.h"
#include "number_stack.h"

static uint8 init_io_pins(void);
static uint8 disp_rst(void);
static uint8 get_key(void);
static uint8 get_extended_key(void);

int main(void) {
    uint8 keynr;
    uint8 extkeynr;
    char *s, *p, buf[24];
    nrstack_func_t op_func;
    init_io_pins();
    disp_rst();
    input_buf_init();
    nrstack_init();
    clear_disp();
    disp_printf("calc started");
    while (1) {
        keynr = get_key();
        extkeynr = get_extended_key();
        op_func = nrstack_nop;
        switch (keynr) {
            case 1: case 2: case 3:
            case 4: case 5: case 6:
            case 7: case 8: case 9:
            case 10:
                clear_disp();
                input_buf_add(48 + keynr - 1);
                disp_printf(input_buf_get());
                break;
            case 11:
                clear_disp();
                input_buf_clear();
                break;
            case 12: /* enter */
                clear_disp();
                disp_printf(input_buf_get());
                nrstack_push(input_buf_get());
                input_buf_clear();
                disp_printf(" (ok)");
                break;
            case 13: op_func = nrstack_add; break;
            case 14: op_func = nrstack_sub; break;
            case 15: op_func = nrstack_mult; break;
            case 16: op_func = nrstack_div; break;
        }
        switch (extkeynr) {
            case 1: op_func = nrstack_power2; break;
            case 2: op_func = nrstack_power3; break;
            case 3: op_func = nrstack_sqrt; break;
            case 4: op_func = nrstack_log; break;
            case 5: 
                clear_disp();
                input_buf_add('.');
                disp_printf(input_buf_get());
                break;
            case 6:
                clear_disp();
                input_buf_add('E');
                disp_printf(input_buf_get());
                break;
            case 7:
                clear_disp();
                s = input_buf_get();
                if (strlen(s) == 0) break;
                p = strchr(s, '-');
                if (p != 0) {
                    *p = '+';
                    disp_printf(input_buf_get());
                    break;
                }
                p = strchr(s, '+');
                if (p != 0) {
                    *p = '-';
                    disp_printf(input_buf_get());
                    break;
                }
                p = strchr(s, 'E');
                if (p != 0) {
                    s = p + 1;
                    strcpy(buf, s);
                    *s = '-';
                    strcpy(s + 1, buf);
                    disp_printf(input_buf_get());
                    break;
                }
                strcpy(buf, s);
                *s = '-';
                strcpy(s + 1, buf);
                disp_printf(input_buf_get());
                break;
            case 8:
                clear_disp();
                input_buf_clear();
                nrstack_init();
                disp_printf("stack leer");
                break;
            default: break;
        }
        keynr = 0;
        extkeynr = 0;
        if (op_func != nrstack_nop) {
            clear_disp();
            nrstack_exec(op_func);
            disp_printf("= ");
            disp_printf(nrstack_get());
        }
    }
    return (int8)0;
}

static uint8 init_io_pins(void) {
    DDRA |= (uint8)(1 << RST_PIN);
    DDRA |= (uint8)(1 << LOAD_0_PIN);
    DDRA |= (uint8)(1 << LOAD_1_PIN);
    DDRA |= (uint8)(1 << SDCLK_PIN);
    DDRA |= (uint8)(1 << SDATA_PIN);
    PORTA &= (uint8)~(1 << RST_PIN);
    PORTA |= (uint8)(1 << LOAD_0_PIN);
    PORTA |= (uint8)(1 << LOAD_1_PIN);
    PORTA &= (uint8)~(1 << SDCLK_PIN);
    PORTA |= (uint8)(1 << SDATA_PIN);
    DDRC &= (uint8)~(1 << PINC0);
    DDRC &= (uint8)~(1 << PINC1);
    DDRC &= (uint8)~(1 << PINC2);
    DDRC &= (uint8)~(1 << PINC3);
    DDRC |= (uint8)(1 << PINC4);
    DDRC |= (uint8)(1 << PINC5);
    DDRC |= (uint8)(1 << PINC6);
    DDRC |= (uint8)(1 << PINC7);
    DDRE |= (uint8)(1 << PINE1);
    DDRE |= (uint8)(1 << PINE2);
    return 0;
}

static uint8 disp_rst(void) {
    PORTA &= (uint8)~(1 << RST_PIN);
    wait_loop(255);
    PORTA |= (uint8)(1 << RST_PIN);
    return 0;
}

static uint8 get_key(void) {
    uint8 pins[] = { PINC4, PINC5, PINC6, PINC7 };
    const uint8 pins_sz = sizeof(pins)/sizeof(pins[0]);
    uint8 row, tmp, key;
    uint16 j;
    for (row = 0; row < pins_sz; row++) PORTC &= (uint8)~(1 << pins[row]);
    for (row = 0; row < pins_sz; row++) {
        PORTC |= (uint8)(1 << pins[row]);
        key = 0;
        for (j = 0; j < 10000; j++) {
            tmp = PINC & 0x0F;
            if (tmp & (tmp - 1)); else {
                key |= tmp;
                if (key & (key - 1)) key = tmp;
            }
        }
        if (key) {
            for (tmp = PINC & 0x0F; tmp; tmp = PINC & 0x0F);
            PORTC &= (uint8)~(1 << pins[row]);
            break;
        }
        PORTC &= (uint8)~(1 << pins[row]);
    }
    if (!key) return 0;
    for (tmp = 0; key >> 1; key >>= 1, tmp++);
    key = 4 - tmp;
    return 4 * row + key;
}

static uint8 get_extended_key(void) {
    uint8 pins[] = { PINE1, PINE2 };
    uint8 row, tmp, key;
    uint16 j;
    for (row = 0; row < sizeof(pins)/sizeof(pins[0]); row++)
        PORTE &= (uint8)~(1 << pins[row]);
    for (row = 0; row < sizeof(pins)/sizeof(pins[0]); row++) {
        PORTE |= (uint8)(1 << pins[row]);
        key = 0;
        for (j = 0; j < 10000; j++) {
            tmp = PINC & 0x0F;
            if (tmp & (tmp - 1)); else {
                key |= tmp;
                if (key & (key - 1)) key = tmp;
            }
        }
        if (key) {
            for (tmp = PINC & 0x0F; tmp; tmp = PINC & 0x0F);
            PORTE &= (uint8)~(1 << pins[row]);
            break;
        }
        PORTE &= (uint8)~(1 << pins[row]);
    }
    if (!key) return 0;
    for (tmp = 0; key >> 1; key >>= 1, tmp++);
    key = 4 - tmp;
    return 4 * row + key;
}
