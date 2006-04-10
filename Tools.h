#ifndef TOOLS_H
#define TOOLS_H

#include <sstream>

inline std::string str(float x)
 {
   std::ostringstream o;
   o << x;
   return o.str();
 }

inline std::string str(int x)
 {
   std::ostringstream o;
   o << x;
   return o.str(); 
 }

inline std::string str(double x)
 {
   std::ostringstream o;
   o << x;
   return o.str();
 }

#endif
