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
		if( ! sAttributeName.compare(attributeName))
		{
			//attribute name was found;
			if(IsThreadAttribute(sAttributeName))
			{
				//check if number
				char* p;
				/* long converted = */ strtol(attributeValue, &p, 10);
				if (*p) {
				    return CommandLineProblems::ThreadIsntNum;
				}
			}
			CLAvalues[sAttributeName] = string(attributeValue);
			return CommandLineProblems::NoProblem;	
		}
	}
	return CommandLineProblems::UnknownCLA;
}

// brief: read all the CLAs and update their values
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
	return problemOccored;
}
