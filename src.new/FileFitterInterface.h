/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
*/

#ifndef NEUROFITTER_FILEFITTERINTERFACE_H
#define NEUROFITTER_FILEFITTERINTERFACE_H

#include <fstream>

#include "FitterResults.h"
#include "FitterInterface.h"
#include "ErrorValueCalculator.h"
#include "FixedParamObject.h"

using namespace std;

class FileFitterInterface : public FitterInterface, FixedParamObject {

    public:
        FileFitterInterface(ErrorValueCalculator * fit, FixedParameters params):FitterInterface(fit), FixedParamObject(params) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters *);

	private:

};

#endif

