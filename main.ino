#include "due_can.h"
#include "CAN.h"
#include "APPS.h"

#define V_BRAKE 3.3

bool safe = 1;

void setup() {
  Serial.begin(9600);
  CAN_setup();
  safe = CAN_check();

}

void loop() {
  float v0 = read_voltage0();
  float v1 = read_voltage1();
  send_torque(v0);
  //use values to get torque and send it

}


//Analog readings averaged

float read_voltage1(){
    float vL=0;
    for(int i=0;i<5;i++){
    vL += analogRead(A1)*5.0/1023.0;
  }
  
  return vL/5;
}

float read_voltage0(){
    float vR=0;
    for(int i=0;i<5;i++){
    vR += analogRead(A0)*5.0/1023.0;
  }
  
  return vR/5;
}


void send_voltage_to_torque(float v0, float v1, float vbrake){
    // float v_out = scale_voltages(v0,v1);

    if(vbrake < V_BRAKE){
        send_torque(v_out);
    }
    else if(v_out > 0){
        // diable enf
        Serial.println("Brake on");
    }
}
