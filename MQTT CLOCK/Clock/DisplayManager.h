#ifndef __DisplayManager_H
#define __DisplayManager_H

#include "LED.h"
class DisplayManager {
 public:
  LED hourLEDList[16];
  LED minLEDList[60];

  DisplayManager(void);
  void upDate(void);
  void changeToStatic(void);
  void upDateStatic(void);
  void upDateRainbow(void);
  void upDateLoading(void);
  RGBW colourWheel(int);



  int hpvValue(int oldhpv, int maxhpv);

  bool newMin;
  bool newSec;
  bool newHour;

  int tempDisplayMode; //a kind of flag used to see if the display mode has changed and may need to do some clean up of the old display mode

  int hpv;  // Head Pulse Value Never let this be 0!
  bool hpvUp; //true if the hpv value should be going up

  String hourToLedMap[13] = {"16,1","2","3","4,5","6","7","8,9","10","11","12,13","14","15","16,1"};//this is to map the 12 hour time to the 16 led pins that display the hour. (12/0 o'clock for example is mapped to LED 15 and 0, 1 o'clock is mapped to 1, ect)
};

#endif
