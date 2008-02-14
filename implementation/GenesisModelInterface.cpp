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


void GenesisModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList, vector< ModelResults > & results) {

	execModel.runParallelModel(paramList, results);

}
