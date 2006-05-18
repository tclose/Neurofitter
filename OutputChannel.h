#ifndef EVOLUFITOUTPUTCHANNEL_H
#define EVOLUFITOUTPUTCHANNEL_H

#include <string>

using namespace std;

class OutputChannel {

	public:
    	virtual OutputChannel &operator<<(const int &) = 0;
    	virtual OutputChannel &operator<<(const unsigned &) = 0;
    	virtual OutputChannel &operator<<(const string &) = 0;
    	virtual OutputChannel &operator<<(const float &) = 0;
    	virtual OutputChannel &operator<<(const double &) = 0;
    	virtual ~OutputChannel() = 0;
            
};	
            
#endif
            
