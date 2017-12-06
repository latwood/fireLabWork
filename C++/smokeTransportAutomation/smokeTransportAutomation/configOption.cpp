#include "configOption.h"

//constructor
configOption::configOption(std::string optionName_value,std::string optionDataType_value,
                             std::string optionNumberOfValues_value,
                             std::vector<std::string> conflictingOptions_value)
{
    optionName = optionName_value;
    if(check_optionDataType(optionDataType_value) == true)
    {
        optionDataType = optionDataType_value;
    } else
    {
        handy.message("optionDataType: " + optionDataType_value + " for option: " + optionName_value + " not a valid data type!");
    }
    // no initial check on optionOriginalNumberOfValues because value can be a string meaning a reference to another option. This will be checked when updating the options or by classes that use the configOptions
    optionOriginalNumberOfValues = optionNumberOfValues_value;
    // no initial check on conflictingOptions because value will be a string meaning a reference to another option. This will be checked by the classes that use the configOptions
    conflictingOptions = conflictingOptions_value;
}

void configOption::resetOption()
{
    optionCurrentNumberOfValues = 0;
    while(!optionValues.empty())
    {
        optionValues.pop_back();    // might have to do something tricky because vector of vectors
    }
    optionConflicts = false;
}


//error handling functions
bool configOption::check_optionDataType(std::string newOptionDataType)
{
    if(newOptionDataType == "std::string" || newOptionDataType == "size_t" || newOptionDataType == "int" ||
            newOptionDataType == "long" || newOptionDataType == "float" || newOptionDataType == "double" ||
            newOptionDataType == "bool")
    {
        return true;
    } else
    {
        handy.message("optionDataType: " + newOptionDataType + " is not a valid type!");
        return false;
    }
}

bool configOption::check_optionNumberOfValues(std::string newOptionNumberOfValues)
{
    //make sure this function doesn't get run on the constructor, but only when updating values.
    //some stuff is initialized as strings since it will be replaced by a variable
    // So this needs to be used on the currentNumberOfValues, not originalNumberOfValues
    if(handy.is_double(newOptionNumberOfValues) == false)
    {
        handy.message("optionNumberOfValues: " + newOptionNumberOfValues + " is not numeric!");
        return false;
    } else
    {
        return true;
    }
}

bool configOption::check_optionValue(std::string newOptionValue)
{
    bool valuesGood = true;
    bool isType = true;
    if(optionDataType == "std::string")
    {
        isType = true;  //it would break at some point if this weren't already true
    } else if(optionDataType == "size_t")
    {
        isType = handy.is_size_t(newOptionValue);
    } else if(optionDataType == "int")
    {
        isType = handy.is_int(newOptionValue);
    } else if(optionDataType == "long")
    {
        isType = handy.is_long(newOptionValue);
    } else if(optionDataType == "float")
    {
        isType = handy.is_float(newOptionValue);
    } else if(optionDataType == "double")
    {
        isType = handy.is_double(newOptionValue);
    } else if(optionDataType == "bool")
    {
        isType = handy.is_bool(newOptionValue);
    }

    if(isType == false)
    {
        handy.message("optionDataType is " + optionDataType + " but newOptionValue \"" + newOptionValue + "\" is not that type!");
        valuesGood = false;
    }
    return valuesGood;
}

void configOption::updateOptionConflicts(bool newOptionConflictsValue)
{
    optionConflicts = newOptionConflictsValue;  //note don't need to check because it is already a bool coming in or it fails
}


//data updater functions
void configOption::updateNumberOfValues(size_t newNumberOfValues)
{
    optionCurrentNumberOfValues = newNumberOfValues;
}

void configOption::addOptionValue(std::string newOptionValue, size_t vectorCounter)   // maybe add another variable: bool isNewVector to tell if it is pushbacked to smallest scale or next up scale vector
{
    // check to make sure the vectorCounter is within the number
    // of vectors plus one (for adding a new vector if needed)
    // don't need a check for negativity because type size_t
    if(vectorCounter > optionValues.size()+1)
    {
        handy.exitMessage("vectorCounter exceeds one plus the number of vectors in optionValues! Only allowed to add at most one vector to optionValues!");
    }

    if(check_optionValue(newOptionValue) == true)
    {
        if(vectorCounter == optionValues.size()+1)
        {
            std::vector<std::string> newOptionVector;
            newOptionVector.push_back(newOptionValue);
            optionValues.push_back(newOptionVector);
        } else
        {
            optionValues[vectorCounter].push_back(newOptionValue);
        }
    } else
    {
        handy.exitMessage("optionValue \"" + newOptionValue + "\" is not valid!");
    }
}


//data getter functions
std::string configOption::get_optionName()
{
    return optionName;
}

std::string configOption::get_optionDataType()
{
    return optionDataType;
}

std::string configOption::get_optionOriginalNumberOfValues()
{
    return optionOriginalNumberOfValues;
}

std::vector<std::string> configOption::get_conflictingOptions()
{
    return conflictingOptions;
}

size_t configOption::get_optionCurrentNumberOfValues()
{
    return optionCurrentNumberOfValues;
}

std::vector< std::vector<std::string> > configOption::get_optionValues()
{
    return optionValues;
}

bool configOption::get_optionConflicts()
{
    return optionConflicts;
}
