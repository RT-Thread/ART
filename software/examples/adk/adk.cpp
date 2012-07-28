#include <Arduino.h>
#include <AndroidAccessory.h>

AndroidAccessory acc("Google, Inc.",
             "DemoKit",
             "DemoKit Arduino Board",
             "1.0",
             "http://www.android.com",
             "0000000012345678");

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

void setup()
{
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
  acc.powerOn();
}

void loop()
{
  byte msg[3];
  if (acc.isConnected()) {
    int len = acc.read(msg, sizeof(msg));
      
    if (len > 0){
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);               // wait for a second
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);               // wait for a second    
    }
  }
}