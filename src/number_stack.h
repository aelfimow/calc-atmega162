#ifndef NUMBER_STACK
#define NUMBER_STACK

#include "config_types.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUMBER_STACK_SIZE   32

typedef enum {
    nrstack_nop,
    nrstack_add,
    nrstack_sub,
    nrstack_mult,
    nrstack_div,
    nrstack_power2,
    nrstack_power3,
    nrstack_sqrt,
    nrstack_log
} nrstack_func_t;

void nrstack_init(void);
void nrstack_push(char *);
char *nrstack_get(void);
void nrstack_exec(nrstack_func_t);

#endif
