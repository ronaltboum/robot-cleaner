#ifndef CommandLineInterpeter_h__
#define CommandLineInterpeter_h__

#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

/*
 * class CommandLineInterpeter is a "static" class with one function - 
 *	readCommandLineArguments for handling command line arguments.
 */
class CommandLineInterpeter
{
//~~~~~~~~~~~~~~~~~~~~` Macros ~~~~~~~~~~~~~~~~~~~~~~~~~`
public:
	typedef map<string,string>::iterator it_type;
	typedef map<string,string>::const_iterator const_it_type;
	#define THREAD_ATTRIBUTE string("-threads")
	#define IsThreadAttribute(s) (! THREAD_ATTRIBUTE.compare((s)))
	enum class CommandLineProblems {OddCLA, UnknownCLA, ThreadIsntNum, NoProblem};	

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	static map<string,string> CLAvalues; // map of all the CLA names to their values


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	CommandLineInterpeter(void){}
	~CommandLineInterpeter(void){}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:

	// initialize the default parameters value
	static map<string,string> initMap(){
		map<string,string> CLAs;
		CLAs["-config"]		 = "config.ini";
		CLAs["-house_path"] 	 = "./";
		CLAs["-algorithm_path"] = "./";
		CLAs["-score_formula"]  = "score_formula.so";
		CLAs[THREAD_ATTRIBUTE] = "1";
		return CLAs;
	}

	// brief: read command line arguments, and returns if there was a problem
	// post: CLAvalues
	static CommandLineProblems readCommandLineArguments(int argc, const char * argv[]);
	static CommandLineProblems ReadAttributeValuePair(const char * attributeName, const char * attributeValue);

	// returns the CLA values
	static const map<string,string> & getCLAvalues(){
		return CLAvalues;
	}

	static const string & getConfigFile() {
		return CLAvalues["-config"];
	}

	static const string & getHousePath() {
		return CLAvalues["-house_path"] ;
	}

	static const string & getAlgorithmPath() {
		return CLAvalues["-algorithm_path"];
	}

	static const string & getScoreFormulaFile(){
		return CLAvalues["-score_formula"];
	}

	static int getThreads(){
		char* p;
		long converted = strtol(CLAvalues[THREAD_ATTRIBUTE].c_str(), &p, 10);
		if (*p) {
			return -1;
		}
		return (int) converted ;
	}
};

#endif // CommandLineInterpeter_h__
