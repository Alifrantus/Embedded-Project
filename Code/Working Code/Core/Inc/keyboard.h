#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "config.h"
#include <stdint.h>

void keyboard_init(void);
void keyboard_task(void);  /* gọi mỗi 1ms trong main loop */

#endif /* KEYBOARD_H */
