/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
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


vector< ModelResults > NeuronModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList) {

	vector< ModelResults > resultList(paramList.size()); 

	for (unsigned int i = 0; i < paramList.size(); i++) {
		resultList[i] = runModel(paramList[i]);
	}

	return resultList;

}

