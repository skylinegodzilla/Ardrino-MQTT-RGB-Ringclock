#include "RGBW.h"
//RGBW is kind of my own data type to hold the individual colour values for the led lights.
//I preferred using a object instead of a fixed 4 size int array as its easer to read and handle
RGBW::RGBW(int R, int G, int B, int W){
  red = R;
  green = G;
  blue = B;
  white = W;
}

RGBW::RGBW(){
  red = 0;
  green = 0;
  blue = 0;
  white = 0;
}

void RGBW::setRGBW(int R, int G, int B, int W){
  red = R;
  green = G;
  blue = B;
  white = W;
}
