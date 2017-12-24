#include "readConfigFile.h"

readConfigFile::readConfigFile(std::vector<configOption> theOptions_value)
{
    // I think this makes a copy of the options found in smokeTransportOptions, but smokeTransportOptions
    // stays around as a single copy the entire time while this one reuses its copy over and over.
    // So in the end there really are just two copies of options, even though I'm not using pointers
    theOptions = theOptions_value;
}

void readConfigFile::newConfigFile(std::string configFilePath_value)
{

    handy.message("\nreading config file");   // extra space because this is a new set of functions/operations

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

    // determine if there are any conflicting options and warn if there are
    checkConflictingOptions();

    // determine if all variables without conflicting options were filled
    checkVariableFill();

    // end the program if there were any problems
    if(setupFail == true)
    {
        handy.exitMessage("failed to read config file: " + configFilePath);
    } else
    {
        handy.message("succeeded in reading config file");
    }
}


void readConfigFile::readFile()
{
    if(handy.fileExists(configFilePath) != true)
    {
        // instead of an exit message here, I should have a separate boolean throughout that ends the
        // process of reading a configFile and running the program, but still allows reading of the
        // next configFile and keeps a log file explaining all that has occurred
        // for now, it is fine to just exit. But maybe this could be handled with a completeExitMessage vs exitMessage, or exitrunMessage
        handy.exitMessage("config file: " + configFilePath + " doesn't exist!");
    }
    std::ifstream is_file(configFilePath.c_str());

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
     * a new vector of values, then the script stores the found configOption name and found vector of vector
     * of values for that configOption into another vector.
     *
     * After all the configOption names and vectors of values are found, the information is processed, checking
     * to make sure the names match one of the configOptions given in the program, making sure that the right
     * number of values are specified for each given configOption and that the option values are of the right
     * type for that given configOption (but not necessarily checking to see if each value is perfect. The
     * individual programs that use the values will do the error checking for if the value is completely legal).
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
    std::vector<std::string> foundOptionNames;
    size_t currentOptionName = 0;
    std::vector<std::string> foundSingleOptionValue;
    std::vector< std::vector<std::string> > foundMultiOptionValues;
    std::vector< std::vector< std::vector<std::string> > > foundOptionValues;
    while(std::getline(is_file,line))
    {
        if(line.substr(0,1) != "#")
        {
            for(std::size_t i = 0; i < line.length(); i++)
            {
                bool changedStatus = false;
                std::string current_chr = line.substr(i,1);
                //handy.debugMessage("current chr = " + current_chr + ", status = " + readStatus);

                // first set up error checking that isn't so specific that
                // it has to be done while actually finding values
                // so stuff that is only allowed during specific read status
                if(readStatus != "findValue" && readStatus != "foundValue")
                {
                    if(current_chr == "}" || current_chr == "]" || current_chr == ")")
                    {
                        handy.exitMessage("Found vector end char }, ], or ) before start vector char {, [, or (!");
                    }
                    if(current_chr == "\"")
                    {
                        handy.exitMessage("Found configOption value start char \" before finding start vector char {, [, or (!");
                    }
                }
                if(readStatus != "findVector")
                {
                    if(current_chr == "{" || current_chr == "[" || current_chr == "(")
                    {
                        handy.exitMessage("Found start vector char {, [, or ( when read status isn't \"findVector\"!");
                    }
                }

                // this error part is a bit tricker because it has to do with only allowing one type
                // of vector start and end chars per line (so it uses the first type it finds for the whole line)
                // so these if statements are used again when using this status, but not for error stuff
                if(readStatus == "findVector")
                {
                    if(current_chr == "{" || current_chr == "[" || current_chr == "(")
                    {
                        if(foundMultiOptionValues.size() != 0)
                        {
                            if(current_chr != vectorStartChr)
                            {
                                handy.exitMessage("Second or later vector start char " + current_chr + " doesn't match first vector start char " + vectorStartChr + " for a configFile line!");
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
                            handy.exitMessage("Vector end char " + current_chr + " doesn't match vector start char " + vectorStartChr + "!");
                        }
                    }
                }
                if(readStatus == "foundValue")
                {
                    if(current_chr == "(" || current_chr == "[" || current_chr == "{" || current_chr == "}" || current_chr == "]" || current_chr == ")")
                    {
                        handy.exitMessage("Found vector char " + current_chr + " which is not allowed as part of a value! Probably forgot end quotation mark for a value!");
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
                            handy.debugMessage("Storing word " + foundWord + " into foundOptionName");
                            foundOptionNames.push_back(foundWord);
                            handy.debugMessage("foundOptionNames.size() = " + handy.intToString(foundOptionNames.size()));
                            foundWord = "";
                            readStatus = "findVector";
                        }
                    }
                }
                if(readStatus == "findVector" && changedStatus == false)
                {
                    if(current_chr == vectorStartChr)
                    {
                        // found the start of a vector, now look for values
                        readStatus = "findValue";
                        changedStatus = true;

                    } else if(current_chr != " " && current_chr != "=" && current_chr != "," && current_chr != ";")
                    {
                        // found the start of a new configOption. Yay!
                        // process the finished configOption
                        handy.debugMessage("Found end of optionName " + foundOptionNames[currentOptionName] + ". Adding foundMultiOptionValues to foundOptionValues");
                        foundOptionValues.push_back(foundMultiOptionValues);
                        handy.debugMessage("foundOptionValues.size() = " + handy.intToString(foundOptionValues.size()));

                        // now clean up and start the beginning process of the new config option
                        readStatus = "findName";
                        changedStatus = true;
                        foundWord = current_chr;
                        vectorStartChr = "\"value not assigned yet\"";
                        vectorEndChr = "\"value not assigned yet\"";
                        while(!foundMultiOptionValues.empty())
                        {
                            foundMultiOptionValues.pop_back();    // might have to do something tricky because vector of vectors; update: don't need to worry about inner vector unless using vector of pointers
                        }
                        while(!foundSingleOptionValue.empty())
                        {
                            foundSingleOptionValue.pop_back();
                        }
                        currentOptionName = currentOptionName + 1;
                    }
                }
                if(readStatus == "findValue" && changedStatus == false)
                {
                    if(current_chr == "\"")
                    {
                        readStatus = "foundValue";
                        changedStatus = true;
                    } else if(current_chr == vectorEndChr)
                    {
                        // found the end of a vector
                        if(foundWord != "")
                        {
                            // was not finished filling in a word, so forgot an end quotation mark
                            // this isn't really a great checker if there isn't a complete start vector char
                            handy.exitMessage("Missing end quotation mark for a value!");
                        } else
                        {
                            handy.debugMessage("Found the end of a vector. Storing foundSingleOptionValue vector into foundMultiOptionValues");
                            foundMultiOptionValues.push_back(foundSingleOptionValue);
                            handy.debugMessage("foundMultiOptionValues.size = " + handy.intToString(foundMultiOptionValues.size()));
                            readStatus = "findVector";
                            changedStatus = true;
                            // need to detect if it is the end of a file
                            // maybe just detect to make sure that this was the last step when it leaves the loop, if not, it didn't end on a vector!
                        }
                    } else if(current_chr != " " && current_chr != "=" && current_chr != "," && current_chr != ";")
                    {
                        //forgot the vector end char!
                        handy.exitMessage("found keyword in vector without quotation marks around it! Vector was probably missing end char!");
                    }
                }
                if(readStatus == "foundValue" && changedStatus == false)
                {
                    if(current_chr == "\"")
                    {
                        // found the end of the value, even if it is empty
                        handy.debugMessage("Storing word " + foundWord + " into foundSingleOptionValue");
                        foundSingleOptionValue.push_back(foundWord);
                        handy.debugMessage("foundSingleOptionValue.size() = " + handy.intToString(foundSingleOptionValue.size()));
                        foundWord = "";
                        readStatus = "findValue";
                        changedStatus = true;
                    } else
                    {
                        foundWord = foundWord + current_chr;
                    }
                }
            }
        }
    }
    // last char of file has to be a vector, so if the file was set up correctly,
    // status was findValue, stored the last vector of values, and changed status to findVector
    // so if it isn't currently findVector, the file did not end with a vector end char!!!
    // but the last set of values hasn't been stored yet either
    if(readStatus != "findVector")
    {
        handy.exitMessage("Last char of file was not an end vector char!");
    } else
    {
        handy.debugMessage("Found end of file when at end of a vector. Storing foundMultiOptionValues into foundOptionValues");
        foundOptionValues.push_back(foundMultiOptionValues);
        handy.debugMessage("foundOptionValues.size() = " + handy.intToString(foundOptionValues.size()));
    }
    handy.debugMessage("finished reading config file\n");   // extra \n because end of work

    // this is just handy debug information, maybe even more interesting than the above information
    handy.debugMessage("Looking at words before processing");
    handy.debugMessage("foundOptionNames.size() = " + handy.intToString(foundOptionNames.size()));
    handy.debugMessage("foundOptionValues.size() = " + handy.intToString(foundOptionValues.size()));
    for(size_t j = 0; j < foundOptionNames.size(); j++)
    {
        handy.debugMessage("foundOptionNames[" + handy.intToString(j) + "] = " + foundOptionNames[j]);
        handy.debugMessage("foundOptionValues[" + handy.intToString(j) + "].size() = " + handy.intToString(foundOptionValues[j].size()));
        for(size_t i = 0; i < foundOptionValues[j].size(); i++)
        {
            handy.debugMessage("foundOptionValues[" + handy.intToString(j) + "][" + handy.intToString(i) + "].size() = " + handy.intToString(foundOptionValues[j][i].size()));
            for(size_t k = 0; k < foundOptionValues[j][i].size(); k++)
            {
                handy.debugMessage("foundOptionValues[" + handy.intToString(j) + "][" + handy.intToString(i) + "][" + handy.intToString(k) + "] = " + foundOptionValues[j][i][k]);
            }
        }
    }
    handy.debugMessage("");     // just adding an extra endline

    // check to make sure the size indices match and process the words as new config options
    if(foundOptionNames.size() != foundOptionValues.size())
    {
        handy.exitMessage("foundOptionNames.size() " + handy.intToString(foundOptionNames.size()) + " != foundOptionValues.size() " + handy.intToString(foundOptionValues.size()));
    } else
    {
        processWords(foundOptionNames,foundOptionValues);
    }
}

