#include "ClockData.h"
//Clock Data Class
//The Clock Data Object is used to hold global values that will be shared between other modules/objects

ClockData::ClockData()
{
    powerState = 0;
    RGBW hourColor(128,128,0,128);
    RGBW minColor(0,128,128,128);
    RGBW secColor(200,200,200,200);
    RGBW backColor(000,000,50,000);
    RGBW incramentColor(255,255,255,255);
    RGBW incrament2Color(255,255,255,255);

    hourBright = 255;
    minBright = 255;
    backBright = 255;

    displayMode = 1; // 0 error, 1 loading, 2 static, 3 rainbow
    rainbowSpeed = 10;
    changedData = 0; //This is a int that changes is value based on what data value has changed. 0= no change, 1= powerstate, 2= RGBW hour Colour, ect
    errorCode = 0;
}
