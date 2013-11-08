/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#include "../EOFitterInterface.h"

//#include <eo>
#include <es/make_es.h>

FitterResults EOFitterInterface::runFitter(ModelTuningParameters * startPoint) {
	
	modelParams = startPoint;	
	FitterResults results;
	
	writeEOParamsFile("EOparam", *startPoint);

	char *argv[] = {"EOFitterInterface","@EOparam"};
	
	eoParser parser(2, argv);  // for user-parameter reading

  	eoState state;    // keeps all things allocated

  	eoValueParam<bool>& simpleParam = parser.createParam(true, "Isotropic", "Isotropic self-adaptive mutation", 'i', "ES mutation");
  	eoValueParam<bool>& stdevsParam = parser.createParam(false, "Stdev", "One self-adaptive stDev per variable", 's', "ES mutation");
  	eoValueParam<bool>& corrParam = parser.createParam(false, "Correl", "Use correlated mutations", 'c', "ES mutation");

    // Run the appropriate algorithm
    if (simpleParam.value() == false) {
      showMessage("Using eoReal\n",2,fixedParams);
      runAlgorithm(eoReal<eoMinimizingFitness>(), parser, state);
    }
    else if (stdevsParam.value() == false) {
      showMessage("Using eoEsSimple\n",2,fixedParams);
      runAlgorithm(eoEsSimple<eoMinimizingFitness>(), parser, state);
    }
    else if (corrParam.value() == false) {
      showMessage("Using eoEsStdev\n",2,fixedParams);
      runAlgorithm(eoEsStdev<eoMinimizingFitness>(), parser, state);
    }
    else {
      	showMessage("Using eoEsFull\n",2,fixedParams);
      	eoEsFull<eoMinimizingFitness> eoResult = runAlgorithm(eoEsFull<eoMinimizingFitness>(), parser, state);
		
		stringbuf * buf = new stringbuf();		
		iostream paramStream(buf);
		eoResult.printOn(paramStream);
      	showMessage("Start: \n",2,fixedParams);
		eoResult.printOn(cout);
      	showMessage("Stop \n",2,fixedParams);

		ModelTuningParameters params(*startPoint);
		double errorValue;
		double tmp;
		paramStream >> errorValue; paramStream >> tmp;		
		for (int i = 0; i < startPoint->getLength(); i++) {
			paramStream >> params[i];
		}
		results.setBestFit(params,errorValue);
		delete buf;
    }
 
	return results;

}

static double real_value(const std::vector<double>& paramVector, ErrorValueCalculator * errorValue) {

	ModelTuningParameters params(paramVector.size());

	for (unsigned int i = 0; i < paramVector.size(); i++) {
		params[i] = paramVector[i];
	}

	errorValue->calculateErrorValue(params);

	return params.getErrorValue();
	
}



/** The templatized main (sort of)
 *  quite similar to the main of other genotypes
 *  (e.g. t-eoReal and t-eoGA in test dir)
 */
template <class EOT>
EOT EOFitterInterface::runAlgorithm(EOT, eoParser& _parser, eoState& _state) {

	// The evaluation fn - encapsulated into an eval counter for output
	EOFitness<EOT, double, const std::vector<double>&> mainEval( &real_value, errorValue );
	eoEvalFuncCounter<EOT> eval(mainEval);

	// the genotype - through a genotype initializer
	eoRealInitBounded<EOT>& init = make_genotype(_parser, _state, EOT());

	// Build the variation operator (any seq/prop construct)
	eoGenOp<EOT>& op = make_op(_parser, _state, init);


    // initialize the population - and evaluate
    // yes, this is representation indepedent once you have an eoInit
	eoPop<EOT>& pop = make_pop(_parser, _state, init);
    //apply<EOT>(eval, pop);

    // stopping criteria
    eoContinue<EOT> & term = make_continue(_parser, _state, eval);
    // output
    eoCheckPoint<EOT> & checkpoint = make_checkpoint(_parser, _state, eval, term);
    // algorithm (need the operator!)

    // Enabling the distributed evaluation of the population 
    EODistFitness<EOT> pop_eval(errorValue);

    eoAlgo<EOT>& ga = make_algo_scalar(_parser, _state, pop_eval, checkpoint, op);

	run_ea(ga, pop); // run the ga

    showMessage("Final Population\n",2,fixedParams);
	pop.sortedPrintOn(cout);
    showMessage("\n",2,fixedParams);

	return pop.best_element();

}

