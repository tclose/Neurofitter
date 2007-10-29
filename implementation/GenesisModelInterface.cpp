/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../GenesisModelInterface.h"

GenesisModelInterface::GenesisModelInterface(TracesReader * t, FixedParameters params) : ModelInterface(t), FixedParamObject(params), execModel(t, params) {

    FixedParameters newParams = fixedParams;

	string genesisCommand = "cd "+fixedParams["ModelDirectory"]+"; "
                        +fixedParams["GenesisLocation"]+" -nox -notty -batch "+fixedParams["ModelSource"];

    newParams.addParameter("ExecuteCommand",genesisCommand,false);
    newParams.removeParameter("GenesisLocation");
    newParams.removeParameter("ModelDirectory");
    newParams.removeParameter("ModelSource");
    execModel.setFixedParameters(newParams);

}

ModelResults GenesisModelInterface::runModel(const ModelTuningParameters & params) {

    return execModel.runModel(params);

}


vector< ModelResults > GenesisModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList) {

	unsigned int numberOfParallelModels = paramList.size();

    vector< ModelResults > resultList(numberOfParallelModels);

    for (unsigned int i = 0; i < numberOfParallelModels; i++) {
		showMessage("\nRunning model " + str((int)i+1) + " of " + str(numberOfParallelModels) + " parallel models\n",4,fixedParams);
        resultList[i] = runModel(paramList[i]);
    }

    return resultList;

}
