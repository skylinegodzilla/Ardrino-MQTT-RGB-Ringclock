#include "LED.h"
//I created this class so that each led could hold values. I found in my experiments that I could create more unique animations this way then without it.

LED::LED(void){
}

LED::LED(int id){
  LEDNumb = id;
}

void LED::setHueVal(int val){
  //This is to reset if values go way to far out of range
  if (val >= 512){val = 0;}
  if (val <= -256) {val = 255;}
  //This is to transition to a more smooth value change (for example if the value is increased by 5 then 255 will go to 4 instead of 0)
  if (val >= 256){val = 0 + val - 256;}
  if (val <= -1){val = 255 + val;}
  hueVal = val;
}
