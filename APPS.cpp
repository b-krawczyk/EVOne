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

float scale_voltages(float v0, float v1){ // gives value for torque
    float v0_per = calc_percent(v0, V0_MAX, V0_MIN);
    float v1_per = calc_percent(v1, V1_MAX, V1_MIN);

    float v_per_avg = avg(v0_per, v1_per);

    float v_scaled = int(SCALE_MAX*v_per_avg/100);
    return v_scaled;
}

bool check_safety(){
    // need to do this
}