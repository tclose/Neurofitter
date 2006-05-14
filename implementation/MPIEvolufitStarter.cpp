#include <mpi.h>

#include "../MPIEvolufitStarter.h"


MPIEvolufitStarter::MPIEvolufitStarter(FixedParameters params)
		: FixedParamObject(params) {}

void MPIEvolufitStarter::run(int argc, char** argv) {

	int rank;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );

	cout << "Rank: " << rank << endl;

	/////////////////////////
    /// Declare variables ///
    /////////////////////////
    ModelInterface * model;
    ExperimentInterface * experiment;
    FitnessCalculator * fitness;
    FitterInterface * fitter;

    ModelTuningParameters startPoint(fixedParams["StartingPoints"],toInt(fixedParams["Dimensions"]),fixedParams["Bounds"]);

    if (toInt(fixedParams["VerboseLevel"]) > 2) {
        cout << "VerboseLevel: " << fixedParams["VerboseLevel"] << endl;
        cout << "Dimensions: " << fixedParams["Dimensions"] << endl;
        cout << "Bounds: "<< fixedParams["Bounds"] << endl;
        cout << "StartingPoints: " << fixedParams["StartingPoints"] << endl;
    }

	////////////////////////
	/// Initialize Model ///
	////////////////////////
	FixedParameters modelFixedParams(fixedParams["ModelParameters"],fixedParams.getGlobals());
	if (fixedParams["ModelType"] == "Genesis") {
		model = new GenesisModelInterface(modelFixedParams);
	}
	else if (fixedParams["ModelType"] == "MPI") {
		fitter = new MeshFitterInterface(fitness,fitterFixedParams);
	}
	else crash("Main program", "No matching model type");
	if (rank == 0) {
		/////////////////////////////
		/// Initialize Experiment ///
		/////////////////////////////
		FixedParameters expFixedParams(fixedParams["ExperimentParameters"],fixedParams.getGlobals());
		if (fixedParams["ExperimentType"] == "Fake") {
			experiment = new FakeExperimentInterface(model, expFixedParams);
		}
		else crash("Main program", "No matching experiment type");

		/////////////////////////////////////
		/// Initialize Fitness Calculator ///
		/////////////////////////////////////
		FixedParameters fitFixedParams(fixedParams["FitnessCalculatorParameters"],fixedParams.getGlobals());
		if (fixedParams["FitnessCalculatorType"] == "Pablo") {
			fitness = new PabloFitnessCalculator(model,experiment,fitFixedParams);
		}
		else crash("Main program", "No matching fitness calculator type");

		/////////////////////////
		/// Initialize Fitter ///
		/////////////////////////
		FixedParameters fitterFixedParams(fixedParams["FitterParameters"],fixedParams.getGlobals());
		if (fixedParams["FitterType"] == "Pablo") {
			fitter = new PabloFitterInterface(fitness,fitterFixedParams);
		}
		else if (fixedParams["FitterType"] == "Mesh") {
			fitter = new MeshFitterInterface(fitness,fitterFixedParams);
		}
		else if (fixedParams["FitterType"] == "Swarm") {
			fitter = new SwarmFitterInterface(fitness,fitterFixedParams);
		}
		else if (fixedParams["FitterType"] == "NOMAD") {
			fitter = new NOMADFitterInterface(fitness,fitterFixedParams);
		}
		else if (fixedParams["FitterType"] == "EO") {
			fitter = new EOFitterInterface(fitness,fitterFixedParams);
		}
		else crash("Main program", "No matching fitter type");

		///////////
		/// Run ///
		///////////
		fitter->runFitter(&startPoint);


		///////////////
		/// Cleanup ///
		///////////////
		delete model;
		delete experiment;
		delete fitness;
		delete fitter;
	}
}
