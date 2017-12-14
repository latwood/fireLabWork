#ifndef READCONFIGFILE_H
#define READCONFIGFILE_H

#include <iostream>

#include <vector>

#include <smokeTransportOptions.h>
#include <configOption.h>

#include "usefulFunctions.h"  //includes <iostream> and <sys/stat.h>

using namespace std;

class readConfigFile
{
public:

    readConfigFile(std::vector<configOption> theOptions_value);

    void newConfigFile(std::string configFilePath_value);    //this is the redo of the constructor, so that the class can use one copy of itself over and over

    //something like overloaded functions for returning stuff of differing types. Build other types as needed
    std::string get_configFilePath();
    int get_optionValues_singleInt(std::string desiredOptionName);
    std::vector<int> get_optionValues_multiInt(std::string desiredOptionName);
    std::string get_optionValues_singleString(std::string desiredOptionName);
    std::vector<std::string> get_optionValues_multiString(std::string desiredOptionName);

private:

    void readFile();
    void processWords(std::vector<std::string> foundOptionNames, std::vector< std::vector< std::vector<std::string> > > foundOptionValues);
    void updateNumberOfValues(size_t theOptionNumber, bool fillStrings);
    void checkConflictingOptions();  //first check to see if any of the filled variables have conflicting options with other filled variables, then check the fill
    void checkVariableFill();

    std::string configFilePath;
    std::vector<configOption> theOptions;

    bool setupFail;

    usefulFunctions handy;
};

#endif // READCONFIGFILE_H
