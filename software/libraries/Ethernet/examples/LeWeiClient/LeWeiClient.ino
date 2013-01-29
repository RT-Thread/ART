#include <LeWeiClient.h>

#define USERKEY         "youkey" // replace your Cosm api key here
#define GATEWAY         "01"

LeWeiClient LeWeiClient(USERKEY, GATEWAY);

void setup()
{
    // start serial port:
    Serial.begin(9600);

    Serial.println("Starting up");
}

void loop()
{
    float value = 12.34;
    double value2 = 12.3456;

    LeWeiClient.append("test4", value2);
    LeWeiClient.append("test3", 20);
    LeWeiClient.append("test2", 40);
    LeWeiClient.append("test", value);
    LeWeiClient.send();

    delay(1000*10);
}