extern string EOParamsContent;

void EOFitterInterface::writeEOParamsFile(string fileName, ModelTuningParameters & startPoint) {

	ofstream EOParam(fileName.c_str(), ios::out);

	EOParam << EOParamsContent;

	EOParam << endl;

	EOParam << endl << "	--popSize=" << fixedParams["popSize"] << endl;
	EOParam << endl << "	--nbOffspring=" << fixedParams["nbOffspring"] << endl;
	EOParam << endl << "	--replacement=" << fixedParams["replacement"] << endl;
	EOParam << endl << "	--maxGen=" << fixedParams["maxGen"] << endl;
	EOParam << endl << "	--minGen=" << fixedParams["minGen"] << endl;
	EOParam << endl << "	--maxEval=" << fixedParams["maxEval"] << endl;
	EOParam << endl << "	--targetFitness=" << fixedParams["targetFitness"] << endl;
	EOParam << endl << "	--steadyGen=" << fixedParams["steadyGen"] << endl;
	EOParam << endl << "	--crossType=" << fixedParams["crossType"] << endl;
	EOParam << endl << "	--crossObj=" << fixedParams["crossObj"] << endl;
	EOParam << endl << "	--TauLoc=" << fixedParams["TauLoc"] << endl;
	EOParam << endl << "	--TauGlob=" << fixedParams["TauGlob"] << endl;
	EOParam << endl << "	--Beta=" << fixedParams["Beta"] << endl;

	if (toInt(fixedParams["VerboseLevel"]) > 2) {
		EOParam << endl << "	--printBestStat=1" << endl;
	}
	else {
		EOParam << endl << "	--printBestStat=0" << endl;	
	}

	if (toInt(fixedParams["VerboseLevel"]) > 4) {
		EOParam << endl << "	--printPop=1" << endl;
	}
	else {
		EOParam << endl << "	--printPop=0" << endl;
	}


	EOParam << endl << "	--seed=" << fixedParams["Seed"] << endl;

	EOParam << endl << "	--vecSize=" << fixedParams["Dimensions"] << endl;

	EOParam << endl << "	--initBounds=";
	for (int i = 0; i < toInt(fixedParams["Dimensions"]); i++) {
		EOParam << "[" << startPoint.getLowerBound(i) << "," << startPoint.getUpperBound(i)  << "];";
	}
	EOParam << endl;

	EOParam << endl << "	--objectBounds=";
	for (int i = 0; i < toInt(fixedParams["Dimensions"]); i++) {
		EOParam << "[" << startPoint.getLowerBound(i) << "," << startPoint.getUpperBound(i)  << "];";
	}
	EOParam << endl;

	EOParam.close();

}

