#ifndef EOFITNESSCALCULATOR_H
#define EOFITNESSCALCULATOR_H 

#include <eo>
#include <eoEvalFunc.h>

#ifdef _MSC_VER
	template< class EOT, class FitT = EOT::Fitness, class FunctionArg = const EOT& >
#else
	template< class EOT, class FitT = typename EOT::Fitness, class FunctionArg = const EOT& >
#endif

///todo rename this class

struct EOFitnessCalculator: public eoEvalFunc<EOT> {

	EOFitnessCalculator( FitT (* _eval)( FunctionArg, FitnessCalculator * ), FitnessCalculator * f)
		: eoEvalFunc<EOT>(), evalFunc( _eval ) {fitness=f;};

	virtual void operator() ( EOT & _eo ) {
		if (_eo.invalid())
			_eo.fitness((*evalFunc)( _eo, fitness ));
		};
  
	private:
		FitT (* evalFunc )( FunctionArg, FitnessCalculator* );
		FitnessCalculator * fitness;
	}; 
#endif
