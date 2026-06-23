#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);
void keyboard_process(void);
uint8_t keyboard_get_layer(void);

#endif /* KEYBOARD_H */
