#include "due_can.h"
#include "CAN.h"

bool safe = 1;

void setup() {
  Serial.begin(9600);
  CAN_setup();
  safe = CAN_check();

}

void loop() {
  send_torque(1000);
  Serial.print(safe);

}
