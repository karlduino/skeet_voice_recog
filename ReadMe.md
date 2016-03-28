## Skeet voice recognition

Arduino software for a device to activate a skeet transmitter
([LongRange T4300](http://www.longrangellc.com/T4300-Transmitters.html))
via voice commands.

Using an [Arduino Nano](https://www.arduino.cc/en/Main/ArduinoBoardNano) (actually,
[this clone](http://www.amazon.com/exec/obidos/ASIN/B00SGMEH7G/7210-20))
and a [Voice Recognition Module v3](http://bit.ly/1WSEPeZ). 
 
Also need the [VoiceRecognitionV3](https://github.com/elechouse/VoiceRecognitionV3) library.

Connections:
 - Voice Recognition Module TX to Arduino D2
 - Voice Recognition Module RX to Arduino D3
 - Arduino D5, D6, D7 to LEDs for double, low, high
 - Arduino D10, D11, D12 to activation wires for double, low, high
 - Arduino D8, D9 to LEDs for power, ready
 
Found that we needed a 470 Ohm resistor on the "high" wire to the
skeet transmitter.

                    
