#include "readConfigFile.h"

readConfigFile::readConfigFile(std::vector<configOption> theOptions_value)
{
    // I think this makes a copy of the options found in smokeTransportOptions,
    // so would resetOptions really reset both? I don't think it would without this actually
    // passing a pointer to it
    // except that I think that there is only one copy of the options that is smokeTransportOptions
    // and one copy in this, but smokeTransportOptions stays around as a single copy the entire time
    // while this one reuses its copy over and over. So in the end there really are just two copies of options
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
    // probably would be better to just add some error handling making sure that configFilePath is always included as a variable in the smokeTransportOptions
    // but also some error handling in here to make sure that it is never specified in the config file. Then this update can happen before reading configFile
    updateOption_configFilePath();
    // I think the idea here was that while I don't have a good conflicting options yet, set a group of booleans
    // inside the options themselves to a single value so there cannot be any conflicting options yet
    // So this script is supposed to reset the conflictingOptions boolean for each option, which since no method, set all to false so there are never conflicting options
    // I'm probably going to change the way this is done, probably similar to the original and current number of values
    // or I guess the boolean is the current status of the conflicting option, the conflictingOptions vector contains a list of what stuff could cause a conflict
    // so maybe this script is correct since it sets all bool to a default so they can be set to their actual value in checkVariableFill.
    // so probably should move checkConflictingOptions so that it is run at the beginning of checkVariableFill (so inside checkVariableFill).

    // I actually like this as it is, but resetOption already resets all the conflictingOptions
    // so this function is probably redundant unless it is to set the new bool value for each option
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
        // instead of an exit message here, I should have a separate boolean throughout that ends the
        // process of reading a configFile and running the program, but still allows reading of the
        // next configFile and keeps a log file explaining all that has occurred
        // for now, it is fine to just exit
        exitMessage("config file: " + configFilePath + " doesn't exist!");
    }
    std::ifstream is_file(configFilePath);

    /*
     * This script goes through the config file line by line, analyzing each line one character at a time,
     * skipping any lines that begin with the pound # sign to allow for commenting, ignoring equals signs,
     * commas, and semicolons (unless they are in quotation marks, then they would be part of the configOption
     * values and later error handling will hopefully deal with it if that isn't a valid option).
     * The script uses a readStatus variable to keep track of what type of words and characters it needs
     * to be looking for. The general idea for how all this is done is shown in the following explanations:
     *
     * If the script comes across a character that is not whitespace or any of the ignored characters
     * (equals signs, commas, and semicolons) or any of the disallowed characters for the current readStatus,
     * then the script marks that char as the beginning of a configOption name. It then continues until
     * it finds the next whitespace or ignored characters (equals sign, commas, and semicolons),
     * using the next whitespace and ignored characters to tell itself that it has found the end of
     * the configOption name.
     *
     * After the script has finds the end of a configOption name, it stores the name and continues reading
     * the next characters until it finds a forwards brace {, forward bracket [, or forward parenthesis (.
     * The script marks this spot as the beginning of a vector of values for the given configOption name,
     * then continues reading until it finds a double quotation mark or a backwards brace },
     * backwards bracket ], or backwards parenthesis ). If the value is a double quotation mark,
     * the script will mark this spot as the beginning of a value for that vector of values and continue on
     * until it finds another double quotation mark. It will then store this value in a vector of values
     * and continue on looking for another start value double quotation mark or an end vector backwards brace,
     * backwards bracket, or backwards parenthesis of the same type used to find the vector start spot,
     * storing all values that it finds into the same vector.
     * When the script finds the end vector symbol instead of a starting double quotation mark for a new value,
     * then the script stores the vector of values it has found in a vector of vectors. Then it looks for the
     * beginning of a new vector or a new configOption name, using the same start vector character
     * to find the next vector for that same configOption. If the script finds a new configOption instead of
     * a new vector of values, then the script processes the found configOption name and found vector of vector
     * of values for that configOption, checking to make sure the name matches one of the configOptions given
     * in the program. It then processes all the values, making sure that the right number of values are
     * specified for that given configOption and that the option values are of the right type for that given
     * configOption (but not necessarily checking to see if each value is perfect. The individual programs that
     * use the values will do the error checking for if the value is completely legal).
     *
     *
     * To clarify the symbols stuff, a forward brace must be followed by a backwards brace, not backwards brackets or backwards parenthesis.
     * A forward bracket must be followed by a backwards bracket, not backwards braces or backwards parenthesis.
     * A forward parenthesis must be followed by a backwards parenthesis, not backwards braces or backwards brackets.
     * All vector start and end characters must match the type of the vector start character for a given config option.
     * So if the first vector of a configOption starts with a forwards parenthesis (, all the following vector start and end characters for that configOption must be forwards or backwards parenthesis ( or ).
     *
     * A given vector of values will always have the values separated by double quotation marks.
     * This allows the values to contain whitespace if necessary. This also allows the values to contain
     * single quotation marks ' if necessary for using variables that need special string manipulation
     * (paths, command line commands).
     *
     * The program gets mad if it ever sees a backward brace }, bracket ], or parenthesis ),
     * without first seeing a forward brace {, bracket [, or parenthesis (.
     * The program also gets mad if it sees a second forwards brace {, bracket [, or parenthesis (
     * before it has seen the corresponding backwards brace, bracket, or parenthesis
     * as determined by the type of the forward symbol.
     * The program also gets mad if it sees vector start positions when it isn't ready to look for vectors,
     * quotation marks when it isn't ready to look for values.
     *
     * This style allows the user to place equals signs between the variable name and the values,
     * and even commas to separate values, but technically it is not required to do so.
     * That would just be for improving readability for the user.
     *
     * After all the characters in the config file are analyzed and used to update the values
     * of the config options, the script checks to make sure that the numbers of vectors and
     * the numbers of values per vector are valid. This involves interaction between the different
     * configOptions, since some options are for giving the size of some of the vectors.
     */

    //get each line in the file
    std::string line;
    std::string readStatus = "findName";
    std::string foundWord = "";
    std::string vectorStartChr = "\"value not assigned yet\"";
    std::string vectorEndChr = "\"value not assigned yet\"";
    std::string foundOptionName = "";
    std::vector<std::string> foundSingleOptionValue;
    std::vector< std::vector<std::string> > foundOptionValues;
    while(std::getline(is_file,line))
    {
        if(line.substr(0,1) != "#")
        {
            for(std::size_t i = 0; i < line.length(); i++)
            {
                std::string current_chr = line.substr(i,1);

                // first set up error checking that isn't so specific that
                // it has to be done while actually finding values
                // so stuff that is only allowed during specific read status
                if(readStatus != "findValue" && readStatus != "foundValue")
                {
                    if(current_chr == "}" || current_chr == "]" || current_chr == ")")
                    {
                        exitMessage("Found vector end char }, ], or ) before start vector char {, [, or (!");
                    }
                    if(current_chr == "\"")
                    {
                        exitMessage("Found configOption value start char \" before finding start vector char {, [, or (!");
                    }
                }
                if(readStatus != "findVector")
                {
                    if(current_chr == "{" || current_chr == "[" || current_chr == "(")
                    {
                        exitMessage("Found start vector char {, [, or ( when read status isn't \"findVector\"!");
                    }
                }

                // this error part is a bit tricker because it has to do with only allowing one type
                // of vector start and end chars per line (so it uses the first type it finds for the whole line)
                // so these if statements are used again when using this status, but not for error stuff
                if(readStatus == "findVector")
                {
                    if(current_chr == "{" || current_chr == "[" || current_chr == "(")
                    {
                        if(foundOptionValues.size() != 0)
                        {
                            if(current_chr != vectorStartChr)
                            {
                                exitMessage("Second or later vector start char " + current_chr + " doesn't match first vector start char " + vectorStartChr + " for a configFile line!");
                            }
                        } else
                        {
                            // store the start vector char to make sure
                            // that all start and end vector chars match this char
                            vectorStartChr = current_chr;
                            if(vectorStartChr == "{")
                            {
                                vectorEndChr = "}";
                            }
                            if(vectorStartChr == "[")
                            {
                                vectorEndChr = "]";
                            }
                            if(vectorStartChr == "(")
                            {
                                vectorEndChr = ")";
                            }
                        }
                    }
                }
                if(readStatus == "findValue")
                {
                    if(current_chr == "}" || current_chr == "]" || current_chr == ")")
                    {
                        if(current_chr != vectorEndChr)
                        {
                            exitMessage("Vector end char " + current_chr + " doesn't match vector start char " + vectorStartChr + "!");
                        }
                    }
                }

                // now set up what to do in a given status
                if(readStatus == "findName")
                {
                    if(current_chr != " " && current_chr != "=" && current_chr != "," && current_chr != ";")
                    {
                        foundWord = foundWord + current_chr;
                    } else
                    {
                        // so found a char that isn't a configOption name char
                        if(foundWord != "")
                        {
                            // found a char that isn't a configOption name char
                            // AND had already started filling in a configOption name
                            // so we have reached the end of the configOption name
                            debugMessage("Storing word " + foundWord + " into foundOptionName");
                            foundOptionName = foundWord;
                            foundWord = "";
                            readStatus = "findVector";
                        }
                    }
                }
                if(readStatus == "findVector")
                {
                    if(current_chr == vectorStartChr)
                    {
                        // found the start of a vector, now look for values
                        readStatus = "findValue";

                    } else if(current_chr != " " && current_chr != "=" && current_chr != "," && current_chr != ";")
                    {
                        // found the start of a new configOption. Yay!
                        // process the finished configOption
                        debugMessage("Found end of optionName " + foundOptionName + ". Processing values");
                        processWords(foundOptionName,foundOptionValues); // need to revamp this function to deal with the new variable storage format
                        // now clean up and start the beginning process of the new config option
                        readStatus = "findName";
                        foundWord = current_chr;
                        vectorStartChr = "\"value not assigned yet\"";
                        vectorEndChr = "\"value not assigned yet\"";
                        foundOptionName = "";
                        while(!foundOptionValues.empty())
                        {
                            foundOptionValues.pop_back();   //might have to improve this because vector of vectors
                        }
                        while(!foundSingleOptionValue.empty())
                        {
                            foundSingleOptionValue.pop_back();
                        }
                    }
                }
                if(readStatus == "foundValue")
                {
                    if(current_chr == "\"")
                    {
                        // found the end of the value, even if it is empty
                        debugMessage("Storing word " + foundWord + " into foundSingleOptionValue");
                        foundSingleOptionValue.push_back(foundWord);
                        foundWord = "";
                        readStatus = "findValue";
                    } else
                    {
                        foundWord = foundWord + current_chr;
                    }
                }
                if(readStatus == "findValue")
                {
                    if(current_chr == "\"")
                    {
                        //if(foundWord != "")
                        //{
                            readStatus = "foundValue";
                        //}
                    } else if(current_chr == vectorEndChr)
                    {
                        // found the end of a vector
                        if(foundWord != "")
                        {
                            // was not finished filling in a word, so forgot an end quotation mark
                            exitMessage("Missing end quotation mark for a value!");
                        } else
                        {
                            debugMessage("Found the end of a vector. Storing foundSingleOptionValue vector into foundOptionValues. foundSingleOptionValue.size = " + foundSingleOptionValue.size());
                            foundOptionValues.push_back(foundSingleOptionValue);
                            readStatus = "findVector";
                        }
                    }
                }
            }
        }
    }
    debugMessage("finished reading config file");
}

