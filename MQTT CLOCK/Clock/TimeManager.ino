#include "TimeManager.h"
//TODO: google tm strut to see what kind of data is stored in one

//This module is used to grab the time from the internet at boot and then mateine it.

struct tm timeStrut; //Create a time strut object
struct tm *mytm;  //pointer for a time strut
time_t now = time(nullptr); //time_t time (time_t* timer);//Time_t datatype = time(empty pointer) ////// May want to move this lime out of the update scope to stop recreating it

TimeManager::TimeManager(void){}

void TimeManager::requestTime(int timeZone){
  //Request time from the internet
  configTime(timeZone * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(100);
  }
  upDate();
}
struct tm* TimeManager::upDate(void){
  //update the time Data
  time (&now); // Now is a time_t type so &now is a time_t* type // Anyway this gets the current time
  mytm = localtime_r (&now, &timeStrut); //Takes the time that's pointed to by now and breaks it apart in timeStrut then returns it as a pointer to mytm
return mytm;
}
