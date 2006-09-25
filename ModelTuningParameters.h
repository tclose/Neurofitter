#ifndef NEUROFITTER_MODELTUNINGPARAMETERS_H
#define NEUROFITTER_MODELTUNINGPARAMETERS_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "InputChannel.h"
#include "OutputChannel.h"

#include "Tools.h"

using namespace std;

/// Class that contains the parameters of the model that need to be tuned

class ModelTuningParameters {

	public:

		ModelTuningParameters();	///< Default constructor
		ModelTuningParameters(const int length); ///< Construct with certain length
			/// Construct with initial parameters and bounds
		ModelTuningParameters(const vector< double > params, const int length, const vector< double > bounds);
			
			/// Construct with initial parameters and bounds read from strings
			///
			/// Read parameters from string like "param1 param2 param3". \n
			/// Read bounds from string like "lower1 upper1 lower2 upper2 lower3 upper3".
		ModelTuningParameters(const string, const int length, const string bounds); 

		int getLength() const;	///< Get the number of parameters
	
		void setTuningParameters(const vector< double >, const int); ///< Change the parameters in the object
		void setTuningParameters(const string, const int); ///< Change the parameters in the object (read from a string)

		void setBounds(const vector< double >, const int boundsLength);	///< Change the bounds in the object	
		void setBounds(const string, const int boundsLength); ///< Change the bounds in the object (read from a string)

		double getLowerBound(const int) const; ///< Get the lower bound of the parameters at index
		double getUpperBound(const int) const; ///< Get the upper bound of the parameters at index

		void setLowerBound(const int, const double); ///< Set the lower bound of the parameter at index
		void setUpperBound(const int, const double); ///< Set the upper bound of the parameter at index

		double &operator[](const int);	///< Get parameters at index
		const double &operator[](const int) const;	///< Get parameters at index (const)

		void resetFitnessValue();
		void setFitnessValue(const double newValue);	///< Set the fitness value of the parameters
		double getFitnessValue() const;	///< Get the fitness value of the parameters

		string toString() const; ///< Returns a string containing the parameters

		void printOn(OutputChannel &) const; ///< Print the object on a stream
		void readFrom(InputChannel &); ///< Read the object from a stream


	private:

		vector < double > tuningParameters; ///< Vector containing the parameters
		vector < double > bounds; ///< Vector containing the bounds (closed interval)
	
		double fitnessValue; ///< The fitness value of the parameters

		bool fitnessIsValid; ///< Set only when fitness value is initialized
};


#endif
