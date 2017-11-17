#include "smokeTransportOptions.h"


//constructor related functions
smokeTransportOptions::smokeTransportOptions()
{
    message("\nSetting up available smokeTransportOptions");    // extra line at the beginning because this is a new set of functions/operations
    setupAvailableOptions();
    if(check_duplicateOptions() == false)
    {
        exitMessage("duplicate option found during setup!");
    }
    if(check_OriginalNumberOfValues() == false)
    {
        exitMessage("invalid numberOfValues found during setup!");
    }
}

void smokeTransportOptions::addOption(std::string newOptionName,std::string newOptionDataType,std::string newOptionNumberOfValues,std::vector<std::string> newConflictingOtions)
{
    theOptions.push_back(configOption(newOptionName,newOptionDataType,newOptionNumberOfValues,newConflictingOtions));
}

void smokeTransportOptions::setupAvailableOptions()
{
    //here is where each and every possible option is specified
    addOption("configFilePath","std::string","1",{"NA"});
    addOption("parentDirectory","std::string","1",{"NA"});
    addOption("startCase","std::string","1",{"NA"});
    addOption("initialTime","std::string","1",{"NA"});
    addOption("numberOfSteps","int","1",{"NA"});
    addOption("endTimes","std::string","numberOfSteps",{"NA"});
    addOption("writeIntervals","std::string","numberOfSteps",{"NA"});
    //addOption("sourceTypes","std::string","1",{"NA"});
    addOption("sourceMinCoordinates","std::string","1",{"NA"});
    addOption("sourceMaxCoordinates","std::string","1",{"NA"});
    addOption("sourceValues","std::string","numberOfSteps",{"NA"});
    addOption("UfileLocations","std::string","numberOfSteps",{"NA"});
    addOption("missingTimes","std::string","1",{"NA"});
    addOption("missingTimeDirectories","std::string","missingTimes",{"NA"});

    /*
    addOption("numberOfSources","int","2");
    addOption("sourceTypes","std::string","numberOfSources",{"NA","NA"});
    addOption("sourceMinCoordinates","std::string","numberOfSources",{"box);
    addOption("sourceMaxCoordinates","std::string","numberOfSources");
    addOption("sourceValues","std::vector<std::string>","numberOfSteps,numberOfSources");//this just made life a whole lot more complicated. So use the vector type to make it realize it needs to look at the vector elements differently, or straight up split it up into vectors?
    */

}


//error handling functions
bool smokeTransportOptions::check_duplicateOptions()
{
    for(size_t i = 0; i < theOptions.size()-1; i++)
    {
        for(size_t j = i+1; j < theOptions.size(); j++)
        {
            if(theOptions[i].get_optionName() == theOptions[j].get_optionName())
            {
                message("Duplicate option: " + theOptions[i].get_optionName() + "!");
                return false;
            }
        }
    }
    return true;
}

bool smokeTransportOptions::check_OriginalNumberOfValues()
{
    std::string theNumberOfValues;
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        istringstream strm;
        theNumberOfValues = theOptions[i].get_optionOriginalNumberOfValues();
        strm.str(theNumberOfValues);
        size_t n = 0;
        if((strm >> n).fail())
        {
            strm.clear();
            bool foundName = false;
            std::string theOptionName;
            for(size_t j = 0; j < theOptions.size(); j++)
            {
                theOptionName = theOptions[j].get_optionName();
                if(theNumberOfValues == theOptionName)
                {
                   foundName = true;
                }
                if(j == theOptions.size()-1 && foundName == false)
                {
                    message("invalid numberOfValues for option " + theOptions[i].get_optionName() + "!");
                    return false;
                }
            }
        }
    }
    return true;
}

//need to work some more on this. Still doesn't handle vector inside the option.
//Another loop for extra vector somehow. Check the right thing (conflict option) with right thing (option name)
//but return the right stuff in the right way. Right now exits on a good thing if it were to work.
bool smokeTransportOptions::check_ConflictingOptions()//probably need this in readConfig, not here, except this is just making sure each conflicting option really does exist as a variable name
{
    bool conflictingOptionsPass = true;
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        std::vector<std::string> currentConflictingOptions = theOptions[i].get_conflictingOptions();
        for(size_t k = 0; k < currentConflictingOptions.size(); k++)
        {
            if(currentConflictingOptions[k] != "NA")
            {
                if(currentConflictingOptions[k] == theOptions[i].get_optionName())
                {
                    message("Conflicting option: " + currentConflictingOptions[k] + " specified the same as it's own option!");
                    conflictingOptionsPass = false;
                } else
                {
                    bool foundOptionName = false;
                    for(size_t j = 0; j < theOptions.size(); j++)
                    {
                        if(currentConflictingOptions[k] == theOptions[j].get_optionName())
                        {
                            foundOptionName = true;
                        }
                        if(j == theOptions.size()-1 && foundOptionName == false)
                        {
                            message("Conflicting option: " + currentConflictingOptions[k] + " for option: " + theOptions[i].get_optionName() + "specified during setup but is not an option!");
                            conflictingOptionsPass = false;
                        }
                    }
                }
            }
        }
    }
    return conflictingOptionsPass;
}

//get value functions
std::vector<configOption> smokeTransportOptions::get_theOptions()
{
    return theOptions;
}
