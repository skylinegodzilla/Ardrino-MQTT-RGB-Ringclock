#ifndef __RGBW_H
#define __RGBW_H

class RGBW {
  public:
    RGBW(int R, int G, int B, int W);
    RGBW(void);
    void setRGBW(int R, int G, int B, int W);
    int red;
    int green;
    int blue;
    int white;
};
#endif
