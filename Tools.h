#ifndef NEUROFITTER_TOOLS_H
#define NEUROFITTER_TOOLS_H

#include <sstream>
#include <string>
#include <stdexcept>
#include <unistd.h>

#include "FixedParameters.h"

using namespace std;

/// Convert a float to a string
inline string str(float x) {
   std::ostringstream o;
   o << x;
   return o.str();
}

/// Convert an integer to a string
inline string str(int x) {
   std::ostringstream o;
   o << x;
   return o.str(); 
}

/// Convert a double to a string
inline string str(double x) {
   std::ostringstream o;
   o << x;
   return o.str();
}

/// Convert a string to an integer
inline int toInt(string s) {
   return atoi(s.c_str());
}

/// Convert a string to a double
inline double toDouble(string s) {
   return atof(s.c_str());
}

inline void showMessage(const string text, const int verboseLevel, FixedParameters fixedParams) {
	if (toInt(fixedParams["VerboseLevel"]) >= verboseLevel) {
		cout << text;
	}
}

inline void showMessage(const string text) {
	cout << text;
}

inline void flushMessage() {
	cout.flush();
}


/// Stop the program with an error message
inline void crash(string object, string message) {
	//throw runtime_error("Error in "+object+": "+message);
	cerr << "Error in " << object << ": " << message << endl;
	cerr.flush();
	//exit(1);
}

/// Get the date and time as a string
inline string getDateAndTime() {
  	ostringstream o;
  	
	// Date
  	time_t t ;
  	time (& t) ;
  	struct tm * tt = localtime (& t) ;
  	o << '[' ;


	unsigned day = tt -> tm_mday ;
  	o << day << '/' ;
  	unsigned month = 1+tt -> tm_mon ;
  	o << month << '/' ;
  	unsigned year = 1900+tt -> tm_year ;
  	o << year << ' ' ;

   
  	unsigned hours = tt -> tm_hour ;
  	if (hours < 10)
    	o << '0' ;
  	o << hours << ':' ;
  	unsigned min = tt -> tm_min ;
  	if (min < 10)
    	o << '0' ;
  	o << min << ':' ;
  	unsigned sec = tt -> tm_sec ;
  	if (sec < 10)
    	o << '0' ;
  	o << sec << "] " ;

	return o.str();
}

#endif
