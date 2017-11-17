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
        message("optionDataType: " + optionDataType_value + " for option: " + optionName_value + " not a valid data type!");
    }
    optionOriginalNumberOfValues = optionNumberOfValues_value;
    conflictingOptions = conflictingOptions_value;
}

void configOption::resetOption()
{
    optionCurrentNumberOfValues = 0;
    while(!optionValues.empty())
    {
        optionValues.pop_back();
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
        message("optionDataType: " + newOptionDataType + " is not a valid type!");
        return false;
    }
}

bool configOption::check_optionNumberOfValues(std::string newOptionNumberOfValues)
{
    //make sure this function doesn't get run on the constructor, but only when updating values.
    //some stuff is initialized as strings since it will be replaced by a variable
    if(is_double(newOptionNumberOfValues) == false)
    {
        message("optionNumberOfValues: " + newOptionNumberOfValues + " is not numeric!");
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
        isType = is_size_t(newOptionValue);
    } else if(optionDataType == "int")
    {
        isType = is_int(newOptionValue);
    } else if(optionDataType == "long")
    {
        isType = is_long(newOptionValue);
    } else if(optionDataType == "float")
    {
        isType = is_float(newOptionValue);
    } else if(optionDataType == "double")
    {
        isType = is_double(newOptionValue);
    } else if(optionDataType == "bool")
    {
        isType = is_bool(newOptionValue);
    }

    if(isType == false)
    {
        message("optionDataType is " + optionDataType + " but newOptionValue \"" + newOptionValue + "\" is not that type!");
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

void configOption::addOptionValue(std::string newOptionValue)
{
    if(check_optionValue(newOptionValue) == true)
    {
        optionValues.push_back(newOptionValue);
    } else
    {
        exitMessage("optionValue \"" + newOptionValue + "\" is not valid!");
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

std::vector<std::string> configOption::get_optionValues()
{
    return optionValues;
}

bool configOption::get_optionConflicts()
{
    return optionConflicts;
}
