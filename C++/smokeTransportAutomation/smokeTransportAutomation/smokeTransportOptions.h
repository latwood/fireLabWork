#ifndef SMOKETRANSPORTOPTIONS_H
#define SMOKETRANSPORTOPTIONS_H

#include <vector>

#include <configOption.h>

#include "usefulFunctions.h"  //includes <iostream> and <sys/stat.h>

class smokeTransportOptions
{

public:

    //I want to say that this class only needs to manipulate/create the initial options
    //then readConfigFile uses the options to set the final option stuff
    //then smokeTransportRun uses the options to do the run

    //constructor, which sets up the available options
    // also checks to make sure the available options were set correctly
    smokeTransportOptions();

    //get value functions
    std::vector<configOption> get_theOptions();


private:

    void addOption(std::string newOptionName,std::string newOptionDataType,std::string newOptionNumberOfValues,std::vector<std::string> newConflictingOtions); //might make this private so that it is only called by constructor
    void setupAvailableOptions();

    //error handling functions
    bool check_duplicateOptions();
    bool check_originalNumberOfValues();
    bool check_conflictingOptions();

    std::vector<configOption> theOptions;

    usefulFunctions handy;
};

#endif // SMOKETRANSPORTOPTIONS_H
