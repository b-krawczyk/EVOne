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

//Request
#define TEMP_REQUEST        0x004B3D

void CAN_setup();
void CAN_check();
void send_torque(int torque);
void request_temp();
int read_temp(CAN_FRAME &frame);
void read_frame();
void print_frame(CAN_FRAME &frame);

#endif