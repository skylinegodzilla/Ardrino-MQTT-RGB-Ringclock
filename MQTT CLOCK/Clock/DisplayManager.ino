//The Display Manager is a module that is used to control the display of the clock face of the device.
//It works by checking the values stored in the clock data object and then calling the corresponding functions.

//TODO:
  //move m and h out of there individual scopes and set them up as global Displaymanager values so that they use less resources
  //Compress all the minLEDList[i].colourRGBW.red = ... minLEDList[i].colourRGBW.blue = ... minLEDList[i].colourRGBW.green = ... into a single function
  //Fix the New Minuit leds in rainbow mode
  //Add seconds to rainbow mode
  //Add Hours to rainbow mode

bool rbSet = false;

DisplayManager::DisplayManager(){
    //The constructor
    hourStrip.begin();
    minStrip.begin();

    newMin = false;
    newSec = false;
    newHour = false;

    hpv = 1; //Head Pulse Value A value to divide the RGB values by to make them fade in and out.
    hpvUp = true;

    tempDisplayMode = 0; //a kind of flag used to see if the display mode has changed and may need to do some clean up of the old display mode

    //Creating the led objects and putting them in there lists
    //hours
    for (int iLED = 0; iLED < 16; iLED++)
    {
      hourLEDList[iLED] = LED(iLED);
    }

    //min
    for (int iLED = 0; iLED < 60; iLED++)
    {
      minLEDList[iLED] = LED(iLED);
    }


  }
// colourWheel (number between 0-255) Returns a RGBW value
RGBW DisplayManager::colourWheel(int wheelPos) {
  static int c[3];

  if(wheelPos < 85) {
   c[0] = wheelPos * 3; //Green
   c[1] = 255 - wheelPos * 3; //Red
   c[2] = 0; //Blue
  } else if(wheelPos < 170) {
   wheelPos -= 85;
   c[0] = 255 - wheelPos * 3;
   c[1] = 0;
   c[2] = wheelPos * 3;
  } else {
   wheelPos -= 170;
   c[0] = 0;
   c[1] = wheelPos * 3;
   c[2] = 255 - wheelPos * 3;
  }
  RGBW cRGBW(*(c+1), *c, *(c+2), 0);

  return cRGBW;
}
/*===============================
           UPDATE
===============================*/

void DisplayManager::upDate(void){
  switch(ClkDtaPtr->displayMode){
    case 0 : Serial.println("display mode 0 : Error");
             //Run Error update function
             break;
    case 1 : Serial.println("display mode 1 : Loading");
             upDateLoading();
             break;
    case 2 : Serial.println("display mode 2 : Static");
             upDateStatic();
             break;
    case 3 : Serial.println("display mode 3 : Rainbow");
             upDateRainbow();
             break;
    case 4 : Serial.println("display mode 4 : Comet");
             //Run comet update function
             break;
  }


  for (int i = 0; i < 60; i++){
    minStrip.setPixelColor(i,minLEDList[i].colourRGBW.red,minLEDList[i].colourRGBW.green,minLEDList[i].colourRGBW.blue);
  }
  for (int i = 0; i < 16; i++){
   hourStrip.setPixelColor(i,hourLEDList[i].colourRGBW.red,hourLEDList[i].colourRGBW.green,hourLEDList[i].colourRGBW.blue);
  }

  int s = timeDataPtr->tm_sec;
  if (s = 0){newSec = true;}else{newSec = false;}

  if(tempDisplayMode != ClkDtaPtr->displayMode){

    minStrip.clear();
    hourStrip.clear();

    rbSet = false;

    for (LED x : minLEDList){
      x.colourRGBW.red = 0;
      x.colourRGBW.green = 0;
      x.colourRGBW.blue = 0;
    }
    for (LED x : hourLEDList){
      x.colourRGBW.red = 0;
      x.colourRGBW.green = 0;
      x.colourRGBW.blue = 0;
    }
  }

  tempDisplayMode = ClkDtaPtr->displayMode; //resets the flag

  minStrip.show();
  hourStrip.show();
}



int DisplayManager::hpvValue(int oldhpv, int maxhpv)
{
  if (hpvUp){
    oldhpv++;
    if (oldhpv >= maxhpv){
     oldhpv = maxhpv;
     hpvUp = false;
    }
  }else{
    oldhpv--;
    if (oldhpv <= 0){
      oldhpv = 1;
      hpvUp = true;
    }
  }
 return oldhpv;
}

