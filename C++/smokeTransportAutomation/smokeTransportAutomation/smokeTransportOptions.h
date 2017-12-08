#ifndef SMOKETRANSPORTOPTIONS_H
#define SMOKETRANSPORTOPTIONS_H

#include <vector>

#include <configOption.h>

#include "usefulFunctions.h"  //includes <iostream> and <sys/stat.h>

class smokeTransportOptions
{

public:

    //constructor, which sets up the available options
    // also checks to make sure the available options were set correctly
    smokeTransportOptions();

    void addOption(std::string newOptionName,std::string newOptionDataType,std::string newOptionNumberOfValues,std::vector<std::string> newConflictingOtions); //might make this private so that it is only called by constructor
    void setupAvailableOptions();

    //error handling functions
    bool check_duplicateOptions();
    bool check_OriginalNumberOfValues();
    bool check_ConflictingOptions();

    //updating values functions
    //don't want to do this here or in configOption. Need to do this inside of readConfigFile since that will use this stuff to hold option names, number of option values, and option values

    //get value functions
    std::vector<configOption> get_theOptions();
    //not sure what to do here, I want to say that this class needs to just manipulate the initial options and say readConfigFile uses them to set other stuff, just on initialization or as a check on updating values and stuff. I guess you have to pull out the options from here to do any manipulation


private:

    std::vector<configOption> theOptions;

    usefulFunctions handy;
};

#endif // SMOKETRANSPORTOPTIONS_H
