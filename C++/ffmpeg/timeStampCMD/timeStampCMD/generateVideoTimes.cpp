#include "generateVideoTimes.h"

generateVideoTimes::generateVideoTimes()
{
    /*
     * don't need to do anything here. The user needs to call
     * the other functions to do stuff with this class.
     * So this script should not do more than one set of files at a time
     */
}

void generateVideoTimes::fillVideoTimes(double secondFrame, double firstFrame, std::string secondTime, std::string firstTime, double lastFrame)
{
    std::cout << "generating video times for timestamps\n";
    checkFillVideoTimesInput(secondFrame,firstFrame,secondTime,firstTime,lastFrame);
    std::string timeDifference = subtractTime(secondTime,firstTime);
    timeInterval = divideTimeByDouble(timeDifference,(secondFrame-firstFrame));
    //fill first one with the firstTime variable before the loop
     //so this is 0 time plus time interval. there is no 0 frame, which would be 0 time,
     //so frame 1 is 0 time plus time interval, which is the time interval
    generatedVideoTimes.push_back(timeInterval);
    for(double i = 1; i < lastFrame; i++)   //notice that this starts one later to avoid problems with the addTime function call in the loop
    {
        std::string generatedTime = addTime(generatedVideoTimes[i-1],timeInterval);
        generatedVideoTimes.push_back(generatedTime);
    }
    std::cout << "finished generating video times for timestamps\n";
}

void generateVideoTimes::checkFillVideoTimesInput(double secondFrame, double firstFrame, std::string secondTime, std::string firstTime, double lastFrame)
{
    std::cout << "checking inputs for fillVideoTimes in generateVideoTimes class\n";
    bool failed = false;
    if(firstFrame < 0)
    {
        std::cout << "Error in fillVideoTimes function in generateVideoTimes class! firstFrame < 0!\n";
        failed = true;
    }
    if(secondFrame < 0)
    {
        std::cout << "Error in fillVideoTimes function in generateVideoTimes class! secondFrame < 0!\n";
        failed = true;
    }
    if(secondFrame <= firstFrame)
    {
        std::cout << "Error in fillVideoTimes function in generateVideoTimes class! secondFrame <= firstFrame!\n";
        failed = true;
    }
    if(lastFrame <= firstFrame)
    {
        std::cout << "Error in fillVideoTimes function in generateVideoTimes class! lastFrame <= firstFrame!\n";
        failed = true;
    }
    if(lastFrame <= secondFrame)
    {
        std::cout << "Error in fillVideoTimes function in generateVideoTimes class! lastFrame <= secondFrame!\n";
        failed = true;
    }
    if(firstTime.length() != 11)
    {
        std::cout << "Error in fillVideoTimes function in generateVideoTimes class! firstTime.length() != 11!\n";
        failed = true;
    }
    if(secondTime.length() != 11)
    {
        std::cout << "Error in fillVideoTimes function in generateVideoTimes class! secondTime.length() != 11!\n";
        failed = true;
    }

    for(double i = 0; i < firstTime.length(); i++)
    {
        std::string chr = firstTime.substr(i,1);
        if(i == 2 || i == 5)
        {
            if(chr != ":")
            {
                std::cout << "Error in fillVideoTimes function in generateVideoTimes class! firstTime is wrong format!\n";
                failed = true;
            }
        } else if(i == 8)
        {
            if(chr != ".")
            {
                std::cout << "Error in fillVideoTimes function in generateVideoTimes class! firstTime is wrong format!\n";
                failed = true;
            }
        } else
        {
            if(is_numeric(chr.c_str()) != true)
            {
                std::cout << "Error in fillVideoTimes function in generateVideoTimes class! firstTime is wrong format!\n";
                failed = true;
            }
        }
    }

    for(double i = 0; i < secondTime.length(); i++)
    {
        std::string chr = secondTime.substr(i,1);
        if(i == 2 || i == 5)
        {
            if(chr != ":")
            {
                std::cout << "Error in fillVideoTimes function in generateVideoTimes class! secondTime is wrong format!\n";
                failed = true;
            }
        } else if(i == 8)
        {
            if(chr != ".")
            {
                std::cout << "Error in fillVideoTimes function in generateVideoTimes class! secondTime is wrong format!\n";
                failed = true;
            }
        } else
        {
            if(is_numeric(chr.c_str()) != true)
            {
                std::cout << "Error in fillVideoTimes function in generateVideoTimes class! secondTime is wrong format!\n";
                failed = true;
            }
        }
    }

    std::cout << "finished checking inputs\n";
    if(failed == true)
    {
        std::cout << "Failed Inputs for fillVideoTimes function in generateVideoTimes class\n";
        std::exit(EXIT_FAILURE);
    }
}

