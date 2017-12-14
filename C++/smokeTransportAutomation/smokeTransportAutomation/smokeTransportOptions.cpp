#include "smokeTransportOptions.h"

//constructor related functions
smokeTransportOptions::smokeTransportOptions()
{
    handy.message("\nSetting up available smokeTransportOptions");    // extra line at the beginning because this is a new set of functions/operations
    setupAvailableOptions();
    if(check_duplicateOptions() == false)
    {
        handy.exitMessage("duplicate option found during setup!");
    }
    if(check_originalNumberOfValues() == false)
    {
        handy.exitMessage("invalid numberOfValues found during setup!");
    }
    if(check_conflictingOptions() == false)    // probably need to mess with variable naming to make sure the booleans keep the same logic for each setup
    {
        handy.exitMessage("invalid conflictingOptions found during setup!");
    }
}

void smokeTransportOptions::addOption(std::string newOptionName,std::string newOptionDataType,std::string newOptionNumberOfValues,std::vector<std::string> newConflictingOtions)
{
    theOptions.push_back(configOption(newOptionName,newOptionDataType,newOptionNumberOfValues,newConflictingOtions));
}

void smokeTransportOptions::setupAvailableOptions()
{
    //here is where each and every possible option is specified
    // note that the config file path used/given by readConfigFile will be kept separately as a variable in readConfigFile
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
                handy.message("Duplicate option: " + theOptions[i].get_optionName() + "!");
                return false;
            }
        }
    }
    return true;
}

bool smokeTransportOptions::check_originalNumberOfValues()
{
    // so if it is purposefully a string type, need to make sure that the string is the optionName of another type
    // also make sure that the two options don't conflict
    std::string theNumberOfValues;
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        theNumberOfValues = theOptions[i].get_optionOriginalNumberOfValues();
        if(handy.is_size_t(theNumberOfValues) == false)
        {
            bool foundName = false;
            for(size_t j = 0; j < theOptions.size(); j++)
            {
                if(theNumberOfValues == theOptions[j].get_optionName())
                {
                    // the given number of values was found to be given by one of the other options
                    // make sure these two interdependent options don't conflict
                    for(size_t k = 0; k < theOptions[j].get_conflictingOptions().size(); k++)
                    {
                        if(theNumberOfValues == theOptions[j].get_conflictingOptions()[k])
                        {
                            handy.message("the conflicting option for option " + theOptions[i].get_optionName() + " is the same as the originalNumberOfValues \"" + theNumberOfValues + "\"!");
                            return false;
                        }
                    }
                    for(size_t k = 0; k < theOptions[i].get_conflictingOptions().size(); k++)
                    {
                        if(theNumberOfValues == theOptions[i].get_conflictingOptions()[k])
                        {
                            handy.message("the conflicting option for option " + theOptions[i].get_optionName() + " is the same as the originalNumberOfValues \"" + theNumberOfValues + "\"!");
                            return false;
                        }
                    }
                    foundName = true;
                }
            }
            if(foundName == false)
            {
                handy.message("invalid numberOfValues for option " + theOptions[i].get_optionName() + "!");
                return false;
            }
        }
    }
    return true; // can only get to here if foundName always ends up true
}

//make sure each conflicting option really does exist as a variable name
bool smokeTransportOptions::check_conflictingOptions()
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
                    handy.message("Conflicting option: " + currentConflictingOptions[k] + " specified the same as it's own option!");
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
                    }
                    if(foundOptionName == false)
                    {
                        handy.message("Conflicting option: " + currentConflictingOptions[k] + " for option: " + theOptions[i].get_optionName() + "specified during setup but is not an option!");
                        conflictingOptionsPass = false;
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
