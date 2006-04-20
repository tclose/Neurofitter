#ifndef TOOLS_H
#define TOOLS_H

#include <sstream>
#include <string>


using namespace std;

inline string str(float x)
 {
   std::ostringstream o;
   o << x;
   return o.str();
 }

inline string str(int x)
 {
   std::ostringstream o;
   o << x;
   return o.str(); 
 }

inline string str(double x)
 {
   std::ostringstream o;
   o << x;
   return o.str();
 }

inline int toInt(string s)
 {
   return atoi(s.c_str());
 }

inline double toDouble(string s)
 {
   return atof(s.c_str());
 }

#endif
