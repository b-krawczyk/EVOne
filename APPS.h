/*
  CAN.h - Library APPS operation for EVOne.
  Bartosz Krawczyk, 05.02.2020
*/

#ifndef Morse_h
#define Morse_h

#include "due_can.h"

//constants
#define V1_MAX  1.74
#define V1_MIN  0.08
#define V0_MAX  1.69
#define V0_MIN  0.13
#define SCALE_MAX 32767

float calc_percent(float v, float v_max, float v_min);
float avg(float v0, float v1);
int scale_voltages(float v0, float v1);

#endif