void readConfigFile::processWords(std::vector<std::string> foundOptionNames, std::vector< std::vector< std::vector<std::string> > > foundOptionValues)
{
    // not sure if all these error checks are enough or even necessary, but keep them for now just in case
    for(size_t j = 0; j < foundOptionNames.size(); j++)
    {
        if(foundOptionNames[j] == "")
        {
            handy.exitMessage("processWords somehow run with empty foundOptionName!");
        }
    }
    for(size_t j = 0; j < foundOptionValues.size(); j++)
    {
        if(foundOptionValues[j].size() == 0)
        {
            // maybe should actually check to see if an empty variable places a "" value
            // then could have a variable checking that multiple configOptions have the same number of vectors
            // if that is even necessary, using a neededOptions variable similar to the conflictingOptions variable
            handy.message("foundOptionName = " + foundOptionNames[j] + ", but found no values for its foundOptionValues!");
            setupFail = true;
        }
    }
    // check for duplicate declared optionNames
    for(size_t j = 0; j < foundOptionNames.size()-1; j++)
    {
        for(size_t i = j+1; i < foundOptionNames.size(); i++)
        if(foundOptionNames[j] == foundOptionNames[i])
        {
            handy.exitMessage("Found duplicate options! foundOptionName[" + handy.intToString(j) + "] \"" + foundOptionNames[j] + "\" == foundOptionNames[" + handy.intToString(i) + "] \"" + foundOptionNames[i] + "\"!");
        }
    }

    // check to see if each option exists
    // if original number of values is number, update the number of values
    // then verify that the number of found values is correct
    // and upload the found values to the option values
    // then do a separate loop to make sure all the other values get updated
    handy.debugMessage("updating currentNumberOfValues for numeric originalNumberOfValues");
    for(size_t j = 0; j < foundOptionNames.size(); j++)
    {
        bool foundOption = false;
        for(size_t i = 0; i < theOptions.size(); i++)
        {
            if(foundOptionNames[j] == theOptions[i].get_optionName())
            {
                foundOption = true;
                updateNumberOfValues(i,false);
                // only do stuff with the options whose originalNumberOfValues are numeric strings
                if(handy.is_size_t(theOptions[i].get_optionOriginalNumberOfValues()) == true)
                {
                    // now that the number of values is updated, check to see if it is the right number of values then put them in the option
                    for(size_t k = 0; k < foundOptionValues[j].size(); k++)
                    {
                        handy.debugMessage("foundOptionValues[" + handy.intToString(j) + "][" + handy.intToString(k) + "].size() = " + handy.intToString(foundOptionValues[j][k].size()));
                        handy.debugMessage("theOptions[" + handy.intToString(i) + "].get_optionCurrentNumberOfValues() = " + handy.intToString(theOptions[i].get_optionCurrentNumberOfValues()));
                        if(foundOptionValues[j][k].size() != theOptions[i].get_optionCurrentNumberOfValues())
                        {
                            handy.message(theOptions[i].get_optionName() + " specified but wrong number of values for vector!");
                            setupFail = true;
                        } else
                        {
                            handy.splitDebugMessage("Inserting " + handy.intToString(foundOptionValues[j][k].size()) + " words into " + theOptions[i].get_optionName() + ":");
                            for(size_t jj = 0; jj < foundOptionValues[j][k].size(); jj++)
                            {
                                handy.splitDebugMessage(" \"" + foundOptionValues[j][k][jj] + "\"");
                                theOptions[i].addOptionValue(foundOptionValues[j][k][jj],k);
                            }
                            handy.debugMessage("");
                        }
                    }
                }
                break;
            }
        }
        if(foundOption == false)
        {
            handy.message("foundOptionNames[" + handy.intToString(j) + "] \"" + foundOptionNames[j] + "\" is not a valid option!");
        }
    }

    // now update number of values for those options whose originalNumberOfValues is a string
    // still need to make sure the option is found, but don't need to warn if a foundOptionName is not valid,
    // already gave that warning in above loop
    handy.debugMessage("\nupdating currentNumberOfValues for non-numeric originalNumberOfValues"); // extra linebreak since this section is so different from above
    for(size_t j = 0; j < foundOptionNames.size(); j++)
    {
        for(size_t i = 0; i < theOptions.size(); i++)
        {
            if(foundOptionNames[j] == theOptions[i].get_optionName())
            {
                updateNumberOfValues(i,true);
                // only do stuff with the options whose originalNumberOfValues are non-numeric strings
                if(handy.is_size_t(theOptions[i].get_optionOriginalNumberOfValues()) == false)
                {
                    // now that the number of values is updated, check to see if it is the right number of values then put them in the option
                    for(size_t k = 0; k < foundOptionValues[j].size(); k++)
                    {
                        handy.debugMessage("foundOptionValues[" + handy.intToString(j) + "][" + handy.intToString(k) + "].size() = " + handy.intToString(foundOptionValues[j][k].size()));
                        handy.debugMessage("theOptions[" + handy.intToString(i) + "].get_optionCurrentNumberOfValues() = " + handy.intToString(theOptions[i].get_optionCurrentNumberOfValues()));
                        if(foundOptionValues[j][k].size() != theOptions[i].get_optionCurrentNumberOfValues())
                        {
                            handy.message(theOptions[i].get_optionName() + " specified but wrong number of values for vector!");
                            setupFail = true;
                        } else
                        {
                            handy.splitDebugMessage("Inserting " + handy.intToString(foundOptionValues[j][k].size()) + " words into " + theOptions[i].get_optionName() + ":");
                            for(size_t jj = 0; jj < foundOptionValues[j][k].size(); jj++)
                            {
                                handy.splitDebugMessage(" \"" + foundOptionValues[j][k][jj] + "\"");
                                theOptions[i].addOptionValue(foundOptionValues[j][k][jj],k);
                            }
                            handy.debugMessage("");
                        }
                    }
                }
                break;
            }
        }
    }
}

