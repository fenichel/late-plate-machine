/*
  Web  Server
 
 A simple web server that shows the value of the analog input pins.
 using a WiFi shield.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 Circuit:
 * WiFi shield attached
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */
#include <SPI.h>
#include <WiFi.h>
#include <String.h>

int NUM_NAMES = 11;

char ssid[] = ;      //  your network SSID (name) 
char pass[] = ;   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

//String names[] = {"Rachel", "Charlotte", "Emily", "Cameron", "George"};
String names[] = {"Aharon", "Alex", "Allie", "Annie", "Annie", "Becca", "Ben", "Cameron", "Carly", "Catie", "Charlotte_B"};//, "Charlotte_G", "Charlotte_G-B", "Chris_Gold", "Chris_Tang", "Cordelia", "Daphne", "Darren", "Dima", "Divya", "Doria", "Elise", "Emily", "Eric", "Ethan", "Everett", "Flora", "George", "Gideon", "Ginny", "Hanna", "Hilary", "Jena", "John", "Josh", "Justin", "Kevin", "Lana", "Layne", "Mark", "Matt", "Max", "Micah", "Rachel", "Rebecca", "Reid", "Rohan", "Samir", "Scott", "Thomissa", "Troy", "Vika", "Will", "Yuliya", "Clear_all_meals"};

int lunches[10];
int dinners[10];
int lunchIndex = 0;
int dinnerIndex = 0;


String line1 = "                ";
String line2 = "                ";

// Strings stored in flash mem for the Html Header
prog_char Header_0[] PROGMEM = "HTTP/1.1 200 OK";            //
prog_char Header_1[] PROGMEM = "Content-Type: text/html";    //
prog_char Header_2[] PROGMEM = "Connection: close";         //
prog_char Header_3[] PROGMEM = "";                           //
prog_char Header_4[] PROGMEM = "<!DOCTYPE HTML>";             // The header lines
prog_char Header_5[] PROGMEM = "<html>"; //
//prog_char Header_5[] PROGMEM = "</HEAD><BODY>";              //
 
// A table of pointers to the flash memory strings for the header
PROGMEM const char *Header_table[] =
{  
  Header_0,
  Header_1,
  Header_2,
  Header_3,
  Header_4,
  Header_5};

// Strings stored in flash mem for the body of the webpage, including the input forms
// Only the longer lines, not involving variables are stored here
prog_char Body_0[] PROGMEM = "<H2>Today's Late Plates:</H2>";            
prog_char Body_1[] PROGMEM = "<H2>Sign up for a late plate!</H2>";    
prog_char Body_2[] PROGMEM = "<form action=\"/?\" method=get>";                          
prog_char Body_3[] PROGMEM = "<b>Name: </b><input type=\"text\" name=\"Name\" maxlength=\"16\" size=\"16\" /><br />";            
prog_char Body_4[] PROGMEM = "<b>Meal: </b><input type=\"radio\" name=\"Meal\" value=\"Lunch\" />Lunch<br />";
prog_char Body_5[] PROGMEM = "<b>Meal: </b><input type=\"radio\" name=\"Meal\" value=\"Dinner\" />Dinner<br />";
prog_char Body_6[] PROGMEM = "<input type=\"submit\" value=\"Submit\" /></form>";              
 
// A table of pointers to the flash memory strings for the body
PROGMEM const char *Body_table[] =
{  
  Body_0,
  Body_1,
  Body_2,
  Body_3,
  Body_4,
  Body_5,
  Body_6};
  
void setup() {
  for (int i = 0; i < 10; i++) {
   lunches[i] = -1;
  dinners[i] = -1; 
  }
  
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
    status = WiFi.begin(ssid);//, pass);

    // wait 10 seconds for connection:
    delay(1000);
  } 
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}

//////////////////////////////////////////////////////////////////
 
// Print out MIME and HTML header at top of webpage using the
// strings previously stored in flash memory
void HtmlHeader(WiFiClient client) {
    Serial.println("Sending html header");
    char buffer[35]; //A character array to hold the strings from the flash mem
   
    for (int i = 0; i < 6; i++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(Header_table[i]))); // Necessary casts and dereferencing, just copy.
      client.println( buffer );
    }
}
 
// Print the footer at the bottom of the webpage
void HtmlFooter(WiFiClient client) {
    Serial.println("Sending html footer");
    client.println("</html?>");
}

void sendHtmlBody(WiFiClient client) {
    char buffer[90]; //A character array to hold the strings from the flash mem
     
    strcpy_P(buffer, (char*)pgm_read_word(&(Body_table[0])));        // gets and prints the first line
    client.println( buffer );
    client.println("<b>Lunch requests:</b><br />");
    for (int i = 0; i < 10; i++) {
      if (lunches[i] != -1) {
        client.println(names[lunches[i]] + "<br />");
      }
    }
    client.println("<b>Dinner requests:</b><br />");
    for (int i = 0; i < 10; i++) {
     if (dinners[i] != -1) {
       client.println(names[dinners[i]] + "<br />");
     } 
    }
    for(int i = 1; i < 7; i++) {                                     // Use a for loop to print the remaining lines
    strcpy_P(buffer, (char*)pgm_read_word(&(Body_table[i])));
    client.println( buffer );
    }
}

void addRequest(String name, String meal) {
  int indexOfName = -1;
 for (int i = 0; i < NUM_NAMES; i++) {
   if (names[i].equals(name)) {
     indexOfName = i;
   }
 }
 if (indexOfName != -1) {
   if (meal.equals("Lunch")){
     lunches[lunchIndex] = indexOfName;
     lunchIndex++;
   }
   else if (meal.equals("Dinner")) {
     dinners[dinnerIndex] = indexOfName;
     dinnerIndex++;
   }
 }
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  String recent = "    ";
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // Did we find a question mark? That's an argument, so let's start parsing.
        if (c == '?') {
          String text = "";
         while ((c = client.read()) != ' ' && client.available()) {
          text += c; 
        }
        // Hax to avoid getting caught up in the referrer part of the request without having to store a lot of state.
        if((c = client.read()) == 'H') {
        // Do some parsing, dawg.
        int indexOfDelim = 0;  //This variable stores the location of our delimiter so we can find where line1 ends and line2 begins
        
        text.replace("Name=", "");
        text.replace("&Meal=", "`");
        indexOfDelim = text.indexOf("`");       // find the index of that delimiter
        String name = text.substring(0, indexOfDelim); // set line1 and line 2 using that knowledge
        String meal = text.substring(indexOfDelim+1, text.length());
        addRequest(name, meal);
      }
        }
        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          HtmlHeader(client);
          sendHtmlBody(client);
          HtmlFooter(client);
          
          while (client.available()) {
            c = client.read();
          }
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
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

