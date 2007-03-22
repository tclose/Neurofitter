/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_EOFITNESS_H
#define NEUROFITTER_EOFITNESS_H 

//#include <eo>
#include <eoEvalFunc.h>

using namespace std;

#ifdef _MSC_VER
	template< class EOT, class FitT = EOT::Fitness, class FunctionArg = const EOT& >
#else
	template< class EOT, class FitT = typename EOT::Fitness, class FunctionArg = const EOT& >
#endif

///A structure used by EO to represent the fitness function
struct EOFitness: public eoEvalFunc<EOT> {

	///Constructor reading the fitness evaluation function and the fitnesscalculator object
	EOFitness( FitT (* _eval)( FunctionArg, FitnessCalculator * ), FitnessCalculator * f)
		: eoEvalFunc<EOT>(), evalFunc( _eval ), fitness(f) {};

	///Operator used to evaluate fitness function
	virtual void operator() ( EOT & _eo ) {
		if (_eo.invalid())
			_eo.fitness((*evalFunc)( _eo, fitness ));
		};
  
	private:
		FitT (* evalFunc )( FunctionArg, FitnessCalculator* ); ///< Evaluation function used by operator()
		FitnessCalculator * fitness; ///< Fitness calculator object
	}; 

#endif
