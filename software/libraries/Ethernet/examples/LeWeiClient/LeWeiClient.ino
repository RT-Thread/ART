/*
  lewei50 open platform sensor client
  This code is in the public domain.
 */
#include <Ethernet.h>

#define USERKEY         "youkey" // replace your Cosm api key here

// assign a MAC address for the ethernet controller.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
// fill in your address here:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// fill in an available IP address on your network here,
// for manual configuration:

// initialize the library instance:
EthernetClient client;

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
char server[] = "open.lewei50.com";   // name address for cosm API

const unsigned long postingInterval = 10*1000; //delay between updates to cosm.com

void sendData(int thisData);
int getLength(int someValue);

void setup() {
  // start serial port:
  Serial.begin(9600);

  // start the Ethernet connection with DHCP:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;) ;
  }
  else {
    Serial.println("Ethernet configuration OK");
  }
}

int SensorOutput=0; //simulated the sensor output
int Delta = 1;
void loop() {
    sendData(SensorOutput);
    SensorOutput += Delta;
    if (SensorOutput >= 100)
    {
        Delta = -1;
    }
    else if (SensorOutput <= 0)
    {
        Delta = 1;
    }

    delay(postingInterval);
}

// this method makes a HTTP connection to the server:
void sendData(int thisData) {
  uint8_t buf[80];
  char *ptr = (char*)&buf[0];

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.print("post data...");
    Serial.println(thisData);

    // send the HTTP PUT request:
    client.println("POST /api/V1/Gateway/UpdateSensors/01 HTTP/1.1");
    client.print("userkey: ");
    client.println(USERKEY);
    client.println("Host: open.lewei50.com ");
    client.println("User-Agent: RT-Thread ART");

    client.print("Content-Length: ");

    // calculate the length of the sensor reading in bytes:
    // 8 bytes for "sensor1," + number of digits of the data:
    int thisLength = 32 + getLength(thisData);
    client.println(thisLength);

    // last pieces of the HTTP PUT request:
    client.println("Connection: close");
    client.println();

    // here's the actual content of the PUT request:
    client.print("[{\"Name\":\"RealTouch\",\"Value\":");
    client.print(thisData);
    client.println("}]"); 

    delay(1000);
  }
  
  client.stop();
}

// This method calculates the number of digits in the
// sensor reading.  Since each digit of the ASCII decimal
// representation is a byte, the number of digits equals
// the number of bytes:
int getLength(int someValue) {
  // there's at least one byte:
  int digits = 1;
  // continually divide the value by ten, 
  // adding one to the digit count for each
  // time you divide, until you're at 0:
  int dividend = someValue /10;
  while (dividend > 0) {
    dividend = dividend /10;
    digits++;
  }
  // return the number of digits:
  return digits;
}

//--------------------------------- code end -------------------------------------
