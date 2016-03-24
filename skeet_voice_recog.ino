/**
 * voice recognition for skeet remote
 * modification of vr_sample_control_led.ino by JiapengLi
 * (demostration of VoiceRecognitionModule)
 **/
  
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];

const int leds[3] = {7, 6, 5}; // high, low, double
const int activators[3] = {10, 11, 12}; // high, low, double
const int power_led = 8;
const int ready_led = 9;
const int activation_time = 500;
const int light_time = 1500;
int last_choice = 10;

#define onRecord    (0)
#define offRecord   (1) 
#define highRecord  (2)
#define lowRecord   (3)
#define dblRecord   (4)
#define pullRecord  (5)


/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void activate(int pin)
{
  last_choice = pin;
  digitalWrite(ready_led, LOW);

  digitalWrite(activators[pin], HIGH);
  digitalWrite(leds[pin], HIGH);
  delay(activation_time);
  digitalWrite(activators[pin], LOW);
  delay(light_time);
  digitalWrite(leds[pin], LOW);

  digitalWrite(ready_led, HIGH);
}

void setup()
{
  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");

  pinMode(power_led, OUTPUT);
  digitalWrite(power_led, HIGH);
  pinMode(ready_led, OUTPUT);
  digitalWrite(ready_led, LOW);
  
  for(int i=0; i<3; ++i) {
    pinMode(leds[i], OUTPUT);
    pinMode(activators[i], OUTPUT);
    digitalWrite(leds[i], HIGH);
    digitalWrite(activators[i], LOW);
    delay(500);
  }
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)onRecord) >= 0){
    Serial.println("onRecord loaded");
  }
  
  if(myVR.load((uint8_t)offRecord) >= 0){
    Serial.println("offRecord loaded");
  }

  if(myVR.load((uint8_t)highRecord) >= 0){
    Serial.println("highRecord loaded");
  }

  if(myVR.load((uint8_t)lowRecord) >= 0){
    Serial.println("lowRecord loaded");
  }

  if(myVR.load((uint8_t)dblRecord) >= 0){
    Serial.println("dblRecord loaded");
  }
  
  if(myVR.load((uint8_t)pullRecord) >= 0){
    Serial.println("pullRecord loaded");
  }
  
  for(int i=0; i<3; i++) {
    digitalWrite(leds[i], LOW);
  }
  
  digitalWrite(ready_led, HIGH);
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case onRecord:
        /** turn on all LEDs */
        for(int i=0; i<3; i++)
          digitalWrite(leds[i], HIGH);
        break;
      case offRecord:
        /** turn off LED*/
        for(int i=0; i<3; i++)
          digitalWrite(leds[i], LOW);
        break;
      case highRecord:
        activate(0);
        break;
      case lowRecord:
        activate(1);
        break;
      case dblRecord:
        activate(2);
        break;
      case pullRecord:
        activate(last_choice);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }

    /** voice recognized */
    printVR(buf);
  }
}


