#include "number_stack.h"

static double nrstack[NUMBER_STACK_SIZE];
static int16 cur_index;
static char buf[32];

static void scan_result(char*);

void nrstack_init(void) {
    cur_index = -1;
    return;
}

void nrstack_push(char *ptr) {
    double z = 0.0;
    const uint8 limit = sizeof(nrstack)/sizeof(nrstack[0]);
    if (sscanf(ptr, "%le", &z)) {
        cur_index++;
        if (cur_index == limit) cur_index--;
        nrstack[cur_index] = z;
    }
    return;
}

char *nrstack_get(void) {
    buf[0] = 0;
    if (cur_index >= 0) {
        if (sprintf(buf, "%le", nrstack[cur_index]) == 0);
        else scan_result(buf);
    }
    return buf;
}

void nrstack_exec(nrstack_func_t func_nr) {
    double n;
    int16 k = 0;
    if (cur_index < 0) func_nr = nrstack_nop;
    else k = cur_index ? cur_index - 1: 0;
    switch (func_nr) {
        case nrstack_add:
            nrstack[k] += nrstack[cur_index];
            cur_index = k;
            break;
        case nrstack_sub:
            nrstack[k] -= nrstack[cur_index];
            cur_index = k;
            break;
        case nrstack_mult:
            nrstack[k] *= nrstack[cur_index];
            cur_index = k;
            break;
        case nrstack_div:
            nrstack[k] /= nrstack[cur_index];
            cur_index = k;
            break;
        case nrstack_power2:
            nrstack[cur_index] *= nrstack[cur_index];
            break;
        case nrstack_power3:
            n = nrstack[cur_index];
            nrstack[cur_index] = n * n * n;
            break;
        case nrstack_sqrt:
            nrstack[cur_index] = sqrt(nrstack[cur_index]);
            break;
        case nrstack_log:
            nrstack[cur_index] = log(nrstack[cur_index]);
            break;
        default:
            break;
    }
    return;
}

static void scan_result(char *str) {
    char *p, *s;
    strupr(str);
    p = strchr(str, 'E');
    if (p) {
        s = p;
        for (p = p - 1; *p == '0' && p > str; p--);
        strcpy(*p == '.'? p: p + 1, s);
    }
    return;
}
