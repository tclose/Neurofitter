#include <map>
#include <string>

///todo add this to every header file
using namespace std;

typedef map< string, string, less< string > > stringmap;

class FixedParameters {

	string& operator[](const string index) {return params[index];};

	void readFromXML(string xmlString);

	stringmap params;

};
