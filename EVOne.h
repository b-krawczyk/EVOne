/*
  CAN.h - Library for Bus-CAN communication for EVOne.
  Bartosz Krawczyk, 05.02.2020
*/

#ifndef Morse_h
#define Morse_h

#include "due_can.h"

//constants

//IDs
#define NDRIVE_RXID 0x201
#define NDRIVE_TXID 0x180
#define CAN_LEN     3

//Write
#define SPEED_WRITE 		0x31
#define MAX_SPEED_WRITE 	0x0CCD
#define TORQUE_WRITE    	0x90
#define MAX_TORQUE_WRITE	0x7FF8
#define DISABLED            0x000451

//Request
#define TEMP_REQUEST        0x004B3D

//APPS
#define V1_MAX  1.74
#define V1_MIN  0.08
#define V0_MAX  1.69
#define V0_MIN  0.13
#define SCALE_MAX 32767
#define V_BRAKE 3.3

//CAN
void CAN_setup();
bool CAN_check();
void send_torque(int torque);
void request_temp();
int read_temp(CAN_FRAME &frame);
void read_frame();
void print_frame(CAN_FRAME &frame);

//APPS
float calc_percent(float v, float v_max, float v_min);
float avg(float v0, float v1);
int scale_voltages(float v0, float v1);
void send_voltage_to_torque(float v0, float v1, float vbrake);

#endif
