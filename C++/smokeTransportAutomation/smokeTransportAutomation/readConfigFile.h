#ifndef READCONFIGFILE_H
#define READCONFIGFILE_H

#include <iostream>

#include <vector>
#include <istream>
#include <fstream>
#include <sstream>

#include <smokeTransportOptions.h>
#include <configOption.h>

using namespace std;

class readConfigFile
{
public:

    readConfigFile(std::vector<configOption> theOptions_value);

    void newConfigFile(std::string configFilePath_value);    //this is the redo of the constructor, but can be done without the constructor

    void readFile();    //returns words
    void processWords();
    void updateNumberOfValues(size_t theOptionNumber);
    void emptyWords();
    void updateOption_configFilePath();
    void checkConflictingOptions();  //first check to see if any of the filled variables have conflicting options with other filled variables, then check the fill
    void checkVariableFill();


    //kay overloaded options for returning stuff of differing types. Build other types as needed
    int get_optionValues_singleInt(std::string desiredOptionName);
    std::vector<int> get_optionValues_multiInt(std::string desiredOptionName);
    std::string get_optionValues_singleString(std::string desiredOptionName);
    std::vector<std::string> get_optionValues_multiString(std::string desiredOptionName);

    int string_to_int(std::string s);
    std::string int_to_string(int n);

private:

    std::string configFilePath;

    std::vector<configOption> theOptions;

    std::vector<std::string> foundWords;
    std::vector<int> vectorStartIndex;  // basically the idea is to catch where the word is specifically a { to know when a vector starts
    std::vector<int> vectorEndIndex;  // basically the idea is to catch where the word is specifically a } to know when a vector ends

    std::string parentDirectory;
    std::string startCase;
    std::string initialTime;
    int numberOfSteps = 0;
    std::vector<std::string> endTimes;
    std::vector<std::string> writeIntervals;
    std::string sourceMinCoordinates;
    std::string sourceMaxCoordinates;
    std::vector<std::string> sourceValues;
    std::vector<std::string> UfileLocations;
    int missingTimes = 0;
    std::vector<std::string> missingTimeDirectories;

    bool setupFail;
    bool conversionFail;

    void message(std::string theMessage);

};

#endif // READCONFIGFILE_H
