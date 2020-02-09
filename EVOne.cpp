
/*
  CAN.h - Library for Bus-CAN communication for EVOne.
  Bartosz Krawczyk, 05.02.2020
*/

#include "EVOne.h"

/*
    CAN functions
*/

int test_var = 0;

void CAN_setup(){
    Can0.init(CAN_BPS_250K);
    Can1.init(CAN_BPS_250K);

    Can0.watchFor();
}

bool CAN_check(){ //requests temp, gets answer true, else false
    request_temp();
    delay(100);
    CAN_FRAME framein;
    if (Can0.read(framein)){
        return true;
    }
    else{
        return false;
    }
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
    frameout.data.low = TEMP_REQUEST;
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
        
        switch(framein.data.bytes[0]){ // checks the kind of input 
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

float calc_percent(float v, float v_max, float v_min){
    return (v-v_min)/(v_max-v_min)*100;
}

float avg(float v0, float v1){
    return (v0+v1)/2;
}

void disable(){
    CAN_FRAME frameout;
    frameout.id = NDRIVE_RXID;
    frameout.length = CAN_LEN;
    frameout.priority = 0;
    frameout.rtr = 0;
    frameout.data.low = DISABLED; //something wrong?
    frameout.extended = 0;

    Can0.sendFrame(frameout);
    Serial.println("MC Disabled!");
}

/*
    APPS functions
*/

int scale_voltages(float v0, float v1){ // gives value for torque
    float v0_per = calc_percent(v0, V0_MAX, V0_MIN);
    float v1_per = calc_percent(v1, V1_MAX, V1_MIN);


    if(abs(v0_per - v1_per) < 10){
        float v_per_avg = avg(v0_per, v1_per);

        if(v_per_avg > -5 && v_per_avg < 5){ // safty checks and upper/lower bound settings
            v_per_avg = 0;
        }
        else if(v_per_avg < -5){
            Serial.println("Dangerously low input");
            disable();
            test_var = 0;
            return 0;
        }

        if(v_per_avg > 100 && v_per_avg < 105){
            v_per_avg = 0;
        }
        else if(v_per_avg > 105){
            Serial.println("Dangerously high input");
            disable();
            test_var = 0;
            return 0;
        }
        int v_scaled = int(SCALE_MAX*v_per_avg/100);
        return v_scaled;
    }
    else{
        
        delay(100);

        if(abs(v0_per - v1_per)<10){
            float v_per_avg = avg(v0_per, v1_per);

            int v_scaled = int(SCALE_MAX*v_per_avg/100);
            return v_scaled;
        }
        else{
            Serial.println("Potentiometer reading too far appart");
            disable();
            test_var = 0;
            return 0;
        }
    }
}

void send_voltage_to_torque(float v0, float v1, float vbrake){
    float v_out = scale_voltages(v0,v1);

    if(vbrake < V_BRAKE){
        send_torque(v_out);
    }
    else if(v_out > 0){
        disable();
        test_var = 0;
        Serial.println("Brake on");
    }
}
