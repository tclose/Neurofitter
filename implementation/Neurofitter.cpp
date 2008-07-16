/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../Neurofitter.h"

using namespace std;

FixedParameters readParameters(int argc, char* argv[], int rank);

int main (int argc, char* argv[]) {

	showMessage("\nStarting Neurofitter...\n");
	showMessage("The date is: " + getDateAndTime() + "\n");		

        int rank = 0;

	#ifdef WITH_MPI		
		MPI_Init(&argc,&argv);
            
        MPI_Comm_rank( MPI_COMM_WORLD, &rank );
        showMessage("Rank: " + str(rank) + "\n");
	#endif

		////////////////////////////
		///	Read parameters file ///
		////////////////////////////
		FixedParameters fixedParams = readParameters(argc,argv,rank);

		//Sets invalid rank if not in MPI mode, only used for parallel version
		fixedParams.addParameter("Rank",str(rank),1);
		
		/////////////////////////
    	/// Declare variables ///
    	/////////////////////////
    	TracesReader * tracesReader = NULL;
    	ModelInterface * model = NULL;
    	ExperimentInterface * experiment = NULL;
    	ErrorValueCalculator * errorValue = NULL;
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
			model = new GenesisModelInterface(tracesReader,modelFixedParams);
		}
		else if (fixedParams["ModelType"] == "Neuron") {
			model = new NeuronModelInterface(tracesReader,modelFixedParams);
		}
		else if (fixedParams["ModelType"] == "Executable") {
			model = new ExecutableModelInterface(tracesReader,modelFixedParams);
		}
	#ifdef WITH_MPI
		else if (fixedParams["ModelType"] == "MPI") {
            model = new MPIModelInterface(tracesReader, modelFixedParams);
        }
	#endif
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
		else if (fixedParams["ExperimentType"] == "FileList") {
			experiment = new FileListExperimentInterface(tracesReader, expFixedParams);
		}
		else crash("Main program", "No matching experiment type");

		/////////////////////////////////////
		/// Initialize Error Value Calculator ///
		/////////////////////////////////////
		FixedParameters fitFixedParams(fixedParams["ErrorValueCalculatorParameters"],fixedParams.getGlobals());
		if (fixedParams["ErrorValueCalculatorType"] == "Matrix") {
			errorValue = new MatrixErrorValueCalculator(model,experiment,fitFixedParams);
		}
		#ifdef WITH_MPI
		else if (fixedParams["ErrorValueCalculatorType"] == "MPI") {
            errorValue = new MPIErrorValueCalculator(model,experiment,fitFixedParams);
        }
		#endif
		else crash("Main program", "No matching error value calculator type");
	

		// In case of MPI only run the fitter on the master node
		if (rank == 0) {
			/////////////////////////
			/// Initialize Fitter ///
			/////////////////////////
			FixedParameters fitterFixedParams(fixedParams["FitterParameters"],fixedParams.getGlobals());
			if (fixedParams["FitterType"] == "Mesh") {
				fitter = new MeshFitterInterface(errorValue,fitterFixedParams);
			}
			else if (fixedParams["FitterType"] == "Easy") {
				fitter = new EasyFitterInterface(errorValue,fitterFixedParams);
			}
			else if (fixedParams["FitterType"] == "SimpleGradient") {
				fitter = new SimpleGradientFitterInterface(errorValue,fitterFixedParams);
			}
			else if (fixedParams["FitterType"] == "Random") {
				fitter = new RandomFitterInterface(errorValue,fitterFixedParams);
			}
			else if (fixedParams["FitterType"] == "File") {
				fitter = new FileFitterInterface(errorValue,fitterFixedParams);
			}
			else if (fixedParams["FitterType"] == "Swarm") {
				fitter = new SwarmFitterInterface(errorValue,fitterFixedParams);
			}
			else if (fixedParams["FitterType"] == "NSGA2") {
				fitter = new NSGA2FitterInterface(errorValue,fitterFixedParams);
			}
		#ifdef WITH_NOMAD
			else if (fixedParams["FitterType"] == "NOMAD") {
				fitter = new NOMADFitterInterface(errorValue,fitterFixedParams);
			}
		#endif
		#ifdef WITH_EO
			else if (fixedParams["FitterType"] == "EO") {
				fitter = new EOFitterInterface(errorValue,fitterFixedParams);
			}
		#endif
		#ifdef WITH_EO
			#ifdef WITH_NOMAD
			else if (fixedParams["FitterType"] == "EONOMAD") {
				fitter = new EONOMADFitterInterface(errorValue,fitterFixedParams);
			}
			#endif
		#endif 
			else crash("Main program", "No matching fitter type");

			///////////
			/// Run ///
			///////////
			FitterResults results = fitter->runFitter(&startPoint);

			showMessage("\nBest fit found: "+ results.getBestFit().toString() + " with error value: " + str(results.getBestErrorValue()) +"\n",1,fixedParams);

			///////////////
			/// Cleanup ///
			///////////////
			delete fitter;
		}
		delete experiment;
		delete errorValue;
		delete model;

		#ifdef WITH_MPI
			MPI_Finalize();
		#endif

	showMessage("\nNeurofitter has finished\n");

	return 0;

}


FixedParameters readParameters(int argc, char* argv[], int rank) {

	showMessage("Commandline arguments: {");
	for (int i = 0; i < argc; i++) {
		showMessage(string(argv[i]) + ",");
	}
	showMessage("}\n");
	if (argc < 2 || argv[1]==NULL) crash("Neurofitter","Not enough arguments: "+str(argc)+" You need to give the location of the settings XML file");

	//When in MPI mode append the rank to the filename;
	#ifdef WITH_MPI	
		string fileLoc = string(argv[1]) + "_" + str(rank);
	#else
		string fileLoc = string(argv[1]);
	#endif

	ifstream paramFile(fileLoc.c_str());
	
	if (!paramFile.good()) crash("Neurofitter","Unable to open parameter file: "+fileLoc);
	
	string fileContent = string(istreambuf_iterator<char>(paramFile),istreambuf_iterator<char>());
	fileContent = XMLString::removeXMLComments(fileContent);
	FixedParameters fixedParameters = FixedParameters(XMLString("<root>"+fileContent+"</root>").getSubString("TestProgram"));

	// Say which parameters should be passed to child objects
	fixedParameters.setGlobal("Dimensions");
	fixedParameters.setGlobal("VerboseLevel");
	fixedParameters.setGlobal("SamplingFrequency");
	fixedParameters.setGlobal("Seed");
	fixedParameters.setGlobal("Bounds");
	fixedParameters.setGlobal("WorkingDirectory");

    if (toInt(fixedParameters["PrintParameterFile"]) >= 1 && rank == 0) {
		showMessage("Settings file: \n" + fileContent + "\n", 1, fixedParameters);
	}

	showMessage("VerboseLevel: " + fixedParameters["VerboseLevel"] + "\n" +
        		"Dimensions: " + fixedParameters["Dimensions"] + "\n" +
        		"Bounds: " + fixedParameters["Bounds"] + "\n" + 
        		"StartingPoints: " + fixedParameters["StartingPoints"] + "\n" + 
       			"Sampling Frequency: " + fixedParameters["SamplingFrequency"] + "\n" + 
        		"Seed: " + fixedParameters["Seed"] + "\n" + 
        		"Working Directory: " + fixedParameters["WorkingDirectory"] + "\n", 
				3, fixedParameters);

	return fixedParameters;
	
}
