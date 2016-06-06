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
	enum class CommandLineProblems {UnknownCLA, ThreadIsntNum, NoProblem};	

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	static map<string,string> CLAvalues; // map of all the CLA names to their values
	static bool _isVideo;   //becomes true if -video is one of the command line arguments

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	CommandLineInterpeter(void){}
	~CommandLineInterpeter(void){}
	static void ValidateScoreFile ();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:

	// initialize the default parameters value
	static map<string,string> initMap(){
		map<string,string> CLAs;
		CLAs[string("-config")]		 = string("config.ini");
		CLAs[string("-house_path")] 	 = string("./");
		CLAs[string("-algorithm_path")] = string("./");
		CLAs[string("-score_formula")]  = string("./score_formula.so");
		CLAs[string("-threads")] = string("1");
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
		long converted = strtol(CLAvalues["-threads"].c_str(), &p, 10);
		if (*p) {
			return -1;
		}
		return (int) converted ;
	}

	static bool getIsVideo(){
		return _isVideo;
	}
};

#endif // CommandLineInterpeter_h__
