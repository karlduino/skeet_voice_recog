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

const int leds[4] = {9, 10, 11, 12};

#define onRecord    (0)
#define offRecord   (1) 
#define highRecord  (2)
#define lowRecord   (3)
#define dblRecord   (4)


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

void setup()
{
  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  for(int i=0; i<4; ++i) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], HIGH);
    delay(1000); 
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
  
  for(int i=0; i<4; i++) {
    digitalWrite(leds[i], LOW);
  }
  
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case onRecord:
        /** turn on all LEDs */
        for(int i=0; i<4; i++)
          digitalWrite(leds[i], HIGH);
        break;
      case offRecord:
        /** turn off LED*/
        for(int i=0; i<4; i++)
          digitalWrite(leds[i], LOW);
        break;
      case highRecord:
        /** turn on LED 9 */
        for(int i=0; i<4; i++)
          digitalWrite(leds[i], LOW);
        digitalWrite(leds[0], HIGH);
        delay(2000);
        digitalWrite(leds[0], LOW);
        break;
      case lowRecord:
        /** turn on LED 10 for 2 sec */
        for(int i=0; i<4; i++)
          digitalWrite(leds[i], LOW);
        digitalWrite(leds[1], HIGH);
        delay(2000);
        digitalWrite(leds[1], LOW);
        break;
      case dblRecord:
        /** turn on LEDs 9 and 10 for 2 sec */
        for(int i=0; i<4; i++)
          digitalWrite(leds[i], LOW);
        for(int i=2; i<4; i++)
          digitalWrite(leds[i], HIGH);
        delay(2000);
        for(int i=2; i<4; i++)
          digitalWrite(leds[i], LOW);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }
}