string EOParamsContent = " \
	 --help=0                                 # -h : Prints this message \n\
	 --stopOnUnknownParam=1                   # Stop if unkown param entered \n\
 	 # read from main Neurofitter parameter file --seed=1098191371                        # -S : Random number seed \n\
\n\
	 --Isotropic=1                            # -i : Isotropic self-adaptive mutation \n\
 	 --Stdev=1                                # -s : One self-adaptive stDev per variable \n\
 	 --Correl=1                               # -c : Use correlated mutations \n\
\n\
	# read from main Neurofitter parameter file --popSize=50                             # -P : Population Size \n\
 	--selection=DetTour(5)                   # -S : Selection: Roulette, Ranking(p,e), DetTour(T), StochTour(t) or Sequential(ordered/unordered) \n\
 	# read from main Neurofitter parameter file--nbOffspring=10                       # -O : Nb of offspring (percentage or absolute) \n\
 	# read from main Neurofitter parameter file--replacement=Plus                       # -R : Replacement: Comma, Plus or EPTour(T), SSGAWorst, SSGADet(T), SSGAStoch(t) \n\
 	--weakElitism=0                          # -w : Old best parent replaces new worst offspring *if necessary* \n\
\n\
	# auto filled in --vecSize=4                             # -n : The number of variables \n\
	# auto filled in --initBounds=[1,100000];[1,100000];[1,100000];[1,100000]# -B : Bounds for initialization (MUST be bounded) \n\
 	--sigmaInit=10                          # -s : Initial value for Sigma(s) \n\
\n\
	--useEval=1                              # Use nb of eval. as counter (vs nb of gen.) \n\
 	--useTime=1                              # Display time (s) every generation \n\
 	# read from main Neurofitter parameter file --printBestStat=1                        # Print Best/avg/stdev every gen. \n\
 	# read from main Neurofitter parameter file --printPop=0                             # Print sorted pop. every gen. \n\
\n\
 	--resDir=ResNew                             # Directory to store DISK outputs \n\
 	--eraseDir=1                             # erase files in dirName if any \n\
 	--fileBestStat=1                         # Output bes/avg/std to file \n\
\n\
	--plotBestStat=0                         # Plot Best/avg Stat \n\
 	--plotHisto=0                            # Plot histogram of fitnesses \n\
\n\
 	--Load=                                  # -L : A save file to restart from \n\
 	--recomputeFitness=0                     # -r : Recompute the fitness after re-loading the pop.? \n\
 	--saveFrequency=1                        # Save every F generation (0 = only final state, absent = never) \n\
 	--saveTimeInterval=0                     # Save every T seconds (0 or absent = never) \n\
 	--status=./ESEA.status                   # Status file \n\
\n\
 	# read from main Neurofitter parameter file --maxGen=50                               # -G : Maximum number of generations () = none) \n\
 	# read from main Neurofitter parameter file --steadyGen=100                            # -s : Number of generations with no improvement \n\
 	# read from main Neurofitter parameter file --minGen=0                               # -g : Minimum number of generations \n\
 	# read from main Neurofitter parameter file --maxEval=300                              # -E : Maximum number of evaluations (0 = none) \n\
 	# read from main Neurofitter parameter file --targetFitness=0.0                        # -T : Stop when fitness reaches \n\
 	--CtrlC=1                                # -C : Terminate current generation upon Ctrl C \n\
\n\
 	# auto filled in --objectBounds=[1,10000];[1,10000];[1,10000];[1,10000]                  # -B : Bounds for variables \n\
 	--operator=SGA                           # -o : Description of the operator (SGA only now) \n\
 	--pCross=1                               # -C : Probability of Crossover \n\
 	--pMut=1                                 # -M : Probability of Mutation \n\
 	# read from main Neurofitter parameter file --crossType=global                       # -C : Type of ES recombination (global or standard) \n\
 	# read from main Neurofitter parameter file --crossObj=intermediate                      # -O : Recombination of object variables (discrete, intermediate or none) \n\
 	--crossStdev=intermediate                # -S : Recombination of mutation strategy parameters (intermediate, discrete or none) \n\
 	# read from main Neurofitter parameter file --TauLoc=1                               # -l : Local Tau (before normalization) \n\
 	# read from main Neurofitter parameter file --TauGlob=1                              # -g : Global Tau (before normalization) \n\
 	# read from main Neurofitter parameter file --Beta=0.0873                            # -b : Beta \n\
";




/*/// THIS IS USED IF YOU DON'T WANT PARALLEL EVALUATION WVG
  //// Now the representation-independent things
  //////////////////////////////////////////////

  // initialize the population - and evaluate
  // yes, this is representation indepedent once you have an eoInit
  eoPop<EOT>& pop   = make_pop(_parser, _state, init);
  apply<EOT>(eval, pop);

  // stopping criteria
  eoContinue<EOT> & term = make_continue(_parser, _state, eval);
  // output
  eoCheckPoint<EOT> & checkpoint = make_checkpoint(_parser, _state, eval, term);
  // algorithm (need the operator!)
  eoAlgo<EOT>& ga = make_algo_scalar(_parser, _state, eval, checkpoint, op);

*/

