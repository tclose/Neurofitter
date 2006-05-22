#ifndef EOFITNESS_H
#define EOFITNESS_H 

#include <eo>
#include <eoEvalFunc.h>

using namespace std;

#ifdef _MSC_VER
	template< class EOT, class FitT = EOT::Fitness, class FunctionArg = const EOT& >
#else
	template< class EOT, class FitT = typename EOT::Fitness, class FunctionArg = const EOT& >
#endif

struct EOFitness: public eoEvalFunc<EOT> {

	EOFitness( FitT (* _eval)( FunctionArg, FitnessCalculator * ), FitnessCalculator * f)
		: eoEvalFunc<EOT>(), evalFunc( _eval ), fitness(f) {};

	virtual void operator() ( EOT & _eo ) {
		if (_eo.invalid())
			_eo.fitness((*evalFunc)( _eo, fitness ));
		};
  
	private:
		FitT (* evalFunc )( FunctionArg, FitnessCalculator* );
		FitnessCalculator * fitness;
	}; 

#endif
