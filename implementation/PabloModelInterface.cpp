#include "../PabloModelInterface.h"

ModelResults	PabloModelInterface::
runModel(const ModelTuningParameters & params) {

	const string          workpath = "/Users/werner/Desktop/EvolufitWork/pablomodel";
	const string          genpath = "/usr/local/bin/";

	const int       numberOfInjections = 1;
	const int       numberOfRecordSites = 3;
	const int       numberOfPeriods = 2;

	ofstream		paramFile;
	ofstream        injectfile;

	string			paramFilename;
	string          injectname;
	string          genCommand;
	string			inputname;

	int             nInject;
	float           inject; //inject current in nA
	float           percenttime; //duration of injection

	ModelResults	results(numberOfInjections*numberOfRecordSites*numberOfPeriods);


	/////
	///	fill conductance parameter file
	/////
    paramFilename = workpath + "/myPurk_param.g"; 
    paramFile.open(paramFilename.c_str(), ios::out);


    paramFile << "float GNaFs = " << params[0] <<endl;
    paramFile << "float GNaPs = " << params[1] <<endl;
    paramFile << "float GCaTs = " << params[2] <<endl;
    paramFile << "float GCaTm = " << params[3] <<endl;
    paramFile << "float GCaTt = " << params[4] <<endl;
    paramFile << "float GCaTd = " << params[5] <<endl;
    paramFile << "float GCaPm = " << params[6] <<endl;
    paramFile << "float GCaPt = " << params[7] <<endl;
    paramFile << "float GCaPd = " << params[8] <<endl;
    paramFile << "float GKAs = " <<  params[9] <<endl;
    paramFile << "float GKAm = " <<  params[10] <<endl;
    paramFile << "float GKdrs = " << params[11] <<endl;
    paramFile << "float GKdrm = " << params[12] <<endl;
    paramFile << "float GKMs = " << params[13] <<endl;
    paramFile << "float GKMm = " << params[14] <<endl;
    paramFile << "float GKMt = " << params[15] <<endl;
    paramFile << "float GKMd = " << params[16] <<endl;
    paramFile << "float GKCm = " << params[17] <<endl;
    paramFile << "float GKCt = " << params[18] <<endl;
    paramFile << "float GKCd = " << params[19] <<endl;
    paramFile << "float GK2m = " << params[20] <<endl;
    paramFile << "float GK2t = " << params[21] <<endl;
    paramFile << "float GK2d = " << params[22] <<endl;
    paramFile << "float Ghs = " << params[23]  <<endl;  
    paramFile << "float percentInject = 1"  <<endl;   //control the injection wrt 1.5 nA
    paramFile << "int dtFactor = 1" <<endl;           //control the number of output
    paramFile.close ();


	/* call genesis with several injection currents */

	for (nInject = 0; nInject < numberOfInjections; nInject++) {

		switch (nInject) {
			case 0: inject = 0.; 	percenttime = 1; 	break;
			case 1: inject = 0.5; 	percenttime = 0.02; break;
			case 2: inject = 3.; 	percenttime = 3; 	break;
			case 3: inject = 1.5; 	percenttime = 2; 	break;
			case 4: inject = 1; 	percenttime = 2; 	break;
			case 5: inject = 1.5;	percenttime = 2; 	break;
			case 6: inject = 2.5; 	percenttime = 3;	break;
			default:
				cerr << "Wrong number of injection site";
				exit(1);
		}


		/////
		/// fill inject file
		/////

		injectname = workpath + "/myPurk_inject.g";
		injectfile.open(injectname.c_str(), ios::out);
		injectfile << "float inject = " << inject << endl;
		injectfile << "float percenttime = " << percenttime << endl;
		injectfile.close();

		/////
		/// call Genesis
		/////

		if (nInject == 3) {
			genCommand = "cd "+workpath+"; "+genpath+"/genesis mylocaldendCURRENT9.g > gen.out 2>gen.err < voidness";
		}
		else {
			genCommand = "cd "+workpath+"; "+genpath+"/genesis mylocalCURRENT9.g > gen.out 2> gen.err < voidness";
		}

		cout << endl << "calling " << genCommand << " with a current of " << inject << " during " << percenttime << "s in " << injectname << endl;
		
		//system(genCommand.c_str());
		
		/////
		/// read results 
		/////

		//todo make it possible to control from outside genesis to name the filename

		if(nInject==3) {
			inputname = workpath + "/results/PurkM9_dend_" + str(inject) + "nA";
		}
     	else {
			inputname = workpath + "/results/PurkM9_soma_" + str(inject) + "nA";
		}
	
		readDataFromFile(results, inputname, nInject, numberOfRecordSites, numberOfPeriods);

	}

	return results;
}


