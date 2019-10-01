#include "Network.h"
//TODO: Figure out the WiFi.Status values to send error codes out for error connections other then timing out

//This module is used to find and connect to the local Wi-Fi

Network::Network(const char* ssidPrm, const char* pwPrm, IPAddress ipPrm, IPAddress gatewayPrm)
{
  ssid= ssidPrm;
  pw = pwPrm;
  ip = ipPrm;
  gateway = gatewayPrm;
}

void Network::networkConnectSuccess(void){
  /*================================
  * Connection a Success
  =================================*/
  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void Network::networkConnectFail(int error){
     Serial.println("Connection Error");
     switch(error){
     case 1 : Serial.println("Connection Timed Out");
     break;
     default: Serial.println("Connection Failed with errorcode: ");
     Serial.print(error);
     Serial.println("");
     break;
  }
}
void Network::networkConnect(void){

  /*===================================
    * Set up the Connection
  ===================================*/
  int timeout = 60;
  int connectError = 0;
  int i = 0;

  IPAddress dummyDNS(8, 8, 8, 8); //Googles DNS just cause
  // Set up and conect to wifi
  WiFi.config(ip, dummyDNS, gateway);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  Serial.println("With ip address: ");
  Serial.println(ip);

  //Conect to wifi comunnacation
  WiFi.begin(ssid, pw);
  delay(500);

  while (WiFi.status() != WL_CONNECTED && i <= timeout){
    delay(20);
    Serial.print(".");
    DisplayManPtr->upDate(); //I know this is bad design putting this here but I want to have animated loading lights
    i++;

  }
  if(WiFi.status() == WL_CONNECTED){Serial.println("CONNECTED!!!");}
  if(i >= timeout){connectError=1;}

  if(connectError == 0){networkConnectSuccess();}else{networkConnectFail(connectError);}
}

void Network::upDate(void){}
