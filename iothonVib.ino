/*
  Web client

  This sketch connects to a website through a MKR NB 1500 board. Specifically,
  this example downloads the URL "http://example.org/" and
  prints it to the Serial monitor.

  Circuit:
   - MKR NB 1500 board
   - Antenna
   - SIM card with a data plan

  created 8 Mar 2012
  by Tom Igoe
*/

// libraries
#include <MKRNB.h>

#include "arduino_secrets.h"

String readLine(){

  String s = "";
  while (true) {
    // Wait for input
    while(Serial.available() == 0)
      delay(1);
    
    char c = Serial.read();
    if(c == '\n') return s;
    s += c;
  }
  return s;
}

int vibPins[] = {0, 1, 2, 3, 4, 5, 6, 7};
int numVibs = 8;

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[]     = SECRET_PINNUMBER;

// initialize the library instance
GPRS gprs;
NB nbAccess;
char url[] = "0.tcp.eu.ngrok.io";
int port = 17426;

void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  delay(500);

  Serial.setTimeout(60000);

  Serial.println("Starting Arduino web client.");

  for(int i = 0; i < numVibs; i++){
    pinMode(vibPins[i], OUTPUT);
    digitalWrite(vibPins[i], 0);
  }

  // connection state
  boolean connected = false;
  // After starting the modem with NB.begin()
  // attach to the GPRS network with the APN, login and password
  while (!connected) {
    if ((nbAccess.begin(PINNUMBER) == NB_READY) &&
        (gprs.attachGPRS() == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  
  Serial.print("Server is set to: ");
  Serial.println(url);
  Serial.print("Port is set to: ");
  Serial.println(port);

  Serial.setTimeout(1000);
}

void loop() {

  NBClient client;
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(url, port)) {
    Serial.println("connected");
    
    for(int i = 0; i < numVibs; i++)
      digitalWrite(vibPins[i], 1);
    delay(200);
    for(int i = 0; i < numVibs; i++)
      digitalWrite(vibPins[i], 0);
    delay(200);
    for(int i = 0; i < numVibs; i++)
      digitalWrite(vibPins[i], 1);
    delay(200);
    for(int i = 0; i < numVibs; i++)
      digitalWrite(vibPins[i], 0);
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    return;
  }

  while(client.connected()){

    Serial.print("Setting state ");
    for(int i = 0; i < numVibs; i++){
      int c = client.read();
      while(c == -1){
        delay(1);
        if(!client.connected()) break;
        c = client.read();
      }

      int val = c ? 1 : 0;
      digitalWrite(vibPins[i], val);
      Serial.print(val);
    }

    Serial.println();
    delay(1);
  }

  for(int i = 0; i < numVibs; i++){
    digitalWrite(vibPins[i], 0);
  }
  
  // if the server's disconnected, stop the client:
  Serial.println();
  Serial.println("disconnecting.");
  client.stop();

  Serial.println("DEAD");

}
