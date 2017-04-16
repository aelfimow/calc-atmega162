#include "input_buffer.h"

static char input_buffer[INPUT_BUFFER_LENGTH];
static uint8 cur_index;

void input_buf_init(void) {
    input_buf_clear();
    return;
}

void input_buf_clear(void) {
    memset(input_buffer, 0, sizeof(input_buffer));
    cur_index = 0;
    return;
}

void input_buf_add(char ch) {
    const uint8 limit =
        sizeof(input_buffer)/sizeof(input_buffer[0]);
    if (cur_index < limit - 1) {
        input_buffer[cur_index] = ch;
        cur_index++;
        input_buffer[cur_index] = 0;
    }
    return;
}

char *input_buf_get(void) { return input_buffer; }
