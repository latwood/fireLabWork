#include "readConfigFile.h"

readConfigFile::readConfigFile(std::vector<configOption> theOptions_value)
{
    theOptions = theOptions_value;
}

void readConfigFile::newConfigFile(std::string configFilePath_value)
{

    message("\nreading config file");   // extra space because this is a new set of functions/operations

    //clear old variables
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        theOptions[i].resetOption();
    }

    //reset variables
    configFilePath = configFilePath_value;

    setupFail = false;  //if this becomes true in the error handling, then the program ends early

    //now start reading config file to get new variables
    readFile();
    updateOption_configFilePath();
    checkConflictingOptions();
    checkVariableFill();
    if(setupFail == true)
    {
        exitMessage("failed to read config file: " + configFilePath);
    } else
    {
        message("succeeded in reading config file");
    }
}


void readConfigFile::readFile()
{
    if(fileExists(configFilePath) != true)
    {
        exitMessage("config file: " + configFilePath + " doesn't exist!");
        exit(EXIT_FAILURE);
    }
    std::ifstream is_file(configFilePath);

//smokeTransportConfig
//this parses by getting each line, then separating out the values, assuming that
//spaces, quotes, equal signs, commas, single quotes, and semicolons are not variables

//you HAVE to keep the variables and their values on the same line!

//Currently this ignores everything that isn't _key variable name_ value value value
//could eventually do multiple lines but why complicate it? That's what boost is for.
//plus it would require always checking the word in the moment for a key, and all words
//following until the next key would have to be considered variables for that key

    //get each line in the file
    std::string line;
    while(std::getline(is_file,line))
    {
        //separate out each word on a given line
        bool foundStartSpot = false;
        int startSpot = 0;
        for(std::size_t i = 0; i < line.length(); i++)
        {
            std::string current_chr = line.substr(i,1);
            std::string next_chr = line.substr(i+1,1);
            if(current_chr == "{")
            {
                vectorStartIndex.push_back(foundWords.size());
            }
            if(current_chr == "}")
            {
                vectorEndIndex.push_back(foundWords.size()-1);
            }
            if(current_chr != " " && current_chr != "\"" && current_chr != "=" && current_chr != ","
                     && current_chr != "{" && current_chr != "}" && current_chr != "\'" && current_chr != ";")
            {
                if(foundStartSpot == false)
                {
                    foundStartSpot = true;
                    startSpot = i;
                }
                if(foundStartSpot == true)
                {
                    if(next_chr == " " || next_chr == "\"" || next_chr == "=" || next_chr == "," || next_chr == "{"
                        || next_chr == "}" || next_chr == "\'" || next_chr == ";" || i == line.length()-1)
                    {
                        foundWords.push_back(line.substr(startSpot,i-startSpot+1));
                        foundStartSpot = false; //reset the search for the next word
                    }
                }
            }
        }
        processWords();
        emptyWords();
    }
    debugMessage("finished reading config file");
}

void readConfigFile::processWords()
{
    if(foundWords.size() != 0)
    {
        if(foundWords.size() == 1)
        {
            message("Error! single word on a line! Need to specify both a variable name AND its values!");
            setupFail = false;
        } else
        {
            for(size_t i = 0; i < theOptions.size(); i++)
            {
                if(foundWords[0] == theOptions[i].get_optionName())
                {
                    if(theOptions[i].get_optionCurrentNumberOfValues() != 0)
                    {
                        message("Duplicate option: " + theOptions[i].get_optionName() + "!");
                    } else
                    {
                        updateNumberOfValues(i);
                        if(foundWords.size()-1 != theOptions[i].get_optionCurrentNumberOfValues())
                        {
                            message(theOptions[i].get_optionName() + " specified but wrong number of values!");
                            setupFail = true;
                        } else
                        {
                            splitDebugMessage("Inserting " + intToString(foundWords.size()-1) + " words into " + theOptions[i].get_optionName() + ":");
                            for(size_t j = 1; j < foundWords.size(); j++)
                            {
                                splitDebugMessage(" \"" + foundWords[j] + "\"");
                                theOptions[i].addOptionValue(foundWords[j]);
                            }
                            debugMessage("");
                        }
                    }
                }
            }
        }
    }
}

