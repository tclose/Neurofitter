/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../NeuronModelInterface.h"

/// Thx to Michiel Berends for his assistance while developing this class 

NeuronModelInterface::NeuronModelInterface(TracesReader * t, FixedParameters params) : ModelInterface(t), FixedParamObject(params), execModel(t, params) {

	FixedParameters newParams = fixedParams;

	string neuronCommand = "cd "+fixedParams["ModelDirectory"]+"; "+fixedParams["SpecialLocation"]+" "+fixedParams["ModelSource"];
	
	newParams.addParameter("ExecuteCommand",neuronCommand,false);
	newParams.removeParameter("SpecialLocation");	
	newParams.removeParameter("ModelDirectory");	
	newParams.removeParameter("ModelSource");	
	execModel.setFixedParameters(newParams);
	
}

ModelResults NeuronModelInterface::runModel(const ModelTuningParameters & params) {

	return execModel.runModel(params);

}


void NeuronModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList, vector< ModelResults > & results) {

	execModel.runParallelModel(paramList, results);

}

