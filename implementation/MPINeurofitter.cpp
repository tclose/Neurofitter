#include "../Neurofitter.h"
#include "../MPINeurofitter.h"

using namespace std;

///todo check if all files are closed
///todo check for leaks
///todo add comments
///todo make all classes use initializers
///todo the correct header files to every header file
///todo check in every class to see if virtual is OK
///todo make it possible to fix some variables
///todo remove Houston
///todo make separate executable for mpi

FixedParameters readParameters(int argc, char* argv[],int rank);

int main (int argc, char* argv[]) {
	cout << "Houston, we have liftoff..." << endl;
	cout << "The date is: " << getDateAndTime() << endl;
		MPI_Init(&argc,&argv);

		int rank;
		MPI_Comm_rank( MPI_COMM_WORLD, &rank );
		cout << "Rank: " << rank << endl;
		
		////////////////////////////
		///	Read parameters file ///
		////////////////////////////
		FixedParameters fixedParams = readParameters(argc,argv,rank);

		fixedParams.addParameter("Rank",str(rank),1);
		
		/////////////////////////
    	/// Declare variables ///
    	/////////////////////////
    	TracesReader * tracesReader = NULL;
    	ModelInterface * model = NULL;
    	ExperimentInterface * experiment = NULL;
    	FitnessCalculator * fitness = NULL;
    	FitterInterface * fitter = NULL;

    	ModelTuningParameters startPoint(fixedParams["StartingPoints"],toInt(fixedParams["Dimensions"]),fixedParams["Bounds"]);

		///////////////////////////////
		/// Initialize TracesReader ///
		///////////////////////////////
		FixedParameters tracesReaderFixedParams(fixedParams["TracesReaderParameters"],fixedParams.getGlobals());
		if (fixedParams["TracesReaderType"] == "Normal") {
			tracesReader = new NormalTracesReader(tracesReaderFixedParams);
		}
		else crash("Main program", "No matching trace reader type");


		////////////////////////
		/// Initialize Model ///
		////////////////////////
		FixedParameters modelFixedParams(fixedParams["ModelParameters"],fixedParams.getGlobals());
		if (fixedParams["ModelType"] == "Genesis") {
			model = new GenesisModelInterface(tracesReader, modelFixedParams);
		}
		else if (fixedParams["ModelType"] == "MPI") {
			model = new MPIModelInterface(tracesReader, modelFixedParams);
		}
		else crash("Main program", "No matching model type");
	
		/////////////////////////////
		/// Initialize Experiment ///
		/////////////////////////////
		FixedParameters expFixedParams(fixedParams["ExperimentParameters"],fixedParams.getGlobals());
		if (fixedParams["ExperimentType"] == "Fake") {
			experiment = new FakeExperimentInterface(model, expFixedParams);
		}
		else if (fixedParams["ExperimentType"] == "File") {
			experiment = new FileExperimentInterface(tracesReader, expFixedParams);
		}
		else crash("Main program", "No matching experiment type");

		/////////////////////////////////////
		/// Initialize Fitness Calculator ///
		/////////////////////////////////////
		FixedParameters fitFixedParams(fixedParams["FitnessCalculatorParameters"],fixedParams.getGlobals());
		if (fixedParams["FitnessCalculatorType"] == "Matrix") {
			fitness = new MatrixFitnessCalculator(model,experiment,fitFixedParams);
		}
		else if (fixedParams["FitnessCalculatorType"] == "MPI") {
			fitness = new MPIFitnessCalculator(model,experiment,fitFixedParams);
		}
		else crash("Main program", "No matching fitness calculator type");
	
		if (rank == 0) {

			/////////////////////////
			/// Initialize Fitter ///
			/////////////////////////
			FixedParameters fitterFixedParams(fixedParams["FitterParameters"],fixedParams.getGlobals());
			if (fixedParams["FitterType"] == "EONOMAD") {
				fitter = new EONOMADFitterInterface(fitness,fitterFixedParams);
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
			delete fitter;
		}
		delete experiment;
		delete fitness;
		delete model;

		MPI_Finalize();

	cout << endl << "And we have touchdown" << endl;

	return 0;
}


FixedParameters readParameters(int argc, char* argv[], int rank) {

	cout << "Arguments: {";
	for (int i = 0; i < argc; i++) {
		cout << string(argv[i]) << ",";
	}
	cout << "}" << endl;
	if (argc < 2 || argv[1]==NULL) crash("Neurofitter","Not enough arguments: "+str(argc));
	
	string fileLoc = string(argv[1]) + "_" + str(rank);
	ifstream paramFile(fileLoc.c_str());
	string fileContent = string(istreambuf_iterator<char>(paramFile),istreambuf_iterator<char>());
	FixedParameters fixedParameters = FixedParameters(XMLString("<root>"+fileContent+"</root>").getSubString("TestProgram"));

	// Say which parameters should be passed to child objects
	fixedParameters.setGlobal("Dimensions");
	fixedParameters.setGlobal("VerboseLevel");
	fixedParameters.setGlobal("SamplingFrequency");
	fixedParameters.setGlobal("Seed");
	fixedParameters.setGlobal("Bounds");
	fixedParameters.setGlobal("WorkingDirectory");

    if (toInt(fixedParameters["VerboseLevel"]) > 2) {
    	cout << "VerboseLevel: " << fixedParameters["VerboseLevel"] << endl;
        cout << "Dimensions: " << fixedParameters["Dimensions"] << endl;
        cout << "Bounds: "<< fixedParameters["Bounds"] << endl;
        cout << "StartingPoints: " << fixedParameters["StartingPoints"] << endl;
    }

	return fixedParameters;
	
}
