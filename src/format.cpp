#include <string>
#include <iostream>

#include "format.h"

using std::string;
using namespace std;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
   
    int hours = (int)(seconds/3600); // cut of decimal part by casting
    int min = (int)((seconds - hours*3600)/60);
    int s = seconds - hours*3600 - min*60;
    if (s < 10) { // for better display 01s 02s etc.
        return to_string(hours) + "h" + to_string(min) + "min0" + to_string(s) +"s"; 
    }
    return to_string(hours) + "h" + to_string(min) + "min" + to_string(s) +"s";
}
//string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }