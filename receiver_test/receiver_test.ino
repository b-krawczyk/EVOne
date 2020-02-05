// Arduino Due - Attempts to automatically figure out bus speed for each bus then show the trafic on the buses
// By Thibaut Viard/Wilfredo Molina/Collin Kidder 2013-2017

// Required libraries
#include "variant.h"
#include <due_can.h>

//Leave defined if you use native port, comment if using programming port
//This sketch could provide a lot of traffic so it might be best to use the
//native port
//#define Serial SerialUSB

void setup()
{
  delay(2000); //give some time for everything to come up (USB takes time)
  Serial.begin(9600);
  pinMode(22,OUTPUT);
  
  //Serial.println("Doing Auto Baud scan on CAN0");
  //Can0.beginAutoSpeed();
  //Serial.println("Doing Auto Baud scan on CAN1");
  //Can1.beginAutoSpeed();

  Can0.begin(CAN_BPS_250K);
  Can1.begin(CAN_BPS_250K);
  
  //By default there are 7 mailboxes for each device that are RX boxes
  //This sets each mailbox to have an open filter that will accept extended
  //or standard frames
  int filter;
  //extended
  for (filter = 0; filter < 3; filter++) {
  Can0.setRXFilter(filter, 0, 0, true);
  Can1.setRXFilter(filter, 0, 0, true);
  }  
  //standard
  for (int filter = 3; filter < 7; filter++) {
  Can0.setRXFilter(filter, 0, 0, false);
  Can1.setRXFilter(filter, 0, 0, false);
  }  
  
}

void printFrame(CAN_FRAME &frame) {
   Serial.print("ID: 0x");
   Serial.print(frame.id, HEX);
   Serial.print(" Len: ");
   Serial.print(frame.length);
   Serial.print(" Data: 0x");
   for (int count = 0; count < frame.length; count++) {
       //Serial.print(count);
       Serial.print(frame.data.bytes[count], HEX);
       Serial.print(" ");
   }
   Serial.print("\r\n");
}

void loop(){
  CAN_FRAME incoming, frameout;
  Serial.println("Reading");

  if (Can0.available() > 0) {
  Can0.read(incoming); 
  printFrame(incoming);
  }
  if (Can1.available() > 0) {
  Can1.read(incoming); 
  printFrame(incoming);
  }

   frameout.id = 0x201; //maybe 201
   frameout.length = 3;
   //Priority
   frameout.priority = 0;
   frameout.rtr = 0;
   //Below we set the 8 data bytes in 32 bit (4 byte) chunks
   //Bytes can be set individually with frame1.data.bytes[which] = something
   frameout.data.low = 0x004b3d;
   //We are using extended frames so mark that here. Otherwise it will just use
   //the first 11 bits of the ID set
   frameout.extended = 0;

   Can0.sendFrame(frameout);
   Serial.println("Sent");
   Serial.print((incoming.data.bytes[2] <<8 |  incoming.data.bytes[1]));
   

   if((incoming.data.bytes[2] <<8 |  incoming.data.bytes[1])> 1350){
    digitalWrite(22,HIGH);
    delay(200);
   }
   
   digitalWrite(22,LOW);
    delay(200);
   
}
