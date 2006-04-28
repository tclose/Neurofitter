#include "../EOFitterInterface.h"

#include <eo>
#include <es/make_es.h>

FitterResults EOFitterInterface::runFitter(ModelTuningParameters * startPoint, int seed) {
	
	modelParams = startPoint;	
	FitterResults results;
	

	writeEOParamsFile("EOparam", *startPoint, seed);

	char *argv[] = {"EOFitterInterface","@EOparam"};
	
	eoParser parser(2, argv);  // for user-parameter reading

  	eoState state;    // keeps all things allocated

  	eoValueParam<bool>& simpleParam = parser.createParam(true, "Isotropic", "Isotropic self-adaptive mutation", 'i', "ES mutation");
  	eoValueParam<bool>& stdevsParam = parser.createParam(false, "Stdev", "One self-adaptive stDev per variable", 's', "ES mutation");
  	eoValueParam<bool>& corrParam = parser.createParam(false, "Correl", "Use correlated mutations", 'c', "ES mutation");

    // Run the appropriate algorithm
    if (simpleParam.value() == false)
    {
      cout << "Using eoReal" << endl;
      runAlgorithm(eoReal<eoMinimizingFitness>(), parser, state);
    }
    else if (stdevsParam.value() == false)
    {
      cout << "Using eoEsSimple" << endl;
      runAlgorithm(eoEsSimple<eoMinimizingFitness>(), parser, state);
    }
    else if (corrParam.value() == false)
    {
      cout << "Using eoEsStdev" << endl;
      runAlgorithm(eoEsStdev<eoMinimizingFitness>(), parser, state);
    }
    else {
		cout << "Using eoEsFull" << endl;
      	eoEsFull<eoMinimizingFitness> eoResult = runAlgorithm(eoEsFull<eoMinimizingFitness>(), parser, state);
		

		stringbuf * buf = new stringbuf();		
		iostream paramStream(buf);
		eoResult.printOn(paramStream);

		ModelTuningParameters params(*startPoint);
		double tmp;
		paramStream >> tmp; paramStream >> tmp;		
		for (int i = 0; i < startPoint->getLength(); i++) {
			paramStream >> params[i];
		}
		results.setBestFit(params);
		delete buf;
    }

	return results;
}

static double real_value(const std::vector<double>& paramVector, FitnessCalculator * fitness) {

	ModelTuningParameters params(paramVector.size());

	for (unsigned int i = 0; i < paramVector.size(); i++) {
		params[i] = paramVector[i];
	}

	return fitness->calculateFitness(params);
	

}



/** The templatized main (sort of)
 *  quite similar to the main of other genotypes
 *  (e.g. t-eoReal and t-eoGA in test dir)
 */
template <class EOT>
EOT EOFitterInterface::runAlgorithm(EOT, eoParser& _parser, eoState& _state)
{
	typedef typename EOT::Fitness FitT;

  ///// FIRST, problem or representation dependent stuff
  //////////////////////////////////////////////////////

  // The evaluation fn - encapsulated into an eval counter for output
  EOFitnessCalculator<EOT, double, const std::vector<double>&>
               mainEval( &real_value, fitness );
  eoEvalFuncCounter<EOT> eval(mainEval);

  // the genotype - through a genotype initializer
  eoRealInitBounded<EOT>& init = make_genotype(_parser, _state, EOT());

  // Build the variation operator (any seq/prop construct)
  eoGenOp<EOT>& op = make_op(_parser, _state, init);

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

  ///// End of construction of the algorith
  /////////////////////////////////////////
  // to be called AFTER all parameters have been read!!!
  	make_help(_parser);
  //// GO
  ///////

  cout << "Initial Population\n";
  pop.sortedPrintOn(cout);
  cout << endl;

  run_ea(ga, pop); // run the ga

  cout << "Final Population\n";
  pop.sortedPrintOn(cout);
  cout << endl;

  return pop.best_element();

}

extern string EOParamsContent;

