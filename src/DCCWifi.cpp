#include <Arduino.h>

#ifndef FILESTRUCT_H 
    #include "filestruct.h"
#endif

#include "DCCWifi.h"
#include "stdio.h"
#include "display.h"
#include "roster.h"


char ssid[] = "DCCEX_c25024";          //  your network SSID (name)
char pass[] = "Monarch1466";   // your network password
char host[] = "192.168.4.1";
int port = 2560;

char WiFiCommand[15];

//int status = 0;
bool wiFiDisconnected = true;
WiFiClient DCCclient;

// Current state of WiFi (connected, idling)
int status = WL_IDLE_STATUS;

// WiFi specific defines
#define WIFITIMEOUTSECONDS 20


void WiFiSetup() {

  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  screenmessage("Attempting to connect to DCC network...");

  WiFi.mode(WIFI_STA);

  // dont allow WiFitosleep
  WiFi.setSleep(false);

  WiFi.begin(ssid, pass);

  // try a few times to connect
  int timeout = WIFITIMEOUTSECONDS * 4;

  Serial.print("WiFi Status ");
  Serial.println(WiFi.status());

  while (WiFi.status() != WL_CONNECTED && (timeout-- > 0))
  //while (WiFi.status() != WL_CONNECTED)
  {
      // delay(500);
      // WiFi.reconnect();
      delay(100);
      Serial.print(".");
  }


  //Succesful connection?

  wl_status_t wifiStatus = WiFi.status();
  if (wifiStatus != WL_CONNECTED)
  {
      Serial.println("/nFailed to connect, exiting.");
      screenmessage("/nFailed to connect, exiting.");
      //Serial.printf("WiFi Status: %s\n", wl_status_to_string(wifiStatus));
      return;
  }  

  Serial.printf("WiFi connected with (local) IP address of: %s\n", WiFi.localIP().toString().c_str());
	wiFiDisconnected = false;

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
  screenmessage("WiFi connected");

  Serial.print("Now connecting to DCC++EX Server : ");
  Serial.println(host);

  //WiFiClient DCCclient;

  Serial.println("Connected to wifi");
  Serial.println("\nStarting connection...");
  // if you get a connection, report back via serial:
  if (DCCclient.connect("192.168.4.1", 2560)) {
    Serial.println("connected");
    
  } 
      

}



void DoDCCThrottle(int Channel) {


  Serial.printf("<t 1 %i %i %i>\n", LocoAddress[Channel], LocoSpeed[Channel], LocoDirection[Channel]);

  DCCclient.printf("<t 1 %i %i %i>\n", LocoAddress[Channel], LocoSpeed[Channel], LocoDirection[Channel]);

}

void DoDCCFunction(int fx, int onof){

  Serial.printf("<F %i %i %i>\n", LocoAddress[CurrentChannel], fx, onof);

  DCCclient.printf("<F %i %i %i>\n", LocoAddress[CurrentChannel], fx, onof);

}

bool GetTheRoster() {

  long startTime = millis();
  readString = "";

  while (!DCCclient.available()){
    if ((millis() - startTime) > 10000){
        return false;
    }
    else {
      delay(50);
    }
  }
  int LocoSub = 0;
  while (DCCclient.available()) {
        readString = DCCclient.readStringUntil('\r');
        Serial.print(readString);
        ParseRoster(); 

        //int x =LOCONAME.length;
        LOCOS[LocoSub].address = ADDRESS;
        //LOCOS[LocoSub].description = LOCONAME.toCharArray[1, 18];
        //LOCONAME.toCharArray[LOCOS[LocoSub].description, 18];
        //strcpy(LOCOS[LocoSub].description, LOCONAME);
        LOCOS[LocoSub].description = LOCONAME.substring(1,18);

        LocoSub = LocoSub + 1;
        readString = "";
    }
  
  return true;
  
}

bool GetTheFKeys() {

long startTime = millis();
 
  readString = "";

  while (!DCCclient.available()){
    if ((millis() - startTime) > 10000){
        return false;
    }
    else {
      delay(50);
    }
  }
  while (DCCclient.available()) {
        readString = DCCclient.readStringUntil('\r');
        Serial.print(readString);
        
    }
  
  return true;

}

void DoDCCRoster(){

// If you send <J>  I will reply 
// <j cabid "name"> for each loco in the roster. 
  Serial.print("<J>\n");

  DCCclient.print("<J>\n");  // send the command to request the roster.


}

void DoDCCFunctionKeys(int Channel){

  //int locoaddress = 7309;
  int locoaddress = LocoAddress[Channel];

  // Send the command for the selected loco
  //Serial.printf("<J %i>\n"), LocoAddress[Channel];
  Serial.printf("<J %i>\n", locoaddress);

  //DCCclient.printf("<J %i>\n"), LocoAddress[Channel];
  DCCclient.printf("<J %i>\n", locoaddress);

  

}




void DoDCCPower(int onof){

  Serial.print("<");
  Serial.print(onof);
  Serial.println(">");

  // DCCclient.write("<");
  // DCCclient.print(onof);
  // DCCclient.print(">");
}