void readConfigFile::updateNumberOfValues(size_t theOptionNumber)
{
    std::string s = theOptions[theOptionNumber].get_optionOriginalNumberOfValues();
    // I almost want to say that the assumption is that this value hasn't been verified to actually be the right type yet, and that it hasn't been verified in configOption
    // oh wait, it was purposefully left as a string because the value is specified somewhere else
    size_t n = 0;
    if(is_size_t(s) == false)
    {
        for(size_t j = 0; j < theOptions.size(); j++)
        {
            if(theOptions[theOptionNumber].get_optionOriginalNumberOfValues() == theOptions[j].get_optionName())
            {
              // assumes the new value isn't a string?
                if(theOptions[j].get_optionCurrentNumberOfValues() == 0)
                {
                    if(j >= theOptionNumber)
                    {
                        // I think this might be a debug message, since the value will be filled later despite this problem? Actually I don't think this will fill in later, so if this message pops up and doesn't make sense, need to change it a bit to be more specific
                        message(theOptions[theOptionNumber].get_optionName() + " specified before " + theOptions[j].get_optionName() + "!");
                        setupFail = false;
                    } else
                    {
                        message(theOptions[j].get_optionName() + " not filled! Can't update currentNumberOfValues for: " + theOptions[theOptionNumber].get_optionName() + "!");
                        setupFail = false;
                    }
                } else
                {
                    s = theOptions[j].get_optionValues()[0];
                    n = 0;
                    if(is_size_t(s) == false)
                    {
                        message("Could not update current number of values to: " + theOptions[j].get_optionValues()[0]);
                    } else
                    {
                        n = stringToInt(s); // may be better to use a double or something else since n is type size_t
                        debugMessage("Updating current number of options for " + theOptions[theOptionNumber].get_optionName() + " to: " + theOptions[j].get_optionValues()[0]);
                        theOptions[theOptionNumber].updateNumberOfValues(n);
                    }
                }
            }
        }
    } else
    {
        n = stringToInt(s); // may be better to use a double or something else since n is type size_t
        debugMessage("Updating numberOfValues for " + theOptions[theOptionNumber].get_optionName() + " to: " + theOptions[theOptionNumber].get_optionOriginalNumberOfValues());
        theOptions[theOptionNumber].updateNumberOfValues(n);
    }
}

void readConfigFile::emptyWords()
{
    while(!foundWords.empty())
    {
        foundWords.pop_back();
    }
}

void readConfigFile::updateOption_configFilePath()
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionName() == "configFilePath")
        {
            theOptions[i].updateNumberOfValues(1);  //only one path
            theOptions[i].addOptionValue(configFilePath);
        }
    }
}

void readConfigFile::checkConflictingOptions()
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        theOptions[i].updateOptionConflicts(false);
    }
}

void readConfigFile::checkVariableFill()
{
    message("verifying all variables specified and assigned values");
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionConflicts() == true)
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() != 0 || theOptions[i].get_optionValues().size() != 0)
            {
                message("the " + theOptions[i].get_optionName() + " option conflicts! It should have no values!");
                setupFail = true;
            }
        } else
        {
            std::vector<std::string> currentOptionValues = theOptions[i].get_optionValues();
            if(theOptions[i].get_optionCurrentNumberOfValues() == 0)
            {
                message("NumberOfValues for " + theOptions[i].get_optionName() + " didn't get filled!");
                setupFail = true;
            }
            for(size_t j = 0; j < currentOptionValues.size(); j++)
            {
                std::string stringOfCounter = intToString(j);
                if(currentOptionValues[j] == "") //oh yeah, I purposefully made it so instead of using "" to specify no change, I said all values had to be filled. I might need to add in some kind of check of conflicting options or some kind of boolean to check to see if it was supposed to be filled instead of just saying nothing can be empty
                {
                    message("Values for " + theOptions[i].get_optionName() + " index " + stringOfCounter + " not specified!");
                    setupFail = true;
                }
            }
        }
    }
}

int readConfigFile::get_optionValues_singleInt(std::string desiredOptionName)
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionName() == desiredOptionName)
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() != 1)
            {
                exitMessage("Requested single int value for " + desiredOptionName + " when there are currently " + intToString(theOptions[i].get_optionCurrentNumberOfValues()) + " values!");
                return -1;
            } else
            {
                return stringToInt(theOptions[i].get_optionValues()[0]);
            }
        }
    }
    exitMessage(desiredOptionName + " is not a valid option!");
    return -1;
}

std::vector<int> readConfigFile::get_optionValues_multiInt(std::string desiredOptionName)
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionName() == desiredOptionName)
        {
            std::vector<int> theOptionValues;
            for(size_t j = 0; j < theOptions[i].get_optionValues().size(); j ++)
            {
                theOptionValues.push_back(stringToInt(theOptions[i].get_optionValues()[j]));
            }
            return theOptionValues;
        }
    }
    exitMessage(desiredOptionName + " is not a valid option!");
    return {-1};
}

std::string readConfigFile::get_optionValues_singleString(std::string desiredOptionName)
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionName() == desiredOptionName)
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() != 1)
            {
                exitMessage("Requested single int value for " + desiredOptionName + " when there are currently " + intToString(theOptions[i].get_optionCurrentNumberOfValues()) + " values!");
                return "";
            } else
            {
                return theOptions[i].get_optionValues()[0];
            }
        }
    }
    exitMessage(desiredOptionName + " is not a valid option!");
    return "";
}

std::vector<std::string> readConfigFile::get_optionValues_multiString(std::string desiredOptionName)
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionName() == desiredOptionName)
        {
            return theOptions[i].get_optionValues();
        }
    }
    exitMessage(desiredOptionName + " is not a valid option!");
    return std::vector<std::string> {""};
}
