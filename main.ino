#include "due_can.h"
#include "EVOne.h"

bool safe = 1;
extern int test_var;

void setup() {
  Serial.begin(9600);
  CAN_setup();
  if(CAN_check()){
    Serial.println("CAN conncetion ok");
  }
  else{
    //while(CAN_check() == 0){ // uncomment later
    //  Serial.println("Establishing CAN connection");
    //}
  }

}

void loop() {
  float v0 = read_voltage0();
  float v1 = read_voltage1();
  send_voltage_to_torque(v0,v1,0);
  //use values to get torque and send it
  Serial.println(test_var);

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