std::string generateVideoTimes::subtractTime(std::string timeBase, std::string timeToSubtract)
{
    std::cout << "subtracting time\n";
    //set up needed variables
    std::string calculatedTime;

    std::cout << "timeBase = " << timeBase << "\n";
    std::cout << "timeToSubtract = " << timeToSubtract << "\n";
    std::string hours_base_string = timeBase.substr(0,2);
    std::string mins_base_string = timeBase.substr(3,2);
    std::string secs_base_string = timeBase.substr(6,2);
    std::string microsecs_base_string = timeBase.substr(9,2);
    double hours_base_int = stringToInt(hours_base_string);
    double mins_base_int = stringToInt(mins_base_string);
    double secs_base_int = stringToInt(secs_base_string);
    double microsecs_base_int = stringToInt(microsecs_base_string);

    std::string hours_subtract_string = timeToSubtract.substr(0,2);
    std::string mins_subtract_string = timeToSubtract.substr(3,2);
    std::string secs_subtract_string = timeToSubtract.substr(6,2);
    std::string microsecs_subtract_string = timeToSubtract.substr(9,2);
    double hours_subtract_int = stringToInt(hours_subtract_string);
    double mins_subtract_int = stringToInt(mins_subtract_string);
    double secs_subtract_int = stringToInt(secs_subtract_string);
    double microsecs_subtract_int = stringToInt(microsecs_subtract_string);

    double hours_sum_int = 0;
    double mins_sum_int = 0;
    double secs_sum_int = 0;
    double microsecs_sum_int = 0;

    //now compute the time
    if(hours_base_int - hours_subtract_int < 0)
    {
        hours_sum_int = hours_base_int - hours_subtract_int + 24;
    } else
    {
        hours_sum_int = hours_base_int - hours_subtract_int;
    }

    if(mins_base_int - mins_subtract_int < 0)
    {
        mins_sum_int = mins_base_int - mins_subtract_int + 60;
    } else
    {
        mins_sum_int = mins_base_int - mins_subtract_int;
    }

    if(secs_base_int - secs_subtract_int < 0)
    {
        secs_sum_int = secs_base_int - secs_subtract_int + 60;
    } else
    {
        secs_sum_int = secs_base_int - secs_subtract_int;
    }

    if(microsecs_base_int - microsecs_subtract_int < 0)
    {
        microsecs_sum_int = microsecs_base_int - microsecs_subtract_int + 100;
    } else
    {
        microsecs_sum_int = microsecs_base_int - microsecs_subtract_int;
    }

    calculatedTime = dblToString(hours_sum_int) + ":" + dblToString(mins_sum_int) + ":"
            + dblToString(secs_sum_int) + "." + dblToString(microsecs_sum_int);
    std::cout << "calculatedTime = " << calculatedTime << "\n";

    std::cout << "finished subtracting time\n";
    return calculatedTime;
}

std::string generateVideoTimes::divideTimeByDouble(std::string timevalue, double doublevalue)
{
    std::cout << "dividing time by double\n";
    //set up needed variables
    std::string calculatedTime;

    std::cout << "time value = " << timevalue << "\n";
    std::cout << "double value = " << doublevalue << "\n";
    std::string hours_string = timevalue.substr(0,2);
    std::string mins_string = timevalue.substr(3,2);
    std::string secs_string = timevalue.substr(6,2);
    std::string microsecs_string = timevalue.substr(9,2);
    int hours_int = stringToInt(hours_string);
    int mins_int = stringToInt(mins_string);
    int secs_int = stringToInt(secs_string);
    int microsecs_int = stringToInt(microsecs_string);

    int hours_sum_int = 0;
    int mins_sum_int = 0;
    int secs_sum_int = 0;
    int microsecs_sum_int = 0;

    //now compute the time
    if(doublevalue == 0)
    {
        std::cout << "Error in divideTimeByDouble in generateVideoTimes class! doublevalue == 0!\n";
        std::exit(EXIT_FAILURE);
    }

    double sum = hours_int*24.0+mins_int*60.0+secs_int*60.0+microsecs_int/100.0;
    std::cout << "sum of all seconds in time = " << sum << "\n";
    sum = sum / doublevalue;
    std::cout << "sum after dividing by double value = " << sum << "\n";
    hours_sum_int = sum / 24.0;
    std::cout << "hours calculated = " << hours_sum_int << "\n";
    sum = sum - hours_sum_int * 60.0;
    std::cout << "sum after subtracting hours in seconds = " << sum << "\n";
    mins_sum_int = sum / 60.0;
    std::cout << "mins calculated = " << mins_sum_int << "\n";
    sum = sum - mins_sum_int * 60.0;
    std::cout << "sum after subtracting mins in seconds = " << sum << "\n";
    secs_sum_int = sum / 60.0;
    std::cout << "seconds calculated = " << secs_sum_int << "\n";
    sum = sum - secs_sum_int * 60.0;
    std::cout << "sum after subtracting seconds in seconds = " << sum << "\n";
    microsecs_sum_int = sum * 100.0;
    std::cout << "microseconds calculated = " << microsecs_sum_int << "\n";

    calculatedTime = dblToString(hours_sum_int) + ":" + dblToString(mins_sum_int) + ":"
            + dblToString(secs_sum_int) + "." + dblToString(microsecs_sum_int);
    std::cout << "calculatedTime = " << calculatedTime << "\n";

    std::cout << "finished dividing time by double\n";
    return calculatedTime;
}

