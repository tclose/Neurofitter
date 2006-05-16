#include "../Evolufit.h"
#include "../MPIEvolufit.h"

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


int main (int argc, char* argv[]) {
	cout << "Houston, we have liftoff..." << endl;

		////////////////////////////
		///	Read parameters file ///
		////////////////////////////
		// Read data from file
		cout << "Arguments: {" << endl;
		for (int i = 0; i < argc; i++) {
			cout << string(argv[i]) << ",";
		}
		cout << "}" << endl;
		/*if (argc < 2 || argv[1]==NULL) crash("Evolufit","Not enough arguments: "+str(argc));
		ifstream paramFile(argv[1]);*/
		ifstream paramFile("/cluster/home/werner/EvolufitWork/Evolufit/bin/mpiparameters.xml");
		string fileContent = string(istreambuf_iterator<char>(paramFile),istreambuf_iterator<char>());
		FixedParameters fixedParameters = FixedParameters(XMLString("<root>"+fileContent+"</root>").getSubString("TestProgram"));

		// Say which parameters should be passed to child objects
		fixedParameters.setGlobal("Dimensions");
		fixedParameters.setGlobal("VerboseLevel");
		fixedParameters.setGlobal("SamplingFrequency");
		fixedParameters.setGlobal("Seed");
		fixedParameters.setGlobal("Bounds");

		///////////////////
		///	Run program ///
		///////////////////
		MPIEvolufitStarter starter(fixedParameters);
		starter.run(argc, argv);

	cout << endl << "And we have touchdown" << endl;

	return 0;
}
