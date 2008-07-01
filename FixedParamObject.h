/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_FIXEDPARAMOBJECT_H
#define NEUROFITTER_FIXEDPARAMOBJECT_H

#include "FixedParameters.h"
#include "Tools.h"

using namespace std;

class FixedParamObject {
	public:
		void setFixedParameters(const FixedParameters newParams) {fixedParams = newParams;};
	protected:
		FixedParamObject() : fixedParams() {};
		FixedParamObject(FixedParameters params) : fixedParams(params) {};
		FixedParameters fixedParams;
		virtual ~FixedParamObject() {};
};

#endif
