#include "readConfigFile.h"

readConfigFile::readConfigFile(std::vector<configOption> theOptions_value)
{
    theOptions = theOptions_value;
    conversionFail = false;
}

void readConfigFile::newConfigFile(std::string configFilePath_value)
{
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
        message("failed to read config file: " + configFilePath + ".\nEnding program now\n");
        exit(EXIT_FAILURE);
    } else
    {
        message("succeeded in reading config file: " + configFilePath + ".\n\n");
    }
}


void readConfigFile::readFile()
{
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
    message("\nreading config file\n");
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
    std::cout << "finished reading config file\n";
}

void readConfigFile::processWords()
{
    if(foundWords.size() != 0)
    {
        if(foundWords.size() == 1)
        {
            message("Error! single word on a line! Need to specify both a variable name AND its values!\n");
            setupFail = false;
        } else
        {
            for(size_t i = 0; i < theOptions.size(); i++)
            {
                if(foundWords[0] == theOptions[i].get_optionName())
                {
                    if(theOptions[i].get_optionCurrentNumberOfValues() != 0)
                    {
                        message("Duplicate option: " + theOptions[i].get_optionName() + "!\n");
                    } else
                    {
                        updateNumberOfValues(i);
                        if(foundWords.size()-1 != theOptions[i].get_optionCurrentNumberOfValues())
                        {
                            message(theOptions[i].get_optionName() + " specified but wrong number of values!\n");
                            setupFail = true;
                        } else
                        {
                            std::cout << "Inserting " << foundWords.size()-1 << " words into " + theOptions[i].get_optionName() + ":";
                            for(size_t j = 1; j < foundWords.size(); j++)
                            {
                                std::cout << " \"" << foundWords[j] << "\"";
                                theOptions[i].addOptionValue(foundWords[j]);
                            }
                            message("\n");
                        }
                    }
                }
            }
        }
    }
}

void readConfigFile::updateNumberOfValues(size_t theOptionNumber)
{
    istringstream strm;
    strm.str(theOptions[theOptionNumber].get_optionOriginalNumberOfValues());
    size_t n = 0;
    if((strm >> n).fail())
    {
        strm.clear();
        for(size_t j = 0; j < theOptions.size(); j++)
        {
            if(theOptions[theOptionNumber].get_optionOriginalNumberOfValues() == theOptions[j].get_optionName())
            {
                if(theOptions[j].get_optionCurrentNumberOfValues() == 0)
                {
                    if(j >= theOptionNumber)
                    {
                        message(theOptions[theOptionNumber].get_optionName() + " specified before " + theOptions[j].get_optionName() + "!\n");
                        setupFail = false;
                    } else
                    {
                        message(theOptions[j].get_optionName() + " not filled! Can't update currentNumberOfValues for: " + theOptions[theOptionNumber].get_optionName() + "!\n");
                        setupFail = false;
                    }
                } else
                {
                    strm.str(theOptions[j].get_optionValues()[0]);
                    n = 0;
                    if((strm >> n).fail())
                    {
                        message("Could not update current number of values to: " + theOptions[j].get_optionValues()[0] + "\n");
                    } else
                    {
                        message("Updating current number of options to: " + theOptions[j].get_optionValues()[0] + "\n");
                        theOptions[theOptionNumber].updateNumberOfValues(n);
                    }
                }
            }
        }
    } else
    {
        message("Updating numberOfValues to: " + theOptions[theOptionNumber].get_optionOriginalNumberOfValues() + "\n");
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
    message("verifying all variables specified and assigned values\n");
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionConflicts() == true)
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() != 0 || theOptions[i].get_optionValues().size() != 0)
            {
                message("the " + theOptions[i].get_optionName() + " option conflicts! It should have no values!\n");
                setupFail = true;
            }
        } else
        {
            std::vector<std::string> currentOptionValues = theOptions[i].get_optionValues();
            if(theOptions[i].get_optionCurrentNumberOfValues() == 0)
            {
                message("NumberOfValues for " + theOptions[i].get_optionName() + " didn't get filled!\n");
                setupFail = true;
            }
            for(size_t j = 0; j < currentOptionValues.size(); j++)
            {
                std::string stringOfCounter = int_to_string(j);
                if(currentOptionValues[j] == "") //oh yeah, I purposefully made it so instead of using "" to specify no change, I said all values had to be filled
                {
                    message("Values for " + theOptions[i].get_optionName() + " index " + stringOfCounter + " not specified!\n");
                    setupFail = true;
                }
            }
        }
    }
    message("verification complete!\n");
}

int readConfigFile::get_optionValues_singleInt(std::string desiredOptionName)
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionName() == desiredOptionName)
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() != 1)
            {
                message("Requested single int value for " + desiredOptionName + " when there are currently " + int_to_string(theOptions[i].get_optionCurrentNumberOfValues()) + " values!\n");
                return -1;
            } else
            {
                return string_to_int(theOptions[i].get_optionValues()[0]);
            }
        }
    }
    message(desiredOptionName + " is not a valid option!\n");
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
                theOptionValues.push_back(string_to_int(theOptions[i].get_optionValues()[j]));
            }
            return theOptionValues;
        }
    }
    message(desiredOptionName + " is not a valid option!\n");
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
                message("Requested single int value for " + desiredOptionName + " when there are currently " + int_to_string(theOptions[i].get_optionCurrentNumberOfValues()) + " values!\n");
                return "";
            } else
            {
                return theOptions[i].get_optionValues()[0];
            }
        }
    }
    message(desiredOptionName + " is not a valid option!\n");
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
    message(desiredOptionName + " is not a valid option!\n");
    return std::vector<std::string> {""};
}


int readConfigFile::string_to_int(std::string s)
{
    conversionFail = false;
    istringstream strm;
    strm.str(s);
    int n = 0;
    if((strm >> n).fail())
    {
        strm.clear();
        message("conversion of string to int failed!\n");
        conversionFail = true;
    }
    return n;
}

std::string readConfigFile::int_to_string(int n)
{
    conversionFail = false;
    ostringstream strm;
    if((strm << n).fail())
    {
        strm.clear();
        message("conversion of int to string failed!\n");
        conversionFail = true;
    }
    return strm.str();
}

void readConfigFile::message(std::string theMessage)
{
    std::cout << theMessage;
    system("sleep 0.5");
}