std::string generateVideoTimes::addTime(std::string timeBase, std::string timeToAdd)
{
    std::cout << "adding time\n";
    //set up needed variables
    std::string calculatedTime;

    std::cout << "time base = " << timeBase << "\n";
    std::cout << "timeToAdd = " << timeToAdd << "\n";
    std::string hours_base_string = timeBase.substr(0,2);
    std::string mins_base_string = timeBase.substr(3,2);
    std::string secs_base_string = timeBase.substr(6,2);
    std::string microsecs_base_string = timeBase.substr(9,2);
    double hours_base_int = stringToInt(hours_base_string);
    double mins_base_int = stringToInt(mins_base_string);
    double secs_base_int = stringToInt(secs_base_string);
    double microsecs_base_int = stringToInt(microsecs_base_string);

    std::string hours_add_string = timeToAdd.substr(0,2);
    std::string mins_add_string = timeToAdd.substr(3,2);
    std::string secs_add_string = timeToAdd.substr(6,2);
    std::string microsecs_add_string = timeToAdd.substr(9,2);
    double hours_add_int = stringToInt(hours_add_string);
    double mins_add_int = stringToInt(mins_add_string);
    double secs_add_int = stringToInt(secs_add_string);
    double microsecs_add_int = stringToInt(microsecs_add_string);

    double hours_sum_int = 0;
    double mins_sum_int = 0;
    double secs_sum_int = 0;
    double microsecs_sum_int = 0;

    //now compute the time
    if(microsecs_base_int + microsecs_add_int >= 100)
    {
        microsecs_sum_int = microsecs_base_int + microsecs_add_int - 100;
        secs_sum_int = secs_sum_int+1;
    } else
    {
        microsecs_sum_int = microsecs_base_int + microsecs_add_int;
    }

    if(secs_base_int + secs_add_int + secs_sum_int >= 60)
    {
        secs_sum_int = secs_base_int + secs_add_int + secs_sum_int - 60;
        mins_sum_int = mins_sum_int + 1;
    } else
    {
        secs_sum_int = secs_base_int + secs_add_int + secs_sum_int;
    }

    if(mins_base_int + mins_add_int + mins_sum_int >= 60)
    {
        mins_sum_int = mins_base_int + mins_add_int + mins_sum_int - 60;
        hours_sum_int = hours_sum_int + 1;
    } else
    {
        mins_sum_int = mins_base_int + mins_add_int + mins_sum_int;
    }

    if(hours_base_int + hours_add_int + hours_sum_int >= 24)
    {
        hours_sum_int = hours_base_int + hours_add_int + hours_sum_int - 24;
        //need to add something to handle the change in the date here
        //or have a function that doesn't care how this is done that goes through
        //and generates the dates for each time.
        //oh, actually this doesn't care about the date. It is video times, not timestamps
        //and the video times need to not have dates. So we are good I think.
    } else
    {
        hours_sum_int = hours_base_int + hours_add_int + hours_sum_int;
    }

    calculatedTime = dblToString(hours_sum_int) + ":" + dblToString(mins_sum_int) + ":"
            + dblToString(secs_sum_int) + "." + dblToString(microsecs_sum_int);
    std::cout << "calculatedTime = " << calculatedTime << "\n";

    std::cout << "finished adding time\n";
    return calculatedTime;
}

std::vector<std::string> generateVideoTimes::getVideoTimes()
{
    if(generatedVideoTimes.size() == 0)
    {
        std::cout << "Error in function getVideoTime in generateVideoTimes class! " <<
                     "generatedVideoTimes in this class is empty still! Call fillVideoTimes first!\n";
        std::exit(EXIT_FAILURE);
    }
    return generatedVideoTimes;
}

bool generateVideoTimes::is_numeric(char const *string)
{
    int sizeOfString = strlen(string);
        int iteration = 0;
        bool isNumeric = true;

        while(iteration < sizeOfString)
        {
            if(!isdigit(string[iteration]))
            {
                isNumeric = false;
                break;
            }

            iteration++;

        }

        return isNumeric;
}

int generateVideoTimes::stringToInt(std::string value)
{
    int Int = boost::lexical_cast<int>(value);
    return Int;
}

std::string generateVideoTimes::dblToString(double value)
{
    std::string str;
    if(value < 10)
    {
        str = "0";
    }
    str = str + boost::lexical_cast<std::string>(value);

    return str;
}
