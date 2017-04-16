#ifndef INPUT_BUFFER_H
#define INPUT_BUFFER_H

#include <string.h>
#include "config_types.h"

#define INPUT_BUFFER_LENGTH     32

void input_buf_init(void);
void input_buf_clear(void);
void input_buf_add(char ch);
char *input_buf_get(void);

#endif
