#ifndef FIXEDPARAMOBJECT_H
#define FIXEDPARAMOBJECT_H

#include "FixedParameters.h"

class FixedParamObject {
	protected:
		FixedParamObject() : fixedParams() {};
		FixedParamObject(FixedParameters params) : fixedParams(params) {};
		FixedParameters fixedParams;
};

#endif
