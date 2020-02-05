#include "due_can.h"
#include "CAN.h"

void setup() {
  Serial.begin(9600);
  CAN_setup();

}

void loop() {
  send_torque(1000);

}
