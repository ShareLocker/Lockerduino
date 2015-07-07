#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

/*--------------------------------------------------------------
  Program:      eth_websrv_LED

  Description:  Arduino web server that serves up a web page
                allowing the user to control an LED
  
  Hardware:     - Arduino Uno and official Arduino Ethernet
                  shield. Should work with other Arduinos and
                  compatible Ethernet shields.
                - LED and resistor in series connected between
                  Arduino pin 2 and GND
                
  Software:     Developed using Arduino 1.0.3 software
                Should be compatible with Arduino 1.0 +
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet

  Date:         11 January 2013
 
  Author:       W.A. Smith, http://startingelectronics.org
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);  // create a server at port 80

String HTTP_req;          // stores the HTTP request
//boolean LED_status = 0;   // state of LED, off by default
int last_pick = 0;

void setup()
{
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
    Serial.begin(9600);       // for diagnostics
    for (int thisPin = 2; thisPin < 8; thisPin++)  {
      pinMode(thisPin, OUTPUT);      
    }
    for (int thisPin = 2; thisPin < 8; thisPin++)  {
      digitalWrite(thisPin, HIGH);   
    }
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
                    client.println("<html><head>");
                    client.println("<title>Arduino LED Control</title>");
                    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /> ");
                    client.println("</head><body>");
                    client.println("<h1>Open Locker</h1>");
                    client.println("<form method=\"get\">");
                    client.println("<input type=\"submit\" name=\"V\" value=\"A1\" style=\"width:50px;height:50px\">");
                    client.println("<input type=\"submit\" name=\"V\" value=\"B1\" style=\"width:50px;height:50px\">");
                    client.println("<input type=\"submit\" name=\"V\" value=\"C1\" style=\"width:50px;height:50px\">");
                    client.println("</form><form method=\"get\">");
                    client.println("<input type=\"submit\" name=\"V\" value=\"A2\" style=\"width:50px;height:50px\">");
                    client.println("<input type=\"submit\" name=\"V\" value=\"B2\" style=\"width:50px;height:50px\">");
                    client.println("<input type=\"submit\" name=\"V\" value=\"C2\" style=\"width:50px;height:50px\">");
                    client.println("</form><form method=\"get\">");
                    client.println("<input type=\"submit\" name=\"V\" value=\"A3\" style=\"width:50px;height:50px\">");
                    client.println("<input type=\"submit\" name=\"V\" value=\"B3\" style=\"width:50px;height:50px\">");
                    client.println("<input type=\"submit\" name=\"V\" value=\"C3\" style=\"width:50px;height:50px\">");
                    client.println("</form></body></html>");
                    Serial.print(HTTP_req);
                    Serial.print(last_pick);
                    ProcessCheckbox(client);
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
    } // end if (client)
}

void ProcessCheckbox(EthernetClient cl)
{
  
    for (int thisPin = 2; thisPin < 8; thisPin++)  {
      digitalWrite(thisPin, HIGH);
    }
    if (HTTP_req.indexOf(" /?V=A1") > -1) { // contains A1
      if (last_pick < 1) {
        last_pick = 1;
        digitalWrite(2, LOW);
        digitalWrite(5, LOW);
        delay(3000);
        digitalWrite(2, HIGH);
        digitalWrite(5, HIGH);
      }
    } else if (HTTP_req.indexOf(" /?V=B1") > -1) {
        digitalWrite(2, LOW);
        digitalWrite(6, LOW);
        delay(3000);
        digitalWrite(2, HIGH);
        digitalWrite(6, HIGH);
    } else if (HTTP_req.indexOf(" /?V=C1") > -1) {
        digitalWrite(2, LOW);
        digitalWrite(7, LOW);
        delay(3000);
        digitalWrite(2, HIGH);
        digitalWrite(7, HIGH);
    } else if (HTTP_req.indexOf(" /?V=A2") > -1) { // next row
        digitalWrite(3, LOW);
        digitalWrite(5, LOW);
        delay(3000);
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
    } else if (HTTP_req.indexOf(" /?V=B2") > -1) {
        digitalWrite(3, LOW);
        digitalWrite(6, LOW);
        delay(3000);
        digitalWrite(3, HIGH);
        digitalWrite(6, HIGH);
    } else if (HTTP_req.indexOf(" /?V=C2") > -1) {
        digitalWrite(3, LOW);
        digitalWrite(7, LOW);
        delay(3000);
        digitalWrite(3, HIGH);
        digitalWrite(7, HIGH);
    } else if (HTTP_req.indexOf(" /?V=A3") > -1) { // next row
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        delay(3000);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
    } else if (HTTP_req.indexOf(" /?V=B3") > -1) {
        digitalWrite(4, LOW);
        digitalWrite(6, LOW);
        delay(3000);
        digitalWrite(4, HIGH);
        digitalWrite(6, HIGH);
    } else if (HTTP_req.indexOf(" /?V=C3") > -1) {
        digitalWrite(4, LOW);
        digitalWrite(7, LOW);
        delay(3000);
        digitalWrite(4, HIGH);
        digitalWrite(7, HIGH);
    } else {
      last_pick = 0;
    }
    
 //   if (LED_status) {    // switch LED on
 //       digitalWrite(2, HIGH);
 //   }
 //   else {              // switch LED off
 //       digitalWrite(2, LOW);
 //   }
}
