#include "CommandLineInterpeter.h"

map<string,string> CommandLineInterpeter::CLAvalues = CommandLineInterpeter::initMap();

// brief: get an attribute name and value and check that is is a valid arguments
// returns: CommandLineProblems where:
//				NoProblem - no problem occured
//				UnknownCLA - CLA isn't a valid VLA for the program
//				ThreadIsntNum - the attributeName is for threads number, but attributeValue isn't a num
CommandLineInterpeter::CommandLineProblems 
CommandLineInterpeter::ReadAttributeValuePair(const char * attributeName, const char * attributeValue)
{
	for(it_type iterator = CLAvalues.begin(); iterator != CLAvalues.end(); iterator++) {
		string sAttributeName = iterator->first;
		string sAttributeValue = iterator->second;
		if( sAttributeName == attributeName)
		{
			CLAvalues[sAttributeName] = string(attributeValue);
			return CommandLineProblems::NoProblem;	
		}
	}
	return CommandLineProblems::UnknownCLA;
}

void CommandLineInterpeter::ValidateScoreFile (){
	string formulaPath = CLAvalues["-score_formula"];
	string suffix = "score_formula.so";

	if (formulaPath.find(suffix) != string::npos)
		return;
	bool endsWithSlash = (formulaPath[formulaPath.length() - 1] == '/');
	formulaPath = formulaPath + (endsWithSlash ? "" : "/") + suffix;
	CLAvalues["-score_formula"] = formulaPath;

}

// brief: read all the CLAs and update their values
// returns CommandLineProblems where:
//				NoProblem - no problem occured
//				UnknownCLA - CLA isn't a valid VLA for the program
//				ThreadIsntNum - the attributeName is for threads number, but attributeValue isn't a num
// @post: CLAvalues
CommandLineInterpeter::CommandLineProblems CommandLineInterpeter::readCommandLineArguments(int argc, const char * argv[]){
	if(argc%2 != 1)
		return CommandLineProblems::OddCLA;
	//reading each pair of argument-value
	CommandLineProblems problemOccored = CommandLineProblems::NoProblem;
	for(int i=1 ; (i < argc) && (problemOccored == CommandLineProblems::NoProblem) ; i+=2)
	{
		problemOccored = ReadAttributeValuePair(argv[i], argv[i+1]);
	}
	//check if thread is a number
	char* p;
	/* long converted = */ strtol(CLAvalues["-threads"].c_str(), &p, 10);
	if (*p) {
	    return CommandLineProblems::ThreadIsntNum;
	}
	ValidateScoreFile();

	return problemOccored;
}