void readConfigFile::processWords(std::string foundOptionName, std::vector< std::vector<std::string> > foundOptionValues)
{
    if(foundOptionName == "")
    {
        exitMessage("processWords somehow run with empty foundOptionName!");
    }
    if(foundOptionValues.size() == 0)
    {
        // maybe need to go through again and see if an empty variable places a "" value
        // then could have a variable checking that multiple configOptions have the same number of vectors
        // using a neededOptions variable similar to the conflictingOptions variable
        message("foundOptionName = " + foundOptionName + ", but found no values for its foundOptionValues!");
        setupFail = true;
    }

    // this works for now because it assumes that the order of the variables sets the needed number of values ahead of time
    // so probably should change the foundOptions to be a vector of vectors of vectors
    // or whatever so that all the information is grabbed by the configFile reader before processing the information
    // instead of processing each time it gets a new option. It can process the option names as it sees them,
    // but really can't do much with the values until it has all the information
    // (except maybe check that the values have a valid type)
    // this won't help avoid the need to make numberOfValues a vector instead of a double
    // since imagine two cases with differing numbers of start and stop runs
    // the number of vectors for certain variables does matter, but the number of values for a given vector of vectors
    // can change for each different run
    // actually for now I think it is fine to keep a single currentNumberOfValues for each configOption
    // because the only variables that can have differing numbers are source variables.
    // The number of start and stop times differ all right, but only between config files.
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(foundOptionName == theOptions[i].get_optionName())
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() != 0)
            {
                message("Duplicate option: " + theOptions[i].get_optionName() + "!");
            } else
            {
                updateNumberOfValues(i);
                for(size_t k = 0; k < foundOptionValues.size(); k++)
                {
                    if(foundOptionValues[k].size() != theOptions[i].get_optionCurrentNumberOfValues())
                    {
                        message(theOptions[i].get_optionName() + " specified but wrong number of values for vector " + intToString(k) + "!");
                        setupFail = true;
                    } else
                    {
                        splitDebugMessage("Inserting " + intToString(foundOptionValues[k].size()) + " words into " + theOptions[i].get_optionName() + ":");
                        for(size_t j = 0; j < foundOptionValues[k].size(); j++)
                        {
                            splitDebugMessage(" \"" + foundOptionValues[k][j] + "\"");
                            theOptions[i].addOptionValue(foundOptionValues[k][j],k);
                        }
                        debugMessage("");
                    }
                }
            }
        }
    }

}