void EOFitterInterface::writeEOParamsFile(string fileName, ModelTuningParameters & startPoint) {

	ofstream EOParam;	
	EOParam.open(fileName.c_str(), ios::out);

	EOParam << endl << "	--popSize=" << fixedParams["popSize"] << endl;
	EOParam << endl << "	--nbOffspring=" << fixedParams["nbOffspring"] << endl;
	EOParam << endl << "	--replacement=" << fixedParams["replacement"] << endl;
	EOParam << endl << "	--maxGen=" << fixedParams["maxGen"] << endl;
	EOParam << endl << "	--minGen=" << fixedParams["minGen"] << endl;
	EOParam << endl << "	--targetFitness=" << fixedParams["targetFitness"] << endl;
	EOParam << endl << "	--steadyGen=" << fixedParams["steadyGem"] << endl;
	EOParam << endl << "	--crossType=" << fixedParams["crossType"] << endl;
	EOParam << endl << "	--corssObj=" << fixedParams["crossObj"] << endl;
	EOParam << endl << "	--TauLoc=" << fixedParams["TauLoc"] << endl;
	EOParam << endl << "	--TauGlob=" << fixedParams["TauGlob"] << endl;

	if (fixedParams["VerboseLevel"] > 2) {
		EOParam << endl << "	--printBestStat=1" << endl;
	}
	else {
		EOParam << endl << "	--printBestStat=0" << endl;	
	}

	if (fixedParams["VerboseLevel"] > 4) {
		EOParam << endl << "	--printPop=1" << endl;
	}
	else {
		EOParam << endl << "	--printPop=0" << endl;
	}


	///todo check to see if floating point seed is possible
	EOParam << endl << "	--seed=" << fixedParams["Seed"] << endl;

	EOParam << endl << "	--vecSize=" << startPoint.getLength() << endl;

	EOParam << endl << "	--initBounds=";
	for (int i = 0; i < startPoint.getLength(); i++) {
		EOParam << "[" << startPoint.getLowerBound(i) << "," << startPoint.getUpperBound(i)  << "];";
	}
	EOParam << endl;

	EOParam << endl << "	--objectBounds=";
	for (int i = 0; i < startPoint.getLength(); i++) {
		EOParam << "[" << startPoint.getLowerBound(i) << "," << startPoint.getUpperBound(i)  << "];";
	}
	EOParam << endl;

	EOParam << EOParamsContent;

	EOParam.close();

}

string EOParamsContent = " \
	 --help=0                                 # -h : Prints this message \n\
	 --stopOnUnknownParam=1                   # Stop if unkown param entered \n\
 	 # read from main Evolufit parameter file --seed=1098191371                        # -S : Random number seed \n\
\n\
	 --Isotropic=1                            # -i : Isotropic self-adaptive mutation \n\
 	 --Stdev=1                                # -s : One self-adaptive stDev per variable \n\
 	 --Correl=1                               # -c : Use correlated mutations \n\
\n\
	# read from main Evolufit parameter file --popSize=50                             # -P : Population Size \n\
 	--selection=DetTour(5)                   # -S : Selection: Roulette, Ranking(p,e), DetTour(T), StochTour(t) or Sequential(ordered/unordered) \n\
 	# read from main Evolufit parameter file--nbOffspring=10                       # -O : Nb of offspring (percentage or absolute) \n\
 	# read from main Evolufit parameter file--replacement=Plus                       # -R : Replacement: Comma, Plus or EPTour(T), SSGAWorst, SSGADet(T), SSGAStoch(t) \n\
 	--weakElitism=0                          # -w : Old best parent replaces new worst offspring *if necessary* \n\
\n\
	# auto filled in --vecSize=4                             # -n : The number of variables \n\
	# auto filled in --initBounds=[1,100000];[1,100000];[1,100000];[1,100000]                       # -B : Bounds for initialization (MUST be bounded) \
 	--sigmaInit=10                          # -s : Initial value for Sigma(s) \n\
\n\
	--useEval=1                              # Use nb of eval. as counter (vs nb of gen.) \n\
 	--useTime=1                              # Display time (s) every generation \n\
 	# read from main Evolufit parameter file --printBestStat=1                        # Print Best/avg/stdev every gen. \n\
 	# read from main Evolufit parameter file --printPop=0                             # Print sorted pop. every gen. \n\
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
 	--maxGen=50                               # -G : Maximum number of generations () = none) \n\
 	--steadyGen=100                            # -s : Number of generations with no improvement \n\
 	--minGen=0                               # -g : Minimum number of generations \n\
 	--maxEval=300                              # -E : Maximum number of evaluations (0 = none) \n\
 	--targetFitness=0.0                        # -T : Stop when fitness reaches \n\
 	--CtrlC=1                                # -C : Terminate current generation upon Ctrl C \n\
\n\
 	# auto filled in --objectBounds=[1,10000];[1,10000];[1,10000];[1,10000]                  # -B : Bounds for variables \n\
 	--operator=SGA                           # -o : Description of the operator (SGA only now) \n\
 	--pCross=1                               # -C : Probability of Crossover \n\
 	--pMut=1                                 # -M : Probability of Mutation \n\
 	--crossType=global                       # -C : Type of ES recombination (global or standard) \n\
 	--crossObj=intermediate                      # -O : Recombination of object variables (discrete, intermediate or none) \n\
 	--crossStdev=intermediate                # -S : Recombination of mutation strategy parameters (intermediate, discrete or none) \n\
 	--TauLoc=1                               # -l : Local Tau (before normalization) \n\
 	--TauGlob=1                              # -g : Global Tau (before normalization) \n\
 	--Beta=0.0873                            # -b : Beta \n\
";

