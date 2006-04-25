#include "../GenesisModelInterface.h"

ModelResults	GenesisModelInterface::
runModel(const ModelTuningParameters & params) const {

	const string workpath = fixedParams["modelDirectory"];
	const string genpath = fixedParams["genesisLocation"]+"/";

	const int       numberOfInjections = 1;
	const int       numberOfRecordSites = 1;
	const int       numberOfPeriods = 1;

	//const double injWeight [] = {0.1,0.1,0.1,0.1,0.5,0.5,1.0,1.0};
	const double injWeight [] = {1.0};

	ofstream		paramFile;
	ofstream        fixedParamFile;

	ifstream		errorFile;

	string			paramFilename;
	string          fixedParamName;
	string			errorFileName;
	string          genCommand;
	string			inputname;

	int             nInject;
	float           inject; //inject current in nA

	ModelResults	results(numberOfInjections*numberOfRecordSites*numberOfPeriods);


	/////
	///	fill conductance parameter file
	/////
    paramFilename = workpath + fixedParams["inputFile"]; 
    paramFile.open(paramFilename.c_str(), ios::out);

    paramFile << "float GNaP = " << params[0] << "e-12"<< endl;
    paramFile << "float GKdr = " << params[1] << "e-12"<<endl;
    paramFile.close ();


	/* call genesis with several injection currents */

	for (nInject = 0; nInject < numberOfInjections; nInject++) {

		switch (nInject) {
			case 0: inject = 0.0; 		break;
			//case 0: case 4: inject = 0.005; 		break;
			case 1: case 5: inject = 0.0001; 	break;
			case 2: case 6: inject = 0.0005; 	break;
			case 3: case 7: inject = 0.0; 	break;
			default:
				cerr << "Wrong number of injection sites";
				exit(1);
		}

		/////
		/// fill inject file
		/////

		fixedParamName = workpath + "/fixedparam.g";
		inputname = workpath + "/output_inject" + str(nInject)  + "_" + str(inject) + "A.dat";
		fixedParamFile.open(fixedParamName.c_str(), ios::out);
		fixedParamFile << "str outputfile = \"" << inputname << "\""<<endl;		
		fixedParamFile << "float inject = " << inject << endl;
		if (nInject < 4) {
			fixedParamFile << "float injectsite = " << str(0) << endl;			
		}
		else {
			fixedParamFile << "float injectsite = " << str(1) << endl;			
		}
		fixedParamFile.close();

		errorFileName = workpath + "/gen.err";
		errorFile.open(errorFileName.c_str(), ios::trunc);
		errorFile.close();

		/////
		/// call Genesis
		/////

		///todo add in pablo's modelinterface nox notty batch
		genCommand = "cd "+workpath+"; "+genpath+"-nox -notty -batch"+fixedParams["inputFile"]+" > gen.out 2> gen.err";
		if (toInt(fixedParams["VerboseLevel"]) > 4) {cout << endl << "calling " << genCommand << " with a current of " << inject << endl;}
		
		system(genCommand.c_str());

		/////
		/// Check to see if there was no error in genesis
		/////

		errorFile.open(errorFileName.c_str(), ios::in);
		errorFile.seekg (0, ios::end);
  		int length = errorFile.tellg();
		if (errorFile.bad() || length != 228) {cerr << "Error while running genesis simulation in GenesisModeInterface" << endl;exit(1);}
		errorFile.close();


		/////
		/// read results 
		/////

		//todo make it possible to control from outside genesis to name the filename
	
		readDataFromFile(results, inputname, nInject, numberOfRecordSites, numberOfPeriods, injWeight[nInject]);

	}

	return results;
}


void GenesisModelInterface::readDataFromFile(ModelResults & results, string inputFileName, int nInject, int numberOfRecordSites, int numberOfPeriods, double injWeight) {

		const double	weights[][2] = {{.5,.5},{.5,.5}};		

		///Periods should be in order of appearance !!!
		//const int		periodStart[] = {50,500};
		//const int		periodStop []= {100,1000};

		const int		periodStart[] = {10000};
		const int		periodStop []= {19999};

		//cout <<  "opening " << inputFileName << endl;

		ifstream		inputfile;
     	inputfile.open(inputFileName.c_str(), ios::in);
     	inputfile.clear();  


		int nPoints = 0;

		for (int nPeriod = 0; nPeriod < numberOfPeriods; nPeriod++) {
			double time, dummy;
			///todo should be replaced by a seek
			while (nPoints < periodStart[nPeriod]) {
				inputfile >> time;
				inputfile >> dummy;
				nPoints = nPoints + 1;
			}

			int traceNumberSoma = nInject*numberOfRecordSites*numberOfPeriods+0*numberOfPeriods+nPeriod;
			//cout << "Soma: " << traceNumberSoma << endl;
			//int traceNumberDend1 = nInject*numberOfRecordSites*numberOfPeriods+1*numberOfPeriods+nPeriod;
			//cout << "Dend: " << traceNumberDend1 << endl;


			results[traceNumberSoma].resetAndSetLength(periodStop[nPeriod] - periodStart[nPeriod] + 1);
			//results[traceNumberDend1].resetAndSetLength(periodStop[nPeriod] - periodStart[nPeriod] + 1);
		
			string name;
			results[traceNumberSoma].setName("Injection: " + str(nInject) + ", Period: " + str(nPeriod) + ", Soma, Filename: "+inputFileName);
			//results[traceNumberDend1].setName("Injection: " + str(nInject) + ", Period: " + str(nPeriod) + ", Dend1, Filename: "+inputFileName);

			results[traceNumberSoma].setSamplingFrequency(5000);
			//results[traceNumberDend1].setSamplingFrequency(50000);

			results[traceNumberSoma].setWeight(injWeight*weights[nPeriod][0]);
			//results[traceNumberDend1].setWeight(injWeight*weights[nPeriod][1]);

			for (int j = 0; j < results[traceNumberSoma].getLength(); j++) {
				inputfile >> time;
				if (j==0) {
					results[traceNumberSoma].setStartTime(time);
                	//results[traceNumberDend1].setStartTime(time);
				}		
				inputfile >> results[traceNumberSoma][j];
				//inputfile >> results[traceNumberDend1][j];
				nPoints = nPoints + 1;
			}

			results[traceNumberSoma].setStopTime(time);
            //results[traceNumberDend1].setStopTime(time);
		}

}