void PabloModelInterface::readDataFromFile(ModelResults & results, string inputFileName, int nInject, int numberOfRecordSites, int numberOfPeriods) {

		const double	weights[][2] = {{.1,.5},{.1,.5}};		

		///Periods should be in order of appearance !!!
		const int		periodStart[] = {5000,25000};
		const int		periodStop []= {10000,55000};

		cout <<  "opening " << inputFileName << endl;

		ifstream		inputfile;
     	inputfile.open(inputFileName.c_str(), ios::in);
     	inputfile.clear();  


		int nPoints = 0;

		for (int nPeriod = 0; nPeriod < numberOfPeriods; nPeriod++) {
			double time;
			///todo should be replaced by a seek
			while (nPoints < periodStart[nPeriod]) {
				double dummy;
				inputfile >> time;
				inputfile >> dummy;
				inputfile >> dummy;
				inputfile >> dummy;
				nPoints = nPoints + 1;
			}

			int traceNumberSoma = nInject*numberOfRecordSites*numberOfPeriods+0*numberOfPeriods+nPeriod;
			int traceNumberDend1 = nInject*numberOfRecordSites*numberOfPeriods+1*numberOfPeriods+nPeriod;
			int traceNumberDend2 = nInject*numberOfRecordSites*numberOfPeriods+2*numberOfPeriods+nPeriod;

			results[traceNumberSoma].resetAndSetLength(periodStop[nPeriod] - periodStart[nPeriod] + 1);
			results[traceNumberDend1].resetAndSetLength(periodStop[nPeriod] - periodStart[nPeriod] + 1);
			results[traceNumberDend2].resetAndSetLength(periodStop[nPeriod] - periodStart[nPeriod] + 1);
		
			string name;
			results[traceNumberSoma].setName("Injection: " + str(nInject) + ", Period: " + str(nPeriod) + ", Soma, Filename: "+inputFileName);
			results[traceNumberDend1].setName("Injection: " + str(nInject) + ", Period: " + str(nPeriod) + ", Dend1, Filename: "+inputFileName);
			results[traceNumberDend2].setName("Injection: " + str(nInject) + ", Period: " + str(nPeriod) + ", Dend2, Filename: "+inputFileName);

			results[traceNumberSoma].setSamplingFrequency(50000);
			results[traceNumberDend1].setSamplingFrequency(50000);
			results[traceNumberDend2].setSamplingFrequency(50000);

			results[traceNumberSoma].setWeight(weights[nPeriod][0]);
			results[traceNumberDend1].setWeight(weights[nPeriod][1]);
			results[traceNumberDend2].setWeight(weights[nPeriod][1]);

			for (int j = 0; j < results[traceNumberSoma].getLength(); j++) {
				inputfile >> time;
				if (j==0) {
					results[traceNumberSoma].setStartTime(time);
                	results[traceNumberDend1].setStartTime(time);
                	results[traceNumberDend2].setStartTime(time);
				}		
				inputfile >> results[traceNumberSoma][j];
				inputfile >> results[traceNumberDend1][j];
				inputfile >> results[traceNumberDend2][j];
				nPoints = nPoints + 1;
			}

			results[traceNumberSoma].setStopTime(time);
            results[traceNumberDend1].setStopTime(time);
			results[traceNumberDend2].setStopTime(time);
		}

}
