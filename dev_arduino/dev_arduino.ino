#include <Dhcp.h>
#include <Dns.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

/* Arduino scrips for share lockers
adapted from the Arduino ethernet libraries example*/

// generic data for ethernet connection
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177); // backup, will attempt to use automatic IP address
EthernetClient client;

// data for the web server roles
EthernetServer server(80);  // create a server at port 80
String HTTP_req;          // stores the HTTP request

// data for the web client roles
char root_addr[] = "192.168.1.2";
int server_port = 8000;
int secretkey = 1;

// global dynamic data
int row = 1;
int col = 1;
int Nrow = 4;
int Ncol = 2;
unsigned int poll_freq = 5000;
int found_message = 0;
unsigned char ctr = 0;
unsigned char prevctr = 0;

void setup()
{
  Serial.begin(9600);
  delay(500);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure automatic IP adress");
    Ethernet.begin(mac, ip);  // try to congifure using IP address instead of DHCP:
  }
  delay(500);
  
  server.begin();           // start to listen for clients
  Serial.begin(9600);       // for diagnostics
  for (int thisPin = 2; thisPin < 8; thisPin++)  {
    pinMode(thisPin, OUTPUT);      
  }
  for (int thisPin = 2; thisPin < 8; thisPin++)  {
    digitalWrite(thisPin, HIGH);   
  }

  char tail_addr[] = "/hubs/connected/";
  makeRequest(tail_addr);
}

void loop()
{
  EthernetClient client = server.available();  // try to get client

  if (client) {  // got client?
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {   // client data available to read
        char c = client.read(); // read 1 byte (character) from client
        HTTP_req += c;  // save the HTTP request 1 char at a time
        // last line of client request is blank and ends with \n
        // respond to client only after last line received
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          // send web page
          client.println("<!DOCTYPE html>");
          client.println("<html>control is through URL parameters");
          client.println("</html>");
          Serial.println(HTTP_req);
          SwitchOn(client);
          HTTP_req = "";    // finished with request, empty string
          break;
        }
        // every line of text received from the client ends with \r\n
        if (c == '\n') {
          // last character on line of received text
          // starting new line with next character read
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // a text character was received from client
          currentLineIsBlank = false;
        }
      } // end if (client.available())
    } // end while (client.connected())
    delay(1);      // give the web browser time to receive the data
    client.stop(); // close the connection
    delay(3000);
    SwitchOff();
    // give signal that locker is closed again
    char tail_addr[] = "/hubs/finished/";
    makeRequest(tail_addr);
  } // end if (client)
  // code to regularily poll server, if we are doing that
  ctr = millis();
  if ((unsigned char)(ctr - prevctr) >= poll_freq) {
    prevctr = ctr;
    Serial.println(ctr);
    char tail_addr[] = "/hubs/poll/";
    makeRequest(tail_addr);
    if (found_message == 1) {
      found_message = 0;
      Serial.println("found message");
      JustSwitchOn();
      delay(3000); // method to open with polling
      SwitchOff();
    }
  }
}

void SwitchOn(EthernetClient cl)
{
  
    for (int thisPin = 2; thisPin < 8; thisPin++)  {
      digitalWrite(thisPin, HIGH);
    }
    int start_c = HTTP_req.indexOf("GET /?V=");
    int offset = 8;
    if (start_c > -1) {
      String val1 = HTTP_req.substring(start_c+offset,start_c+offset+1);
      String val2 = HTTP_req.substring(start_c+offset+1,start_c+offset+2);
      row = val2.toInt();
      col = val1.toInt();
      Serial.println(col);
      Serial.println(row);
      JustSwitchOn();
    }
}

void JustSwitchOn() {
  digitalWrite(1+row, LOW);
  digitalWrite(1+Nrow+col, LOW);
}

void SwitchOff() {
  digitalWrite(1+row, HIGH);
  digitalWrite(1+Nrow+col, HIGH);
}

void makeRequest(char* tail) {

  Serial.println("connecting...");

  if (client.connect(root_addr, server_port)) { // could use server_ip
    Serial.println("connected");
    client.print("GET ");  // ex. "GET /hubs/connected/2 HTTP/1.1"
    client.print(tail);
    client.print(secretkey);
    client.println(" HTTP/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }
  
  int start_stream = 0;
  while (true) {
    if (client.available()) { // read incoming byes from server
      char c = client.read();
      if (start_stream == 1) {
        col = c - '0'; // this convert char into into
        start_stream = 2;
      } else if (start_stream == 2) {
        row = c - '0';
        found_message = 1;
        start_stream = 3;
      } else if (c == '?') {
        start_stream = 1;
      }
      Serial.print(c);
    }

    if (!client.connected()) { // stop client if server disconnected
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      break;
    }
  }

}
