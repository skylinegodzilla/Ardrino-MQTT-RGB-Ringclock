#include "Utilities.h"
//This class is for any tools that are useful to use

Utilities::Utilities(){
  //do nothing just create the object
}

std::vector<String> Utilities::splitStringToVector(String msg)
{
  std::vector<String> subStrings;
  int j = 0;
  for(int i = 0; i < msg.length(); i++){
    if(msg.charAt(i) == ','){
      subStrings.push_back(msg.substring(j,i));
      j = i + 1;
    }
  }
  subStrings.push_back(msg.substring(j,msg.length())); //to grab the last value of the string
  return subStrings;
}


int Utilities::to12HourFormat(int millTime){
  return ((millTime > 12) ? millTime - 12 : millTime);
  }

int Utilities::notLessThenZero(int value){
  return ((value <= 0) ? 0 : value);
}

int Utilities::smoothRangeLoop(int currentNumb, int increment, int maxNumb){
  int val;
  //This is to reset if values go way to far out of range
  if (currentNumb + increment > maxNumb*2){val = 0;}
  if (val <= -maxNumb) {val = maxNumb;}

   //This is to transition to a more smooth value change (for example if the value is increased by 5 then 255 will go to 4 instead of 0)
  if(currentNumb + increment > maxNumb){val = 0 + currentNumb - maxNumb;}//Going up
  else if(currentNumb + increment < 0){val = maxNumb + currentNumb;}//Going down
  else {val = currentNumb + increment;}//increment like normal
  return val;
}
