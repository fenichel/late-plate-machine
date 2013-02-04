#include <SPI.h>
#include <WiFi.h>
#include <String.h>
#include <Time.h>

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 18, 17, 16, 15, 14);

int NUM_NAMES = 55;

char ssid[] = ;      //  your network SSID (name) 
char pass[] = ;   // your network password

int status = WL_IDLE_STATUS;

// We will be acting as a server
WiFiServer server(80);
// But will sometimes need to be a client in order to request 
WiFiClient client;

// Sigh.  This is forced by the abysmally small size of the Uno's SRAM.
// See http://arduino.cc/en/Tutorial/Memory
prog_char s_0[] PROGMEM = "Aharon";
prog_char s_1[] PROGMEM = "Alex";
prog_char s_2[] PROGMEM = "Alli";
prog_char s_3[] PROGMEM = "Annie_P";
prog_char s_4[] PROGMEM = "Annie_D";
prog_char s_5[] PROGMEM = "Becca";
prog_char s_6[] PROGMEM = "Ben";
prog_char s_7[] PROGMEM = "Cameron";
prog_char s_8[] PROGMEM = "Carly";
prog_char s_9[] PROGMEM = "Catie";
prog_char s_10[] PROGMEM = "Charlotte_B";
prog_char s_11[] PROGMEM = "Charlotte_G";
prog_char s_12[] PROGMEM = "Charlotte_G-B";
prog_char s_13[] PROGMEM = "Chris_Gold";
prog_char s_14[] PROGMEM = "Chris_Tang";
prog_char s_15[] PROGMEM = "Cordelia";
prog_char s_16[] PROGMEM = "Daphne";
prog_char s_17[] PROGMEM = "Darren";
prog_char s_18[] PROGMEM = "Dima";
prog_char s_19[] PROGMEM = "Divya";
prog_char s_20[] PROGMEM = "Doria";
prog_char s_21[] PROGMEM = "Elise";
prog_char s_22[] PROGMEM = "Emily";
prog_char s_23[] PROGMEM = "Eric";
prog_char s_24[] PROGMEM = "Ethan";
prog_char s_25[] PROGMEM = "Everett";
prog_char s_26[] PROGMEM = "Flora";
prog_char s_27[] PROGMEM = "George";
prog_char s_28[] PROGMEM = "Gideon";
prog_char s_29[] PROGMEM = "Ginny";
prog_char s_30[] PROGMEM = "Hanna";
prog_char s_31[] PROGMEM = "Hilary";
prog_char s_32[] PROGMEM = "Jena";
prog_char s_33[] PROGMEM = "John";
prog_char s_34[] PROGMEM = "Josh";
prog_char s_35[] PROGMEM = "Justin";
prog_char s_36[] PROGMEM = "Kevin";
prog_char s_37[] PROGMEM = "Lana";
prog_char s_38[] PROGMEM = "Layne";
prog_char s_39[] PROGMEM = "Mark";
prog_char s_40[] PROGMEM = "Matt";
prog_char s_41[] PROGMEM = "Max";
prog_char s_42[] PROGMEM = "Micah";
prog_char s_43[] PROGMEM = "Rachel";
prog_char s_44[] PROGMEM = "Rebecca";
prog_char s_45[] PROGMEM = "Reid";
prog_char s_46[] PROGMEM = "Rohan";
prog_char s_47[] PROGMEM = "Samir";
prog_char s_48[] PROGMEM = "Scott";
prog_char s_49[] PROGMEM = "Thomissa";
prog_char s_50[] PROGMEM = "Troy";
prog_char s_51[] PROGMEM = "Vika";
prog_char s_52[] PROGMEM = "Will";
prog_char s_53[] PROGMEM = "Yuliya";
prog_char s_54[] PROGMEM = "Clear_all_meals";

