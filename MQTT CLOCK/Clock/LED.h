#ifndef _LED_H
#define _LED_H

class LED{
  public:
    LED(void);
    LED(int id);
    void setHueVal(int val);

    int LEDNumb;        //this is an id for the led
    RGBW colourRGBW;    //This is the current colour of the led
    RGBW oldColourRGBW; //use this if you want to remember what colour an led used to be

    int hueVal;         //This is for the colour wheel picker The wheel itself will output an RGBW value
};
#endif
