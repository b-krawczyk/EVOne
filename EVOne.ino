#include "due_can.h"

float t = 0;
float vL = 0;
float vR = 0;
float vBrake = 0;
float vminL = 0.08;
float vmaxL = 1.69;
float vminR = 0.13;
float vmaxR = 1.74;
uint32_t old = 0;


#define BAMOCAR_ID  0x201
#define CAN_FRAME_LEN   3

//#define SerialDebug Serial

void setup() {
  //delay(2000);
  Serial.begin(115200);

  // Initialize CAN0 and CAN1, baudrate is 250kb/s
   Can0.init(CAN_BPS_250K);
   Can1.init(CAN_BPS_250K);
   
   Can0.watchFor();

   //values(vminL, vmaxL, vL, vminR, vmaxR, vR, vBrake);
   
}

void loop() {
  // Gets both voltages from average of 5 points
  CAN_FRAME framein;

  delay(10);

  if(Can0.available() > 0){
    Can0.read(framein);
    Serial.println("FRAME RECEIVED:");
    printFrame(framein);
  }
  
  vL = 0;
  vR = 0;
  
  for(int i=0;i<5;i++){
    vL += analogRead(A1)*5.0/1023.0;
    vR += analogRead(A0)*5.0/1023.0;
  }
  
  vL = vL/5;
  vR = vR/5;

  values(vminL, vmaxL, vL, vminR, vmaxR, vR, vBrake);

  delay(500);

}


float calc_percent(float vmin,float vmax,float v){
  return (v-vmin)/(vmax-vmin)*100;
}


bool check_safety(float percentL, float percentR){
  float avg = (percentL+percentR)/2;
  
  if(abs(percentL - percentR) > 5){
    Serial.print("A0: " );
    Serial.println(percentR);
    Serial.print("A1: " );
    Serial.println(percentL);
    return false;
    
  }
  else{
    Serial.print("A0: " );
    Serial.println(percentR);
    Serial.print("A1: " );
    Serial.println(percentL);
    return true;
  }
}


bool check_brake(float v){
  if(v > 2){
    return true;
  }
  else{
    return false;
  }
}


void values(float vminL, float vmaxL, float vL, float vminR, float vmaxR, float vR, float vBrake){
  float percentR = calc_percent(vminR, vmaxR, vR);
  float percentL = calc_percent(vminL, vmaxL, vL);
  float avg = (percentL + percentR)/2;
  
  if(check_safety(percentL, percentR)){
    
      if(check_brake(vBrake)){
        Serial.println("BRAKE");
        disable_enf();
      }
      else{
        //transmit signal
        send_torque(scale_percent(avg));
      }
    
  }
  else{
    delay(100);
    if(check_safety(percentL, percentR)){
      if(check_brake(0)){
        //shut down motor
        Serial.println("BRAKE");
        disable_enf();
      }
      else{
        //transmit signal
        send_torque(scale_percent(avg));
        Serial.println("Safe Again");
      }
    }
    else{
      //shut down motor
      Serial.println("Not Safe");
      disable_enf();
    }
  }
}

int scale_percent(float percent){
  if(percent > 100){
    percent = 100;
  }
  else if(percent < 5){
    percent = 0;
  }
  Serial.print("AVG PERCENT: ");
  Serial.println(percent);
  Serial.print("\r\n");
  return int(10000*percent/100);//int(32767*percent/100);
}

void send_torque(int torque){
   CAN_FRAME frameout;

   frameout.id = BAMOCAR_ID;
   frameout.length = CAN_FRAME_LEN;
   frameout.priority = 0;
   frameout.rtr = 0;
   
   frameout.data.low = 0x90 | (((torque ^ 0x7FFF) | 0x8000)<<8);
   //frameout.data.low = 0xFC1790;
   frameout.extended = 0;

   Can0.sendFrame(frameout);
   Serial.println("Frame Sent: ");
   printFrame(frameout);

   old = frameout.data.low;
}

void disable_enf(){
  CAN_FRAME frameout;

   frameout.id = BAMOCAR_ID;
   frameout.length = CAN_FRAME_LEN;
   frameout.priority = 0;
   frameout.rtr = 0;

   frameout.data.low = 0x000451;
   frameout.extended = 0;

   Can0.sendFrame(frameout);
   Serial.println("ENF DISABLED");
   while(true){
    }
   //exit(0);
}

void enable_enf(){
  CAN_FRAME frameout;

   frameout.id = BAMOCAR_ID;
   frameout.length = CAN_FRAME_LEN;
   frameout.priority = 0;
   frameout.rtr = 0;
  
   frameout.data.low = 0x000051;
   frameout.extended = 0;

   Can0.sendFrame(frameout);
   Serial.println("ENF ENABLED");
   //printFrame(frameout);
}

void printFrame(CAN_FRAME &frame) {
   Serial.print("ID: 0x");
   Serial.print(frame.id, HEX);
   Serial.print(" Len: ");
   Serial.print(frame.length);
   Serial.print(" Data: 0x");
   for (int count = 0; count < frame.length; count++) {
       Serial.print(frame.data.bytes[count], HEX);
       Serial.print(" ");
   }
   Serial.print("\r\n");
}
