#include "MQTTManager.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//This is the MQTT client it handles the communications with the MQTT server and if something changes it updates the Clock Data accordingly.

//TODO: Figure out the WiFi.Status values to send error codes out for error connections other then timing out
// Define A sub  list and a pub list up here to hold all the channels

/*===============================
            Globals
===============================*/
#define DeviceName      "ClockNodeMcu"
#define MQTTServer      "192.168.1.2"
#define MQTTPort  1883  // use 8883 for SSL
#define MQTTUserName    "MQTT USER NAME"
#define MQTTPW          "MQTT USER PASSWORD"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTTServer, MQTTPort, MQTTUserName, MQTTPW);

/*===============================
            Feeds
===============================*/
//========== Publish ==========
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish chPubConnectTest = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Devices/" DeviceName);
Adafruit_MQTT_Publish chPubExample = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Devices/" DeviceName "/PubExample");

Adafruit_MQTT_Publish chPubClkSleep = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Lights/RGB/Clock/Sleep");
Adafruit_MQTT_Publish chPubClkHourColour = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Lights/RGB/Clock/HourColour");
Adafruit_MQTT_Publish chPubClkMinColour = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Lights/RGB/Clock/MinColour");
Adafruit_MQTT_Publish chPubClkSecColour = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Lights/RGB/Clock/SecColour");
Adafruit_MQTT_Publish chPubClkHourBright = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Lights/RGB/Clock/HourBright");
Adafruit_MQTT_Publish chPubClkMinBright = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Lights/RGB/Clock/MinBright");
Adafruit_MQTT_Publish chPubClkSecBright = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Lights/RGB/Clock/SecBright");
Adafruit_MQTT_Publish chPubClkDisplayMode = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Lights/RGB/Clock/DisplayMode");
Adafruit_MQTT_Publish chPubClkRainbowSpeed = Adafruit_MQTT_Publish(&mqtt, MQTTUserName "/Lights/RGB/Clock/RainbowSpeed");

//========== Subscribe ==========
// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe chSubClkSleep = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/Sleep");
Adafruit_MQTT_Subscribe chSubClkHourColour = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/HourColour");
Adafruit_MQTT_Subscribe chSubClkMinColour = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/MinColour");
Adafruit_MQTT_Subscribe chSubClkSecColour = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/SecColour");
Adafruit_MQTT_Subscribe chSubClkBackColour = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/BackColour");
Adafruit_MQTT_Subscribe chSubClkIntColour = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/IntColour");
Adafruit_MQTT_Subscribe chSubClkInt2Colour = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/Int2Colour");

Adafruit_MQTT_Subscribe chSubClkHourBright = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/HourBright");
Adafruit_MQTT_Subscribe chSubClkMinBright = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/MinBright");
Adafruit_MQTT_Subscribe chSubClkBackBright = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/BackBright");

Adafruit_MQTT_Subscribe chSubClkDisplayMode = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/DisplayMode");
Adafruit_MQTT_Subscribe chSubClkRainbowSpeed = Adafruit_MQTT_Subscribe(&mqtt, MQTTUserName "/Lights/RGB/Clock/RainbowSpeed");

/*===============================
            Constructor
===============================*/
MQTTManager::MQTTManager(ClockData* clkDtrPtrPrm){
  clkDtrPtr = clkDtrPtrPrm;
}




/*===============================
            Start
===============================*/
void MQTTManager::MQTTManagerStart(void){
  // Setup MQTT subscriptions
  mqtt.subscribe(&chSubClkSleep);
  mqtt.subscribe(&chSubClkHourColour);
  mqtt.subscribe(&chSubClkMinColour);
  mqtt.subscribe(&chSubClkSecColour);
  mqtt.subscribe(&chSubClkBackColour);
  mqtt.subscribe(&chSubClkIntColour);
  mqtt.subscribe(&chSubClkInt2Colour);

  mqtt.subscribe(&chSubClkHourBright);
  mqtt.subscribe(&chSubClkMinBright);
  mqtt.subscribe(&chSubClkBackBright);

  mqtt.subscribe(&chSubClkDisplayMode);
  mqtt.subscribe(&chSubClkRainbowSpeed);

  this->MQTT_connect();

  Serial.print(F("\nSending TestConnect Value ")); //noiidear what F is but print line saying sending rgb value
  Serial.print("ClockNodeMCU");  // then print the value we are going to send
  Serial.print("...");
  if (! chPubConnectTest.publish("Yes!")) { ///if we cant publish the Adafruit_MQTT_Publish
    Serial.println(F("Failed")); //then say we failed the publish
  } else {
    Serial.println(F("OK!")); //else say ok to indacate that we sucessfley published the value
  }
}



