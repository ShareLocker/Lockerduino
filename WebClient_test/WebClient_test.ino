/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "192.168.1.2:8000/hubs/connected/2";    // name address for server
char root_addr[] = "192.168.1.2";
int server_port = 8000;
int secretkey = 2;
//IPAddress server_ip(192,168,1,2);

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);  // try to congifure using IP address instead of DHCP:
  }
  delay(1000);  // give the Ethernet shield a second to initialize:
}

void loop()
{

  char tail_addr[] = "/hubs/connected/";
  makeRequest(tail_addr);

  // do nothing forevermore:
  while (true);
  
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
  
  while (true) {
    if (client.available()) { // read incoming byes from server
      char c = client.read();
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