/*===============================
            STATIC
static was written b4 I created the
LED class so it is not using it.
===============================*/

void DisplayManager::upDateStatic(void){
  //Some pre loop calulations
  bool hourLedis2 = false;
  hpv = hpvValue(hpv,25); //the head fade value

  //-----Hour---
  int h = timeDataPtr->tm_hour;
  h = Utilities::to12HourFormat(h);
  if (h == 0 &&  newHour == false){
    newHour = true;
    //Clear the hour leds;
    hourStrip.clear();
  }
  if (h > 0){
    newHour = false;
  }

  //Draw Hour
  int hLength = -1;
  std::vector<String> hLengthStringVect = Utilities::splitStringToVector((String)hourToLedMap[h]); //Create a vector store the values stored at hourToLedMap[h] in it while splitting the values.
                                                                                                    //AKA if h was 0 the array hour to led map has "12,0" there so it will store 12 and 0 as strings in
  int hLengthStringVectLength = 0;                                                                                                  //the new vecter you just created.
  for (String hLengthString:hLengthStringVect)
    {hLength = (hLength < hLengthString.toInt()) ? hLengthString.toInt():hLength;                //Compear the value(after converting it from a string to a int) with the value in hLength.
                                                                                                  //store the largest number in hLength. then repeat again till there's no values left in the vector array
    hLengthStringVectLength ++;
    }
  for(int i=0; i<hLength; i++) {
    hourLEDList[i].colourRGBW = ClkDtaPtr->hourColor;
    hourStrip.setBrightness(ClkDtaPtr->hourBright);
  }
  //The hour incraments
  int incrmentLeds[8] = {15,0,3,4,7,8,11,12};
  //int incrmentLeds[8] = {16,1,4,5,8,9,12,13};
  for ( int x : incrmentLeds){
    hourLEDList[x].colourRGBW = ClkDtaPtr->incrament2Color;
  }
  hourLEDList[hLength-1].colourRGBW = ClkDtaPtr->hourColor;
  //Draw Background (on hour ring)
  //Draw hour Head
  hourLEDList[hLength-1].colourRGBW.red = round(ClkDtaPtr->hourColor.red/hpv);
  hourLEDList[hLength-1].colourRGBW.green = round(ClkDtaPtr->hourColor.green/hpv);
  hourLEDList[hLength-1].colourRGBW.blue = round(ClkDtaPtr->hourColor.blue/hpv);
  if(hLengthStringVectLength == 2){
    hourLEDList[hLength-2].colourRGBW.red = round(ClkDtaPtr->hourColor.red/hpv);
    hourLEDList[hLength-2].colourRGBW.green = round(ClkDtaPtr->hourColor.green/hpv);
    hourLEDList[hLength-2].colourRGBW.blue = round(ClkDtaPtr->hourColor.blue/hpv);
  }

  //-----Minuit---
  int m = timeDataPtr->tm_min;
  if (m == 0 &&  newMin == false){
    newMin = true;
    //Clear the minuit leds;
    minStrip.clear();
  }
  if (m > 0){
    newMin = false;
  }
  //Draw Minuits
  for(int i = 0; i < m; i++) {
    minLEDList[i].colourRGBW = ClkDtaPtr->minColor;
    minStrip.setBrightness(ClkDtaPtr->minBright);
  }
  //Draw Background
  for(int i = m; i < 60; i++){
    minLEDList[i].colourRGBW = ClkDtaPtr->backColor;
  }
  //Draw Interviles
  for(int i = -1; i < 60; i+=5){
    minLEDList[i].colourRGBW = ClkDtaPtr->incramentColor;
  }
  //Draw min Head
  minLEDList[m-1].colourRGBW.red = round(ClkDtaPtr->minColor.red/hpv);
  minLEDList[m-1].colourRGBW.green = round(ClkDtaPtr->minColor.green/hpv);
  minLEDList[m-1].colourRGBW.blue = round(ClkDtaPtr->minColor.blue/hpv);
  //-----Second---
  int s = timeDataPtr->tm_sec;
  minLEDList[s].colourRGBW = ClkDtaPtr->secColor;
}

