/*
17/02/2025
Used as mood lamp system code template so..
Just copy and past into a new arduino skecth. 
*/ 

#include <IRremote.h> //version 2.6.1. Go to tool -> manage library -> search for: IRremote -> install version: 2.6.1
#include <ir_Lego_PF_BitStreamEncoder.h>

//---------------Exsample::
/*****************Hex Map For ELEGOO Remote******************
  Error when decoding: FFFFFFFF, FF, FF42BD

  Primary controles: 
    0:C101E57B ,1:9716BE3F ,2:3D9AE3F7 ,3:6182021B ,4:8C22657B 
    5:488F3CBB ,6:449E79F  ,7:32C6FDF7 ,8:1BC0157B ,9:3EC3FC1B  

  Secondary controles: 
    Power: E318261B,  Vol+: 511DBB,  Vol-: A3C8EDDB,  func/stop: EE886D7F
    Play-back: 52A3D41F, Play-forward: 20FE4DBB,  play: D7E84B1B, 
    Down: F076C13B,  Up: E5CFBD7F,  EQ: 97483BFB,  ST/REPT: F0C41643
    */

const int RECV_PIN = 3; //reciver pin

IRrecv irrecv(RECV_PIN);
decode_results cmd;

const int RED_LED = 2; //RED LED on digital pin 2
const int GREEN_LED = 4; //Green LED on digital pin 4

void setup() {
  pinMode(RED_LED, OUTPUT); //defining LEDs as output
  pinMode(GREEN_LED , OUTPUT);
  
  Serial.begin(9600); //make sure serial monitor is set to 9600

  irrecv.enableIRIn();
  irrecv.blink13(true); //flash reciver when signle detected
}

void loop() {
  if(irrecv.decode(&cmd)){
    Serial.println(cmd.value, HEX);//prints signal from remote decoded to HEX
    delay(1500); //so we dont read reciver signal to quick
    irrecv.resume(); //read reciver for signal
    
    //keep different funtionalitys under first 'if statment' above!!

    //exsample 1: 
    if(cmd.value == 0x9716BE3F){ // when button one is pressed 
      Serial.println("RED LED"); //informing operator of chosen LED
      digitalWrite(RED_LED, HIGH); //turn LED on
      Serial.println("on"); //informing operator chosen LED is on
      delay(1000); //on for 1s
      digitalWrite(RED_LED, LOW); //turn LED off
      Serial.println("************************************\noff"); //informing operator chosen LED is off
      delay(1000); //off for 1s
    } 
    
    //exsample 2: 
    if(cmd.value == 0x3D9AE3F7){ // when button two is pressed 
      Serial.println("GREEN LED"); //informing operator of chosen LED
      digitalWrite(GREEN_LED, HIGH); //turn LED on
      Serial.println("on"); //informing operator chosen LED is on
      delay(1000); //on for 1s
      digitalWrite(GREEN_LED, LOW); //turn LED off
      Serial.println("************************************\noff"); //informing operator chosen LED is off
      delay(1000); //off for 1s
    } 
  }

}