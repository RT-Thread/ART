#include <Arduino.h>
#line 1 "blink\\blink.pde"
int ledPin = 13;

void setup()
{
	pinMode(ledPin,OUTPUT);
}

void loop()
{
	digitalWrite(ledPin,HIGH);
	delay(300);
	digitalWrite(ledPin,LOW);
	delay(300);
}