/*===============================
           RAINBOW
===============================*/
int offSet = 50;
void DisplayManager::upDateRainbow(void){
  //-----Minuit---
  int m = timeDataPtr->tm_min;

  // Setup M
  if (rbSet == false){
    rbSet = true;
    for(int i = 0; i < m; i++) {
      minLEDList[i].setHueVal(i*3 + offSet);
      minLEDList[i].colourRGBW.red = colourWheel(minLEDList[i].hueVal).red/2;
      minLEDList[i].colourRGBW.green = colourWheel(minLEDList[i].hueVal).green/2;
      minLEDList[i].colourRGBW.blue = colourWheel(minLEDList[i].hueVal).blue/2;
    }
  }
  //update M
  for(int i = 0; i < m; i++) {
    minLEDList[i].setHueVal(minLEDList[i].hueVal + 1);
    minLEDList[i].colourRGBW.red = colourWheel(minLEDList[i].hueVal).red/2;
    minLEDList[i].colourRGBW.green = colourWheel(minLEDList[i].hueVal).green/2;
    minLEDList[i].colourRGBW.blue = colourWheel(minLEDList[i].hueVal).blue/2;
  }

  //if newSec = true (the secionds have compleated a circit, then set the new minuit led values
  if(newSec && m != 0){
    minLEDList[m].setHueVal(minLEDList[m-1].hueVal + 1);
    minLEDList[m].colourRGBW.red = colourWheel(minLEDList[m - 1].hueVal).red/2;
    minLEDList[m].colourRGBW.green = colourWheel(minLEDList[m - 1].hueVal).green/2;
    minLEDList[m].colourRGBW.blue = colourWheel(minLEDList[m - 1].hueVal).blue/2;
  }
  //Clear unused minuits leds
  for(int i = m; i < 60; i++) {
    minLEDList[i].colourRGBW.red = 0;
    minLEDList[i].colourRGBW.green = 0;
    minLEDList[i].colourRGBW.blue = 0;
  }

}
/*===============================
           Loading...
===============================*/
int headLEDM = 0;
int headLEDH = 0;
int LEDMTail = 15;
int LEDHTail = 8;
int currentLED = 0;
//Utilities::smoothRangeLoop(int currentNumb, int increment, int maxNumb);
//Utilities::smoothRangeLoop(int currentNumb, 1, 15);
void DisplayManager::upDateLoading(void){
  //outside spin
  minLEDList[headLEDM].colourRGBW.red = 0;
  minLEDList[headLEDM].colourRGBW.green = 1;
  minLEDList[headLEDM].colourRGBW.blue = 3;

  currentLED = headLEDM;
  //Blue
  for (int i = 0; i < LEDMTail; i++){
    currentLED = Utilities::smoothRangeLoop(currentLED, -1, 59);
    Serial.println("currentLED");
    Serial.println(currentLED);
    hourLEDList[currentLED].colourRGBW.red = 0;
    hourLEDList[currentLED].colourRGBW.green = 1;
    hourLEDList[currentLED].colourRGBW.blue = 3;
  }
  //Gold
  for (int i=0; i<LEDMTail; i++){
    currentLED = Utilities::smoothRangeLoop(currentLED, -1, 59);
    minLEDList[currentLED].colourRGBW.red = 5;
    minLEDList[currentLED].colourRGBW.green = 3;
    minLEDList[currentLED].colourRGBW.blue = 1;
  }
  headLEDM = Utilities::smoothRangeLoop(headLEDM, -1, 59);

  //inside spin
  hourLEDList[headLEDH].colourRGBW.red = 0;
  hourLEDList[headLEDH].colourRGBW.green = 1;
  hourLEDList[headLEDH].colourRGBW.blue = 3;

  currentLED = headLEDH;
  for (int i=0; i < LEDHTail -1; i++){
    currentLED = Utilities::smoothRangeLoop(currentLED, 1, 15);
    Serial.println("currentLED");
    Serial.println(currentLED);
    hourLEDList[currentLED].colourRGBW.red = 0;
    hourLEDList[currentLED].colourRGBW.green = 1;
    hourLEDList[currentLED].colourRGBW.blue = 3;
  }
  for (int i=0; i<LEDHTail; i++){
    currentLED = Utilities::smoothRangeLoop(currentLED, 1, 15);
    hourLEDList[currentLED].colourRGBW.red = 5;
    hourLEDList[currentLED].colourRGBW.green = 3;
    hourLEDList[currentLED].colourRGBW.blue = 1;
  }
  headLEDH = Utilities::smoothRangeLoop(headLEDH, 1, 15);
}