PROGMEM const char *names[] = { s_0, s_1, s_2, s_3, s_4, s_5, s_6, s_7, s_8, s_9, s_10, s_11, s_12, s_13, s_14, s_15, s_16, s_17, s_18, s_19, s_20, s_21, s_22, s_23, s_24, s_25, s_26, s_27, s_28, s_29, s_30, s_31, s_32, s_33, s_34, s_35, s_36, s_37, s_38, s_39, s_40, s_41, s_42, s_43, s_44, s_45, s_46, s_47, s_48, s_49, s_50, s_51, s_52, s_53, s_54 };


int lunches[10];
int dinners[10];
int lunchIndex = 0;
int dinnerIndex = 0;

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
prog_char Body_3[] PROGMEM = "<b>Name: </b><select name=\"Name\"><br />";            
prog_char Body_4[] PROGMEM = "<input type=\"radio\" name=\"Meal\" value=\"Lunch\" />Lunch<br />";
prog_char Body_5[] PROGMEM = "<input type=\"radio\" name=\"Meal\" value=\"Dinner\" />Dinner<br />";
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
  
  
void clearMeals() {
  for (int i = 0; i < 10; i++) {
   lunches[i] = -1;
  dinners[i] = -1; 
  }
  lunchIndex = 0;
  dinnerIndex = 0;  
}
void setup() {
  clearMeals();
  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);
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
        //client.println(names[lunches[i]] + "<br />");
        strcpy_P(buffer, (char*)pgm_read_word(&(names[lunches[i]])));
        client.print(buffer);
        client.println("<br />");
      }
    }
    client.println("<b>Dinner requests:</b><br />");
    for (int i = 0; i < 10; i++) {
     if (dinners[i] != -1) {
       strcpy_P(buffer, (char *)pgm_read_word(&(names[dinners[i]])));
       client.print(buffer);
       client.println("<br />");
       //client.println(names[dinners[i]] + "<br />");
     } 
    }
    for(int i = 1; i < 4; i++) {                                     // Use a for loop to print the remaining lines
    strcpy_P(buffer, (char*)pgm_read_word(&(Body_table[i])));
    client.println( buffer );
    }
    
    // Dynamically add the list of names as a drop-down box
    for (int i = 0; i < NUM_NAMES; i++) {
//     client.println("<option value=" + i + ">" + names[i] + "</option>");
       strcpy_P(buffer, (char *)pgm_read_word(&(names[i])));
       client.print("<option value=\"");
       //client.print(names[i]);
       client.print(buffer);
       client.print("\">");
       //client.print(names[i]);
       client.print(buffer);
       client.println("</option>");
    }
    client.println("</select></br>");
    
    for(int i = 4; i < 7; i++) {                                     // Use a for loop to print the remaining lines
    strcpy_P(buffer, (char*)pgm_read_word(&(Body_table[i])));
    client.println( buffer );
    }
}

void addRequest(String name, String meal) {
  char buffer[30];
  if (name.equals("Clear_all_meals")) {
  clearMeals();
  return;
  }
  
  int indexOfName = -1;
 for (int i = 0; i < NUM_NAMES; i++) {
   strcpy_P(buffer, (char*)pgm_read_word(&(names[i])));
   if (name.equals(buffer)) {
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
  char buffer[20];
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    //Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
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
    delay(2);
      // close the connection:
      client.stop();
  }
  displayNames();
}

void displayNames() {
  lcd.clear();
  lcd.print("Lunches");
  delay(1000);
  for (int i = 0; i < lunchIndex; i++) {
    lcd.clear();
        strcpy_P(buffer, (char*)pgm_read_word(&(names[lunches[i]])));
    lcd.print(buffer);
    delay(1000);
  }
  lcd.clear();
  lcd.print("Dinners");
  delay(1000);
  for (int i = 0; i < dinnerIndex; i++) {
    lcd.clear();
    strcpy_P(buffer, (char*)pgm_read_word(&(names[dinners[i]])));
    lcd.print(buffer);
   delay(1000); 
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

