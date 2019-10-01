#ifndef __ClockData_H
#define __ClockData_H


class ClockData
{
  public:
    ClockData(void); //The constructor
    int powerState;

    RGBW hourColor;
    RGBW minColor;
    RGBW secColor;
    RGBW backColor;
    RGBW incramentColor;
    RGBW incrament2Color;

    int hourBright;
    int minBright;
    int backBright;


    int displayMode; //1 normal, 2 rainbow
    int rainbowSpeed;
    int changedData;  //1 power, 3 HC, 4 MC, 5 SC, 6 HB, 7 MB, 8 SB, 10 mode Normal, 11 mode Rainbow
    int errorCode;
};

#endif
