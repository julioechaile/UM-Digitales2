#include "hal.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static int gpioA[3] ;
#define left rand()%2
#define right rand()%2
#define back rand()%2

int hal_read_pin(int pinId)
{ 
  gpioA[0]= left;
  gpioA[1]= right;
  gpioA[2]= back;
  printf("leer %d\n\r",gpioA[pinId]);
  return gpioA[pinId];
}

void hal_write_pin(int pinId, bool value)
{
  gpioA[pinId] = value;
  printf("escribir\n\r");
}

