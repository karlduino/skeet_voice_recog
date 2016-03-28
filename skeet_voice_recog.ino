/**
 * voice recognition for skeet remote
 * modification of vr_sample_control_led.ino by JiapengLi
 * (demonstration of VoiceRecognitionModule)
 **/
  
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/**        
  Voice Recognition Module v3: http://bit.ly/1WSEPeZ
  Library: https://github.com/elechouse/VoiceRecognitionV3

  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX

   5,6,7 -----> LEDs for double, low, high
   8,9   -----> LEDs for power, ready
   10,11,12 --> activation wires for double, low, high
                 (need 470 Ohm resistor on high wire)
*/
VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

const int leds[3] = {7, 6, 5}; // high, low, double
const int activators[3] = {12, 11, 10}; // high, low, double
const int power_led = 8;
const int ready_led = 9;
const int activation_time = 250;
const int light_time = 1750;
const int delay_time = 1000;
int last_choice = 12;

#define highRecord    (0)
#define lowRecord     (1) 
#define lowaltRecord  (2)
#define dblRecord     (3)
#define dblaltRecord  (4)
#define pullRecord    (5)
#define pullaltRecord (6)


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

  delay(delay_time);

  digitalWrite(ready_led, HIGH);
}

void load_records()
{
  myVR.load((uint8_t)highRecord);
  myVR.load((uint8_t)lowRecord);
  myVR.load((uint8_t)lowaltRecord);
  myVR.load((uint8_t)dblRecord);
  myVR.load((uint8_t)dblaltRecord);
  myVR.load((uint8_t)pullRecord);
  myVR.load((uint8_t)pullaltRecord);
}

void setup()
{
  /** initialize voice recog board */
  myVR.begin(9600);
  
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
    
  load_records();

  for(int i=0; i<3; i++) {
    digitalWrite(leds[i], LOW);
  }
  
  digitalWrite(ready_led, HIGH);
}

void loop()
{
  uint8_t buf[64];
  int ret = myVR.recognize(buf, 50);
  
  if(ret>0){
    switch(buf[1]){
      case highRecord:
        activate(0);
        break;
      case lowRecord: case lowaltRecord:
        activate(1);
        break;
      case dblRecord: case dblaltRecord:
        activate(2);
        break;
      case pullRecord: case pullaltRecord:
        activate(last_choice);
        break;
    }
  }
}