void readConfigFile::updateNumberOfValues(size_t theOptionNumber, bool fillStrings)
{
    std::string originalNumberOfValues = theOptions[theOptionNumber].get_optionOriginalNumberOfValues();
    if(fillStrings == false)
    {
        if(handy.is_size_t(originalNumberOfValues) == true)
        {
            handy.debugMessage("Updating numberOfValues for " + theOptions[theOptionNumber].get_optionName() + " to: " + originalNumberOfValues);
            theOptions[theOptionNumber].updateNumberOfValues(handy.stringToInt(originalNumberOfValues));
            handy.debugMessage("theOptions[" + handy.intToString(theOptionNumber) + "].currentNumberOfValues = " + handy.intToString(theOptions[theOptionNumber].get_optionCurrentNumberOfValues()));
        }
    } else
    {
        if(handy.is_size_t(originalNumberOfValues) == false)
        {
            // look through past variables to see what number of values to use
            for(size_t ii = 0; ii < theOptions.size(); ii++)
            {
                if(theOptions[theOptionNumber].get_optionOriginalNumberOfValues() == theOptions[ii].get_optionName())
                {
                  // assumes the new value cannot be a string. If it somehow is, then some option initialization error checking needs updated; or need to adjust the whole method to allow to check series of values back to one that is a number instead of a string
                    if(theOptions[ii].get_optionCurrentNumberOfValues() == 0 || handy.is_size_t(theOptions[ii].get_optionOriginalNumberOfValues()) == false)
                    {
                        handy.message(theOptions[ii].get_optionName() + " not filled! Can't update currentNumberOfValues for: " + theOptions[theOptionNumber].get_optionName() + "!");
                        setupFail = true;
                    } else
                    {
                        // this requires that all variables that specify the next variable's size have to be only one vector and only one value
                        handy.debugMessage("theOptions[" + handy.intToString(ii) + "].get_optionName() = " + theOptions[ii].get_optionName());
                        handy.debugMessage("theOptions[" + handy.intToString(ii) + "].get_optionValues().size() = " + handy.intToString(theOptions[ii].get_optionValues().size()));
                        handy.debugMessage("theOptions[" + handy.intToString(ii) + "].get_optionValues()[0].size() = " + handy.intToString(theOptions[ii].get_optionValues()[0].size()));
                        if(theOptions[ii].get_optionValues().size() == 1 && theOptions[ii].get_optionValues()[0].size() == 1)
                        {
                            std::string s = theOptions[ii].get_optionValues()[0][0];
                            if(handy.is_size_t(s) == false)
                            {
                                handy.message("Could not update current number of values to: " + theOptions[ii].get_optionValues()[0][0] + ". The value is not numeric!");
                            } else
                            {
                                handy.debugMessage("Updating current number of options for " + theOptions[theOptionNumber].get_optionName() + " to: " + theOptions[ii].get_optionValues()[0][0]);
                                theOptions[theOptionNumber].updateNumberOfValues(handy.stringToInt(s));
                            }
                        } else
                        {
                            handy.exitMessage(theOptions[ii].get_optionName() + " number of values is greater than 1! Can't update currentNumberOfValues for: " + theOptions[theOptionNumber].get_optionName() + "!");
                        }
                    }
                    break;
                }
            }
        }
    }
}

