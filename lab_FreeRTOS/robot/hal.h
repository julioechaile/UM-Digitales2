#ifndef HAL_H
#define HAL_H

#include <stdbool.h>

int hal_read_pin(int pinId);
void hal_write_pin(int pinId, bool value);

#endif //HAL_H
