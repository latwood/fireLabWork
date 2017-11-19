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
    // I think the idea here was to make sure that configFilePath is set no matter what, at least in the options. It may or not be set inside the readFile. You would think that it should be set there or something, or before that function, but I guess after makes sure that it will always be the right variable even if something else were to get put there.
    // could also probably put this into checkVariableFill then.
    updateOption_configFilePath();
    // I think the idea here was that while I don't have a good conflicting options yet, set a group of booleans
    //inside the options themselves to a single value so there cannot be any conflicting options yet
    // So this script is supposed to set the conflictingOptions boolean for each option, which since no method, set all to false so there are never conflicting options
    // I'm probably going to change the way this is done, probably similar to the original and current number of values
    // or I guess the boolean is the current status of the conflicting option, the conflictingOptions vector contains a list of what stuff could cause a conflict
    // so maybe this script is correct since it sets all bool to a default so they can be set to their actual value in checkVariableFill.
    // so probably should move checkConflictingOptions so that it is run at the beginning of checkVariableFill (so inside checkVariableFill).
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

    /*
     * This script goes through the config file line by line, analyzing each line one character at a time,
     * skipping any lines that begin with the pound # sign to allow for commenting, ignoring equals signs, commas, and semicolons
     * (unless they are in quotation marks, then they would be part of the configOption values and later error handling will hopefully deal with it if that isn't a valid option).
     * If it comes across a character that is not whitespace, a forwards brace {, a forwards bracket [, a forwards parenthesis(, a backwards brace }, a backwards bracket ], a backwards parenthesis ),
     * or a double quotation mark ", then the script marks that char as the beginning of a configOption name.
     * It then continues until it finds the next whitespace or ignored characters (equals sign, commas, and semicolons), getting mad if it sees braces, brackets, parenthesis, or double quotation marks,
     * using the next whitespace and ignored characters to tell itself that it has found the end of the configOption name.
     *
     * After the script has found a configOption name, it checks it to make sure the configOption name matches
     * with one of the configOptions given in the program, then continues reading the next characters
     * until it finds a forwards brace {, forward bracket [, or forward parenthesis (. The script marks this spot as the beginning of a vector of values
     * for the given configOption name that it has last found. Then continues reading until it finds a double quotation mark.
     * The script will mark this spot as the beginning of a value for that vector of values and continue on until it finds
     * another double quotation mark, using the next double quotation mark to tell the script that it has found the end of the value for the given vector.
     * The script stores that value in a vector, which when it has found all the values in that vector will be added as one of the given configOption vector of values given by that configOption name,
     * doing error checking to make sure that the value is the right type for what is allowed for the given config option (but not necessarily checking to see if the value is perfect).
     * The script then continues on until it finds another double quotation mark or a backwards brace }, backwards bracket ], or backwards parenthesis ) to let it know that it has
     * found more values for the given vector, that it has found all the values for the given vector.
     *
     * When the script finds a backwards brace, bracket, or parenthesis to finish off a vector of configOption values, it continues reading.
     * If it finds a character that isn't a forwards brace {, forwards bracket [, forwards parenthesis (, whitespace, or ignored characters (equals signs, commas, semicolons),
     * then the script has found all the vectors of values for that given configOption and it has found a new configOption name and repeats the above process again, but for this new configOption.
     * If it finds another forwards brace, bracket, or parenthesis first, then it repeats the process of finding a vector of values,
     * adding each vector it finds this way for that given configOption to the vector of vectors for the given config option.
     *
     * To clarify the symbols stuff, a forward brace must be followed by a backwards brace, not backwards brackets or backwards parenthesis.
     * A forward bracket must be followed by a backwards bracket, not backwards braces or backwards parenthesis.
     * A forward parenthesis must be followed by a backwards parenthesis, not backwards braces or backwards brackets.
     *
     * A given vector of values will always have the values separated by double quotation marks. This allows the values to contain whitespace if necessary. This also allows the values to contain
     * single quotation marks ' if necessary for using variables that need special string manipulation (paths, command line commands).
     *
     * The program gets mad if it ever sees a backward brace }, bracket ], or parenthesis ), without first seeing a forward brace {, bracket [, or parenthesis (.
     * The program also gets mad if it sees a second forwards brace {, bracket [, or parenthesis ( before it has seen the corresponding backwards brace, bracket, or parenthesis
     * as determined by the type of the forward symbol.
     *
     * This style allows the user to place equals signs between the variable name and the values, and even commas to separate values,
     * but technically it is not required to do so. That would just be for improving readability for the user.
     *
     * After all the characters in the config file are analyzed and used to update the values of the config options,
     * the script checks to make sure that the numbers of vectors and the numbers of values per vector are valid.
     * This involves interaction between the different configOptions, since some options are for giving the size of some of the vectors.
     *
     */

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
