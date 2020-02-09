/*
  CAN.h - Library APPS operation for EVOne.
  Bartosz Krawczyk, 05.02.2020
*/

#include "APPS.h"


float calc_percent(float v, float v_max, float v_min){
    return (v-v_min)/(v_max-v_min)*100;
}

float avg(float v0, float v1){
    return (v0+v1)/2;
}

int scale_voltages(float v0, float v1){ // gives value for torque
    float v0_per = calc_percent(v0, V0_MAX, V0_MIN);
    float v1_per = calc_percent(v1, V1_MAX, V1_MIN);


    if(abs(v0_per - v1_per) < 10){
        float v_per_avg = avg(v0_per, v1_per);

        if(v_per_avg < 0 && v_per_avg > -5){ // safty checks and upper/lower bound settings
            v_per_avg = 0;
        }
        else if(abs(v_per_avg) < 5){
            Serial.println("Dangerously low input");
            // disable enf
            return 0;
        }

        if(v_per_avg > 100 && v_per_avg < 105){
            v_per_avg = 0;
        }
        else if(v_per_avg > 105){
            Serial.println("Dangerously high input");
            // disable enf
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
            return 0;
        }
    }
}
