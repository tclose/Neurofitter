/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_OUTPUTCHANNEL_H
#define NEUROFITTER_OUTPUTCHANNEL_H

#include <string>

using namespace std;

/// Abstract class that provides a general channel to send output to
class OutputChannel {

	public:
    	virtual OutputChannel &operator<<(const bool &) = 0; ///< Send a bool
    	virtual OutputChannel &operator<<(const int &) = 0; ///< Send an integer
    	virtual OutputChannel &operator<<(const unsigned &) = 0; ///< Send an unsigned integer
    	virtual OutputChannel &operator<<(const string &) = 0; ///< Send a string
    	virtual OutputChannel &operator<<(const float &) = 0; ///< Send a float
    	virtual OutputChannel &operator<<(const double &) = 0; ///< Send a double
    	virtual ~OutputChannel() {}; ///< Destructor
            
};	
            
#endif
            
