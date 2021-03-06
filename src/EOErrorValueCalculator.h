/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_EOERRORVALUECALCULATOR_H
#define NEUROFITTER_EOERRORVALUECALCULATOR_H 

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

	///Constructor reading the ErrorValue evaluation function and the ErrorValuecalculator object
	EOFitness( FitT (* _eval)( FunctionArg, ErrorValueCalculator * ), ErrorValueCalculator * f)
		: eoEvalFunc<EOT>(), evalFunc( _eval ), errorValue(f) {};

	///Operator used to evaluate fitness function
	virtual void operator() ( EOT & _eo ) {
		if (_eo.invalid())
			_eo.fitness((*evalFunc)( _eo, errorValue ));
		};
  
	private:
		FitT (* evalFunc )( FunctionArg, ErrorValueCalculator* ); ///< Evaluation function used by operator()
		ErrorValueCalculator * errorValue; ///< Error Value calculator object
	}; 

#endif
