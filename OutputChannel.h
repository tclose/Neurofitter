#ifndef NEUROFITTEROUTPUTCHANNEL_H
#define NEUROFITTEROUTPUTCHANNEL_H

#include <string>

using namespace std;

class OutputChannel {

	public:
    	virtual OutputChannel &operator<<(const int &) = 0;
    	virtual OutputChannel &operator<<(const unsigned &) = 0;
    	virtual OutputChannel &operator<<(const string &) = 0;
    	virtual OutputChannel &operator<<(const float &) = 0;
    	virtual OutputChannel &operator<<(const double &) = 0;
    	virtual ~OutputChannel() {};
            
};	
            
#endif
            
