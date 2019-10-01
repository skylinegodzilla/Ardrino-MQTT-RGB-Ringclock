#ifndef _TimeManager_H
#define _TimeManager_H

class TimeManager
{
  public:
    TimeManager(void);
    struct tm* upDate(void);
    void requestTime(int timeZone);
  
  private:
    struct tm timeStrut; //Create a time strut object
    struct tm *mytm;  //pointer for a time strut
    time_t now;
  
};
#endif
