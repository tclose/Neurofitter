#ifndef TOOLS_H
#define TOOLS_H

#include <sstream>
#include <string>
#include <stdexcept>
#include <unistd.h>

using namespace std;

inline string str(float x) {
   std::ostringstream o;
   o << x;
   return o.str();
}

inline string str(int x) {
   std::ostringstream o;
   o << x;
   return o.str(); 
}

inline string str(double x) {
   std::ostringstream o;
   o << x;
   return o.str();
}

inline int toInt(string s) {
   return atoi(s.c_str());
}

inline double toDouble(string s) {
   return atof(s.c_str());
}

inline void crash(string object, string message) {
	throw runtime_error(object+": "+message);
	//cerr << object << ": " << message << endl;
	//cerr.flush();
	//exit(1);
}

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
