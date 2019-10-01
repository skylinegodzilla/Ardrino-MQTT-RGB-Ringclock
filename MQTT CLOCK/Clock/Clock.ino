/*======================================================
           IOT RGBW LED RING CLOCK
           by Ben Cawley
                 Running on NodeMCU
                 Powered by Ardrino C++
                 Using ESP8266WiFi/Adafruit Mqtt Client/Adafruit NeoPixel
  ======================================================*/
//TODO:
  //Write the MQTT Interface Client(Web or phone app) -Separate project
  //Rainbow Mode
  //Add digital thermometer (DHT)
  //Clean up MQTT Directory's
  //Add google assistance support -Separate project
  //Spotify music sync using Echo Nest data / spotify-viz https://github.com/zachwinter/spotify-viz
  //Generalise the modules for easy use in other future IOT home automation projects
/*===============================
            Headers
===============================*/
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <time.h>
#include <cmath>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "RGBW.h"
#include "Utilities.h"
#include "DisplayManager.h"
#include "Network.h"
#include "TimeManager.h"

#include "ClockData.h"
#include "MQTTManager.h"
/*===============================
            Globals
===============================*/

const char* ssid     = "XXXXXXXX"; //Router Name
const char* password = "XXXXXXXX"; //Router Password
IPAddress ip(192, 168, 1, 50);  //Sett a static ip on the network for the webserver
IPAddress gateway(192, 168, 1, 1);


int minPin = 5;
int hourPin = 4;

//Adafruit_NeoPixel secStrip = Adafruit_NeoPixel(60, secPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minStrip = Adafruit_NeoPixel(60, minPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel hourStrip = Adafruit_NeoPixel(16, hourPin, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel hourStrip = Adafruit_NeoPixel(60, hourPin, NEO_GRB + NEO_KHZ800);

ClockData ClkDta;
ClockData* ClkDtaPtr = &ClkDta;

//Utilities utl;


DisplayManager DisplayMan;
DisplayManager* DisplayManPtr = &DisplayMan;

Network WifiConnection(ssid,password,ip,gateway);

MQTTManager MQTTMan(&ClkDta);

TimeManager TimeMan;
int timeZone = 12; //time zone +12GMT
struct tm* timeDataPtr;


/*===============================
            Setup
===============================*/
void setup() {
  ets_intr_lock(); // all interrupts off

  Serial.begin(115200);
  Serial.println("Setup");

  //Serial.println("Starting LED's");
  //hourStrip.begin();
  //minStrip.begin();

  Serial.println("--Wifi Conect--");
  WifiConnection.networkConnect();

  Serial.println("--Time Grab from internet--");
  TimeMan.requestTime(12);

  Serial.println("--MQTT Client Start up--");
  MQTTMan.MQTTManagerStart();


//Leds setup
  ClkDtaPtr->displayMode = 2;
  Serial.println("--Prep Hour Led Strip--");

  ClkDtaPtr->hourColor.setRGBW(5, 3, 1, 0);
  hourStrip.show();
  Serial.println("--Prep Minuit/Second Led Strip--");
  ClkDtaPtr->minColor.setRGBW(0, 1, 3, 0);
  ClkDtaPtr->secColor.setRGBW(0, 10, 25, 0);
  ClkDtaPtr->incramentColor.setRGBW(5, 3, 1, 0);
  ClkDtaPtr->incrament2Color.setRGBW(5, 4, 2, 0);
  minStrip.show();
}

/*===============================
           Main Loop
===============================*/
void loop() {
  Serial.println("LOOP");
  Serial.println("--TimeUpdate--");
  timeDataPtr = TimeMan.upDate();
  Serial.println("--MQTTUpdate--");
  MQTTMan.upDate();
  Serial.println("--DisplayUpdate--");
  DisplayManPtr->upDate();


  //displayTime();

}

void displayTime(){
    //Just some test code
  Serial.print("time:");
  Serial.print(timeDataPtr->tm_hour);
  Serial.print(":");
  Serial.print(timeDataPtr->tm_min);
  Serial.print(":");
  Serial.print(timeDataPtr->tm_sec);
  Serial.println("");
  //end of test code
}
