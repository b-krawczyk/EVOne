#include "due_can.h"

uint32_t CAN_MSG_1 = 0;
#define TEST1_CAN_TRANSFER_ID    0x18FF0017 //random 29 bits
#define TEST1_CAN0_TX_PRIO       1
#define CAN_MSG_DUMMY_DATA       0xABC
#define MAX_CAN_FRAME_DATA_LEN   3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  CAN.init(CAN_BPS_250K);
  CAN2.init(CAN_BPS_250K);
  
}

void loop() {

   // put your main code here, to run repeatedly:
  CAN_FRAME frame1, frame2;

   frame1.id = TEST1_CAN_TRANSFER_ID;
   frame1.length = MAX_CAN_FRAME_DATA_LEN;
   //Priority
   frame1.priority = 0;
   frame1.rtr = 0;
   //Below we set the 8 data bytes in 32 bit (4 byte) chunks
   //Bytes can be set individually with frame1.data.bytes[which] = something
   frame1.data.low = 0x31 | (0x1234<<8);
   frame1.data.high = CAN_MSG_DUMMY_DATA;
   //We are using extended frames so mark that here. Otherwise it will just use
   //the first 11 bits of the ID set
   frame1.extended = 0;

   CAN.sendFrame(frame1);
   
   Serial.println("Sent");
   delay(1000);

}
