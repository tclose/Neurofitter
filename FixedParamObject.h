#ifndef NEUROFITTER_FIXEDPARAMOBJECT_H
#define NEUROFITTER_FIXEDPARAMOBJECT_H

#include "FixedParameters.h"

using namespace std;

class FixedParamObject {
	public:
		void setFixedParameters(const FixedParameters newParams) {fixedParams = newParams;};
	protected:
		FixedParamObject() : fixedParams() {};
		FixedParamObject(FixedParameters params) : fixedParams(params) {};
		FixedParameters fixedParams;
};

#endif
