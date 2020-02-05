#include "CAN.h"

void CAN_setup(){
    Can0.init(CAN_BPS_250K);
    Can1.init(CAN_BPS_250K);

    Can0.watchFor();
}

void CAN_check(){
    //request temp, if id agrres ok, else print and idle mode
}

void send_torque(int torque){
    CAN_FRAME frameout;
    frameout.id = NDRIVE_RXID;
    frameout.length = CAN_LEN;
    frameout.priority = 1;
    frameout.rtr = 0;
    frameout.data.low = TORQUE_WRITE | (((torque ^ 0x7FFF) | 0x8000)<<8); //something wrong?
    frameout.extended = 0;

    Can0.sendFrame(frameout);
    Serial.print("Torque sent: ");
    Serial.println(torque);
}

void request_temp(){
    CAN_FRAME frameout;
    frameout.id = NDRIVE_RXID;
    frameout.length = CAN_LEN;
    frameout.priority = 2;
    frameout.rtr = 0;
    frameout.data.low = TEMP_REQUEST; //something wrong?
    frameout.extended = 0;

    Can0.sendFrame(frameout);
    Serial.println("Requested Temperature");
}

int read_temp(CAN_FRAME &frame){
    Serial.print("Temp received: ");
    int temp = frame.data.bytes[2] <<8 |  frame.data.bytes[1];
    Serial.println(temp);
    return temp;
    //further instructions for temp?
}

void read_frame(){
    CAN_FRAME framein;

    while (Can0.available() > 0) {
        Can0.read(framein);
        
        switch(framein.data.bytes[0]){
            case 0x4B:
                read_temp(framein);
                break;

            default:
                Serial.println("FrameIn not recognised!");
                Serial.print("Frame ID: ");
                Serial.println(framein.data.bytes[0]);
        }
    }
  }

void print_frame(CAN_FRAME &frame){
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
