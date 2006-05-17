#ifndef EVOLUFITINPUTCHANNEL_H
#define EVOLUFITINPUTCHANNEL_H

#include <string>

using namespace std;

class InputChannel {

	public:
		virtual InputChannel &operator>>(int &) = 0;
		virtual InputChannel &operator>>(unsigned &) = 0;
		virtual InputChannel &operator>>(string &) = 0;
		virtual InputChannel &operator>>(float &) = 0;
		virtual InputChannel &operator>>(double &) = 0;
	      
};

#endif
