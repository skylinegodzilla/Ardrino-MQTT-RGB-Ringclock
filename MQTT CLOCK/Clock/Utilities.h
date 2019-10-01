#ifndef __Utilities_h
#define __Utilities_h


class Utilities {
  public:
   Utilities(void); //The constructor
   static std::vector<String> splitStringToVector(String msg); //Method to split a CSV string in to a vector populated by its values
   static int to12HourFormat(int millTime); //Converts military time hours to 12 hour format.
   static int notLessThenZero(int value);
   static int smoothRangeLoop(int currentNumb, int increment, int maxNumb);
};
#endif
