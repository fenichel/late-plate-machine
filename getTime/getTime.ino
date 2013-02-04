
/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
http://www.kloth.net/software/timesrv1.php
 */


#include <SPI.h>
#include <Time.h>
#include <WiFi.h>

#define YEAR_INDEX 6
#define MONTH_INDEX 9
#define DAY_INDEX 12
#define HOUR_INDEX 15
#define MINUTE_INDEX 18
#define SECOND_INDEX 21

char ssid[] = ; //  your network SSID (name) 
char pass[] = ;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(173,194,73,105);  // numeric IP for Google (no DNS)
char server[] = "time.nist.gov";    // name address for Google (using DNS)

    // The "r" prefix is just to avoid name collisions.
  int charsReceived = 0;
  int rHour = 0;
  int rMinute = 0;
  int rSec = 0;
  int rDay = 0;
  int rMonth = 0;
  int rYear = 0;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(2000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();

  requestTime();
}

// Ask the server named above for the time, politely :)
void requestTime() {
  if (client.connect(server, 13)) {  // DAYTIME
    // Make a HTTP request:
    client.println("GET /");
    client.println();
  }
}

// Convert a character to an int and multiply by a multiplier if necessary
int atoiMult(char c, int multiplier) {
 return multiplier * (c - '0'); 
}

void loop() {
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    charsReceived++;
    switch (charsReceived - 2) {
      case YEAR_INDEX:
        rYear += atoiMult(c, 10);
        break;
      case YEAR_INDEX + 1:
        rYear += atoiMult(c, 1);
        break;
      case MONTH_INDEX:
        rMonth += atoiMult(c, 10);
        break;
      case MONTH_INDEX + 1:
        rMonth += atoiMult(c, 1);
        break;
      case DAY_INDEX:
        rDay += atoiMult(c, 10);
        break;
      case DAY_INDEX + 1:
        rDay += atoiMult(c, 1);
        break;
      case HOUR_INDEX:
        rHour += atoiMult(c, 10);
        break;
      case HOUR_INDEX + 1:
        rHour += atoiMult(c, 1);
        break;
      case MINUTE_INDEX:
        rMinute += atoiMult(c, 10);
        break;
      case MINUTE_INDEX + 1:
        rMinute += atoiMult(c, 1);
        break;
      case SECOND_INDEX:
        rSec += atoiMult(c, 10);
        break;
      case SECOND_INDEX + 1:
        rSec += atoiMult(c, 1);
        break;
      default:
        break;
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    client.stop();
    if (rYear != 0) {
      //Succeeded at reading back the time
    rHour = (rHour + 16) % 24;
    setTime(rHour, rMinute, rSec, rDay, rMonth, rYear);
    // do nothing forevermore:
    while(true);
    }
    else {
      requestTime();
    }
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}