void readConfigFile::checkConflictingOptions()
{
    // so for now set them all to false (which is already done with reset options)
    // but at some point need to change this so it sets some of these to true if the options conflict
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        theOptions[i].updateOptionConflicts(false);
    }
}

void readConfigFile::checkVariableFill()
{
    handy.message("verifying variable specification and value assignment");
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionConflicts() == true)
        {
            // make sure it doesn't have even a single value
            if(theOptions[i].get_optionCurrentNumberOfValues() != 0 || theOptions[i].get_optionValues().size() != 0)
            {
                handy.message("the " + theOptions[i].get_optionName() + " option conflicts! It should have no values!");
                setupFail = true;
            }
        } else
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() == 0)
            {
                handy.message("NumberOfValues for " + theOptions[i].get_optionName() + " didn't get filled!");
                setupFail = true;
            }

            // create a temporary variable to act as an alias
            std::vector<std::vector<std::string> > currentOptionValues = theOptions[i].get_optionValues();
            for(size_t k = 0; k < currentOptionValues.size(); k++)
            {
                for(size_t j = 0; j < currentOptionValues[k].size(); j++)
                {
                    if(currentOptionValues[k][j] == "") //nothing that isn't a conflicting option can be empty
                    {
                        handy.message("Values for " + theOptions[i].get_optionName() + " index [" + handy.intToString(k) + "][" + handy.intToString(j) + "] not specified!");
                        setupFail = true;
                    }
                }
            }
        }
    }
}