void MQTTManager::upDate(void) {
  Serial.println("atoi Test");
  Serial.println(atoi("onhhj86hgjkkje"));

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  this->MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here


  //Subscriber
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20))) { //while the client is reading the scubscription //Safe is 20

    //ClockSleep
    if (subscription == &chSubClkSleep) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chPubClkSleep");
      Serial.println((char *)&chSubClkSleep.lastread);
      int tempSleepValue = atoi((char *)chSubClkSleep.lastread);
      if (tempSleepValue >1 ||  tempSleepValue < 0){tempSleepValue = 0;}
      clkDtrPtr-> powerState = tempSleepValue;
    }
    //TODO:
    //HourColour  //Rember that the data needs to be fedd in as "RRR,GGG,BBB,WWW" where RR is tthe red value in a int_16 value and GGG is Green, BBB is blue and WWW is white
    if (subscription == &chSubClkHourColour) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe{
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkHourColour");
      Serial.println((char *)&chSubClkHourColour.lastread);

      std::vector<String> hourColourValues = Utilities::splitStringToVector((char *)&chSubClkHourColour.lastread);
      clkDtrPtr->hourColor.red = hourColourValues[0].toInt();
      clkDtrPtr->hourColor.green = hourColourValues[1].toInt();
      clkDtrPtr->hourColor.blue = hourColourValues[2].toInt();
      clkDtrPtr->hourColor.white = hourColourValues[3].toInt();
      Serial.println("hourData Values from the RGBW data");
    }



    //MinColour
    if (subscription == &chSubClkMinColour) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe{
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkMinColour");
      Serial.println((char *)&chSubClkMinColour.lastread);

      std::vector<String> minColourValues = Utilities::splitStringToVector((char *)&chSubClkMinColour.lastread);
      clkDtrPtr->minColor.red = minColourValues[0].toInt();
      clkDtrPtr->minColor.green = minColourValues[1].toInt();
      clkDtrPtr->minColor.blue = minColourValues[2].toInt();
      clkDtrPtr->minColor.white = minColourValues[3].toInt();
      Serial.println("minData Values from the RGBW data");
    }

    //SecColour
    if (subscription == &chSubClkSecColour) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe{
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkSecColour");
      Serial.println((char *)&chSubClkSecColour.lastread);

      std::vector<String> secColourValues = Utilities::splitStringToVector((char *)&chSubClkSecColour.lastread);
      clkDtrPtr->secColor.red = secColourValues[0].toInt();
      clkDtrPtr->secColor.green = secColourValues[1].toInt();
      clkDtrPtr->secColor.blue = secColourValues[2].toInt();
      clkDtrPtr->secColor.white = secColourValues[3].toInt();
      Serial.println("secData Values from the RGBW data");
    }

    //BackColour
    if (subscription == &chSubClkBackColour) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe{
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkBackColour");
      Serial.println((char *)&chSubClkBackColour.lastread);

      std::vector<String> backColourValues = Utilities::splitStringToVector((char *)&chSubClkBackColour.lastread);
      clkDtrPtr->backColor.red = backColourValues[0].toInt();
      clkDtrPtr->backColor.green = backColourValues[1].toInt();
      clkDtrPtr->backColor.blue = backColourValues[2].toInt();
      clkDtrPtr->backColor.white = backColourValues[3].toInt();
      Serial.println("chSubClkBackColour Values from the RGBW data");
    }

     //IntColour
    if (subscription == &chSubClkIntColour) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe{
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkIntColour");
      Serial.println((char *)&chSubClkIntColour.lastread);

      std::vector<String> intColourValues = Utilities::splitStringToVector((char *)&chSubClkIntColour.lastread);
      clkDtrPtr->incramentColor.red = intColourValues[0].toInt();
      clkDtrPtr->incramentColor.green = intColourValues[1].toInt();
      clkDtrPtr->incramentColor.blue = intColourValues[2].toInt();
      clkDtrPtr->incramentColor.white = intColourValues[3].toInt();
      Serial.println("chSubClkIntColour Values from the RGBW data");
    }

     //IntColour2
    if (subscription == &chSubClkInt2Colour) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe{
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkInt2Colour");
      Serial.println((char *)&chSubClkInt2Colour.lastread);

      std::vector<String> intColour2Values = Utilities::splitStringToVector((char *)&chSubClkInt2Colour.lastread);
      clkDtrPtr->incrament2Color.red = intColour2Values[0].toInt();
      clkDtrPtr->incrament2Color.green = intColour2Values[1].toInt();
      clkDtrPtr->incrament2Color.blue = intColour2Values[2].toInt();
      clkDtrPtr->incrament2Color.white = intColour2Values[3].toInt();
      Serial.println("chSubClkInt2Colour Values from the RGBW data");
    }

    //HourBright
    if (subscription == &chSubClkHourBright) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkHourBright");
      Serial.println((char *)&chSubClkHourBright.lastread);
      int tempHBValue = atoi((char *)chSubClkHourBright.lastread);
      if (tempHBValue >255 ||  tempHBValue < 0){tempHBValue = 0;}
      clkDtrPtr-> hourBright = tempHBValue;
    }

    //MinBright
    if (subscription == &chSubClkMinBright) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkMinBright");
      Serial.println((char *)&chSubClkMinBright.lastread);
      int tempMBValue = atoi((char *)chSubClkMinBright.lastread);
      if (tempMBValue >255 ||  tempMBValue < 0){tempMBValue = 0;}
      clkDtrPtr-> minBright = tempMBValue;
    }

    //BackBright
    if (subscription == &chSubClkBackBright) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkBackBright");
      Serial.println((char *)&chSubClkBackBright.lastread);
      int tempBBValue = atoi((char *)chSubClkBackBright.lastread);
      if (tempBBValue >255 ||  tempBBValue < 0){tempBBValue = 0;}
      clkDtrPtr-> backBright = tempBBValue;
    }


  //displayMode
  if (subscription == &chSubClkDisplayMode) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkDisplayMode");
      Serial.println((char *)&chSubClkDisplayMode.lastread);
      int tempDMValue = atoi((char *)chSubClkDisplayMode.lastread);
      if (tempDMValue >255 ||  tempDMValue < 0){tempDMValue = 0;}
      clkDtrPtr-> displayMode = tempDMValue;
    }

  //rainbowSpeed
  if (subscription == &chSubClkRainbowSpeed) { //if the current subscription is the same as the the Adafruit_MQTT_Subscribe
      Serial.println("---");
      Serial.print(F("Got: ")); //print got and then the Adafruit_MQTT_Subscribe.lastread witch is the last change in the Adafruit_MQTT_Subscribe
      Serial.println((char *)"chSubClkRainbowSpeed");
      Serial.println((char *)&chSubClkRainbowSpeed.lastread);
      int tempRBSValue = atoi((char *)chSubClkRainbowSpeed.lastread);
      if (tempRBSValue >255 ||  tempRBSValue < 0){tempRBSValue = 0;}
      clkDtrPtr-> rainbowSpeed = tempRBSValue;
    }
 }

  //publisher
  /*
    The Clock Should not need to publish any information but for other projects I've
    put in an example below of a publish with error catching
  */
  if (! chPubExample.publish(0)) { ///if we cant publish the Adafruit_MQTT_Publish
    Serial.println(F("Failed")); //then say we failed the publish
  } else {
    Serial.println(F("OK!")); //else say ok to indicate that we successfully published the value
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }

  */
}



/*===============================
          MQTT_connect()
===============================*/
void MQTTManager::MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  //TODO: Add in here a error code for clock data

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       DisplayManPtr->upDate(); //I know this is bad design putting this here but I want to have animated loading lights. if it becomes a problem I will remove it.
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