void readConfigFile::updateNumberOfValues(size_t theOptionNumber)
{
    std::string s = theOptions[theOptionNumber].get_optionOriginalNumberOfValues();
    //Some of the originalNumberOfValues were purposefully left as strings because the value is specified somewhere else
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
                    // check to see if the new value to be used is numeric because if it is not, then it hasn't been filled yet
                    // this also assumes that the very first of all variables for this past option is already filled
                    s = theOptions[j].get_optionValues()[0][0];
                    n = 0;
                    if(is_size_t(s) == false)
                    {
                        message("Could not update current number of values to: " + theOptions[j].get_optionValues()[0][0] + ". The value is not numeric!");
                    } else
                    {
                        n = stringToInt(s); // may be better to use a double or something else since n is type size_t
                        debugMessage("Updating current number of options for " + theOptions[theOptionNumber].get_optionName() + " to: " + theOptions[j].get_optionValues()[0][0]);
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

void readConfigFile::updateOption_configFilePath()
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionName() == "configFilePath")
        {
            theOptions[i].updateNumberOfValues(1);  //only one path
            theOptions[i].addOptionValue(configFilePath,0);
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
            // make sure it doesn't have even a single value
            if(theOptions[i].get_optionCurrentNumberOfValues() != 0 || theOptions[i].get_optionValues().size() != 0)
            {
                message("the " + theOptions[i].get_optionName() + " option conflicts! It should have no values!");
                setupFail = true;
            }
        } else
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() == 0)
            {
                message("NumberOfValues for " + theOptions[i].get_optionName() + " didn't get filled!");
                setupFail = true;
            }

            // create a temporary variable to act as an alias
            std::vector<std::vector<std::string> > currentOptionValues = theOptions[i].get_optionValues();
            for(size_t k = 0; k < currentOptionValues.size(); k++)
            {
                for(size_t j = 0; j < currentOptionValues[k].size(); j++)
                {
                    if(currentOptionValues[k][j] == "") //oh yeah, I purposefully made it so instead of using "" to specify no change, I said all values had to be filled. I might need to add in some kind of check of conflicting options or some kind of boolean to check to see if it was supposed to be filled instead of just saying nothing can be empty
                    {
                        message("Values for " + theOptions[i].get_optionName() + " index [" + intToString(k) + "][" + intToString(j) + "] not specified!");
                        setupFail = true;
                    }
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
                return stringToInt(theOptions[i].get_optionValues()[0][0]);
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
            for(size_t j = 0; j < theOptions[i].get_optionValues()[0].size(); j ++)
            {
                theOptionValues.push_back(stringToInt(theOptions[i].get_optionValues()[0][j]));
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
                return theOptions[i].get_optionValues()[0][0];
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
            return theOptions[i].get_optionValues()[0];
        }
    }
    exitMessage(desiredOptionName + " is not a valid option!");
    return std::vector<std::string> {""};
}
