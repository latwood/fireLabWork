#ifndef CONFIGOPTION_H
#define CONFIGOPTION_H

#include <iostream>
#include <vector>

#include <sstream>
#include <cstdlib>

using namespace std;

class configOption
{

public:

    //this should probably be actually called configOption
    configOption(std::string optionName_value,std::string optionDataType_value,
                  std::string optionNumberOfValues_value,std::vector<std::string> conflictingOptions_value);
    void resetOption();     //kind of a reusable constructor/destructor

    //error handling for new values as they come in, to be used by constructor and value updater functions
    bool check_optionDataType(std::string newOptionDataType);
    bool check_optionNumberOfValues(std::string newOptionNumberOfValues);   //assumes the numberOfValues will be turned into a size_t or an int, not sure which is best yet.
    bool check_optionValue(std::string newOptionValue);

    //update some values of the options
    void updateNumberOfValues(size_t newNumberOfValues);
    void addOptionValue(std::string newOptionValue);
    void updateOptionConflicts(bool newOptionConflictsValue);

    //get values from the options (for passing on to other stuff)
    std::string get_optionName();
    std::string get_optionDataType();
    std::string get_optionOriginalNumberOfValues();
    std::vector<std::string> get_conflictingOptions();
    size_t get_optionCurrentNumberOfValues();
    std::vector<std::string> get_optionValues();
    bool get_optionConflicts();

private:

    std::string optionName;
    std::string optionDataType;
    std::string optionOriginalNumberOfValues;
    std::vector<std::string> conflictingOptions;    //basically fill this with the variable names of other options that can't go along with.
    size_t optionCurrentNumberOfValues;
    std::vector<std::string> optionValues;
    bool optionConflicts;

    void message(std::string theMessage);

};

#endif // CONFIGOPTION_H
