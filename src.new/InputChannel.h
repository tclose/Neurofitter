/*
Revision of last commit: $Rev: 234 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-22 14:20:25 +0900 (Mon, 22 Oct 2007) $
*/

#ifndef NEUROFITTER_INPUTCHANNEL_H
#define NEUROFITTER_INPUTCHANNEL_H

#include <string>

using namespace std;

class InputChannel {

	public:
		virtual InputChannel &operator>>(bool &) = 0;
		virtual InputChannel &operator>>(int &) = 0;
		virtual InputChannel &operator>>(unsigned &) = 0;
		virtual InputChannel &operator>>(string &) = 0;
		virtual InputChannel &operator>>(float &) = 0;
		virtual InputChannel &operator>>(double &) = 0;
		virtual ~InputChannel() {};	      
};

#endif
