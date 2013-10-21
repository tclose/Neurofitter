/*
Revision of last commit: $Rev: 184 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-07-12 15:27:13 +0900 (Thu, 12 Jul 2007) $
*/

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
		virtual ~FixedParamObject() {};
};

#endif