std::string readConfigFile::get_configFilePath()
{
    return configFilePath;
}

// need to rework all the get option values of single and multi int and string, but first need to adapt smoke transport
// everything above here except conflicting options seems to be working correctly. Still need to error test the config file stuff
// could also probably use work tightening up messages and debug info so less wordy

int readConfigFile::get_optionValues_singleInt(std::string desiredOptionName)
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionName() == desiredOptionName)
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() != 1)
            {
                handy.exitMessage("Requested single int value for " + desiredOptionName + " when there are currently " + handy.intToString(theOptions[i].get_optionCurrentNumberOfValues()) + " values!");
                return -1;
            } else
            {
                return handy.stringToInt(theOptions[i].get_optionValues()[0][0]);
            }
        }
    }
    handy.exitMessage(desiredOptionName + " is not a valid option!");
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
                theOptionValues.push_back(handy.stringToInt(theOptions[i].get_optionValues()[0][j]));
            }
            return theOptionValues;
        }
    }
    handy.exitMessage(desiredOptionName + " is not a valid option!");
    return std::vector<int> ();
}

std::string readConfigFile::get_optionValues_singleString(std::string desiredOptionName)
{
    for(size_t i = 0; i < theOptions.size(); i++)
    {
        if(theOptions[i].get_optionName() == desiredOptionName)
        {
            if(theOptions[i].get_optionCurrentNumberOfValues() != 1)
            {
                handy.exitMessage("Requested single int value for " + desiredOptionName + " when there are currently " + handy.intToString(theOptions[i].get_optionCurrentNumberOfValues()) + " values!");
                return "";
            } else
            {
                return theOptions[i].get_optionValues()[0][0];
            }
        }
    }
    handy.exitMessage(desiredOptionName + " is not a valid option!");
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
    handy.exitMessage(desiredOptionName + " is not a valid option!");
    return std::vector<std::string> ();
}
