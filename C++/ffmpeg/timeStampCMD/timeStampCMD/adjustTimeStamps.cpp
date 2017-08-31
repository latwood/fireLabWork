#include "adjustTimeStamps.h"

adjustTimeStamps::adjustTimeStamps()
{
    /*
     * don't need to do anything here. The user needs to call
     * the other functions to do stuff with this class.
     * So this script should not do more than one set of files at a time
     */
}

std::vector<std::string> adjustTimeStamps::adjustTime(std::string timeCorrector,std::vector<std::string> timeStamps)
{
    std::cout << "adjusting timestamps to correct timezone\n";
    std::cout << "timeCorrector = " << timeCorrector << "\n";
    checkInputs(timeCorrector,timeStamps);  //notice this assigns timeCorrector variables
    std::cout << "timeCorrector.substr(0,1) = " << timeCorrector.substr(0,1) << "\n";
    if(timeCorrector.substr(0,1) == "-")
    {
        for(double i = 0; i < timeStamps.size(); i++)
        {
            std::string generatedTime = subtractTime(timeCorrector,timeStamps[i]);
            correctedTimeStamps.push_back(generatedTime);
        }
    } else if (timeCorrector.substr(0,1) == "+")
    {
        for(double i = 0; i < timeStamps.size(); i++)
        {
            std::string generatedTime = addTime(timeCorrector,timeStamps[i]);
            correctedTimeStamps.push_back(generatedTime);
        }
    } else
    {
        std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector not \"+\" or \"-\"!\n";
        std::exit(EXIT_FAILURE);
    }
    std::cout << "finished adjusting timestamps to correct timezone";
    return correctedTimeStamps;
}

void adjustTimeStamps::checkInputs(std::string timeCorrector,std::vector<std::string> timeStamps)
{
    std::cout << "checking inputs for adjustTime in adjustTimeStamps class\n";
    bool failed = false;
    if(timeCorrector.length() != 20)
    {
        std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector.length() != 20!\n";
        failed = true;
    }

    for(double i = 0; i < timeCorrector.length(); i++)
    {
        std::string chr = timeCorrector.substr(i,1);
        if(i == 0)
        {
            if(chr != "+" && chr != "-")
            {
                std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector not \"+\" or \"-\"!\n";
                failed = true;
            }
        } else if(i == 5 || i == 8 || i == 14 || i == 17)
        {
            if(chr != ":")
            {
                std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector is wrong format!\n";
                failed = true;
            }
        } else if(i == 11)
        {
            if(chr != " ")
            {
                std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector is wrong format!\n";
                failed = true;
            }
        } else
        {
            if(is_numeric(chr.c_str()) != true)
            {
                std::cout << "Error in adjustTime function in adjustTimeStamps class! currentTimeStamp is wrong format!\n";
                failed = true;
            }
        }
    }
    timeCorrector_year_string = timeCorrector.substr(1,4);
    timeCorrector_year_int = stringToInt(timeCorrector_year_string);
    if(timeCorrector_year_int < 0 || timeCorrector_year_int > 9999) //hmm, technically this is already done by limiting the format to 4 digits. And does a negative sign fail the isnumeric? And this won't catch if the addition of the year adds too many, so that will only be caught in the addition or subtraction stuff
    {
        std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector year out of range!\n";
        failed = true;
    }
    timeCorrector_month_string = timeCorrector.substr(6,2);
    timeCorrector_month_int = stringToInt(timeCorrector_month_string);
    if(timeCorrector_month_int < 0 || timeCorrector_month_int >= 12 )
    {
        std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector month out of range! Use a value less than 12!\n";
        failed = true;
    }
    timeCorrector_day_string = timeCorrector.substr(9,2);
    timeCorrector_day_int = stringToInt(timeCorrector_day_string);
    if(timeCorrector_day_int < 0 || timeCorrector_day_int >= 31)
    {
        std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector day out of range! Use a value less than 31!\n";
        failed = true;
    }
    timeCorrector_hour_string = timeCorrector.substr(12,2);
    timeCorrector_hour_int = stringToInt(timeCorrector_hour_string);
    if(timeCorrector_hour_int < 0 || timeCorrector_hour_int >= 24)
    {
        std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector hour out of range! Use a value less than 24!\n";
        failed = true;
    }
    timeCorrector_min_string = timeCorrector.substr(15,2);
    timeCorrector_min_int = stringToInt(timeCorrector_min_string);
    if(timeCorrector_min_int < 0 || timeCorrector_min_int >= 60)
    {
        std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector min out of range! Use a value less than 60!\n";
        failed = true;
    }
    timeCorrector_sec_string = timeCorrector.substr(18,2);
    timeCorrector_sec_int = stringToInt(timeCorrector_sec_string);
    if(timeCorrector_sec_int < 0 || timeCorrector_sec_int >= 60)
    {
        std::cout << "Error in adjustTime function in adjustTimeStamps class! timeCorrector sec out of range! Use a value less than 60!\n";
        failed = true;
    }
    //hmm, haven't done this kind of error checking for the other functions. Need to add this at some point in time

    for(double j = 0; j < timeStamps.size(); j++)
    {
        std::string currentTimeStamp = timeStamps[j];
        if(currentTimeStamp.length() != 23)
        {
            std::cout << "Error in adjustTime function in adjustTimeStamps class! currentTimeStamp.length() != 23!\n";
            failed = true;
        }

        for(double i = 0; i < currentTimeStamp.length(); i++)
        {
            std::string chr = currentTimeStamp.substr(i,1);
            if(i == 4 || i == 8 || i == 15 || i == 19)
            {
                if(chr != "\\")
                {
                    std::cout << "Error in adjustTime function in adjustTimeStamps class! currentTimeStamp is wrong format!\n";
                    failed = true;
                }
            } else if(i == 5 || i == 9 || i == 16 || i == 20)
            {
                if(chr != ":")
                {
                    std::cout << "Error in adjustTime function in adjustTimeStamps class! currentTimeStamp is wrong format!\n";
                    failed = true;
                }
            } else if(i == 12)
            {
                if(chr != " ")
                {
                    std::cout << "Error in adjustTime function in adjustTimeStamps class! currentTimeStamp is wrong format!\n";
                    failed = true;
                }
            } else
            {
                if(is_numeric(chr.c_str()) != true)
                {
                    std::cout << "Error in adjustTime function in adjustTimeStamps class! currentTimeStamp is wrong format!\n";
                    failed = true;
                }
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

std::string adjustTimeStamps::subtractTime(std::string timeCorrector,std::string originalTime)
{
    std::cout << "subtracting time\n";
    //set up needed variables
    std::string calculatedTime;

    std::cout << "timeCorrector = " << timeCorrector << "\n";
    std::cout << "originalTime = " << originalTime << "\n";
    std::string year_string = originalTime.substr(0,4);
    std::string month_string = originalTime.substr(6,2);
    std::string day_string = originalTime.substr(10,2);
    std::string hours_string = originalTime.substr(13,2);
    std::string mins_string = originalTime.substr(17,2);
    std::string secs_string = originalTime.substr(21,2);
    int year_int = stringToInt(year_string);
    int month_int = stringToInt(month_string);
    int day_int = stringToInt(day_string);
    int hours_int = stringToInt(hours_string);
    int mins_int = stringToInt(mins_string);
    int secs_int = stringToInt(secs_string);

    int year_sum_int = 0;
    int month_sum_int = 0;
    int day_sum_int = 0;
    int hours_sum_int = 0;
    int mins_sum_int = 0;
    int secs_sum_int = 0;

    //now compute the time
    //notice that the input checker makes sure there can't be more than 24 hours time change
    /*
     * while this is true, the problem I'm seeing is the difficulty of handling the seconds adding up to affect the minutes,
     * the minutes adding up to affect the hours, and so on. The subtract time in generateVideoTimes doesn't do that,
     * so each of the different parts are independent. minutes, seconds, and hours are independent there, but not here
     * and this makes it a heck of a ton more complicated. In fact, it should be independent, since the corrector
     * should not be able to go more than a given amount of change for each part.
     *
     * So should rewrite this to make sure that the seconds only affect the seconds,
     * the minutes only affect the minutes, and so on.
     *
     * there have to be error checking to allow the months to affect the days, in the sense that
     * if a month is changed, you don't want to have too many days now with the date.
     *
     * hm, even if the idea is to only affect the first timestamp by a maximum ammount, still need to do rollover
     * effects from each type of time to the next part. Man dates are a pain lol.
     *
     * for now, this at least works, it just needs checked to make sure it worked every time
     *
     *
     * Okay, I have a separate git repository with the old one for just in case. I'm thinking, allow
     * maximum values like 9999 years, 99 months, 99 days, 99 hours, 99 mins, 99 seconds. However,
     * a warning will be given that it only increments a given type of time, ie hours affect hours, mins affect mins
     * but there is zero roll over. While the user is allowed to specify values bigger than the amount
     * before rollover for each given time unit, if when adding or subtracting rollover is found, the
     * adjustment will cap out at the min and max values possible for a given adjustment that avoids rollover.
     * This means that if the time is 0 hours, and a time of 24 hours or greater is specified,
     * the hours will become 23 hours, which is the max value possible for hours. Seconds max is 59, same with mins
     * the max for years is 9999. The min for all is 0. The max month is 12.
     *
     * The max days depends on the month and year that is calculated, not the original month or year value.
     * So if the original time has february 2000, 0 days; if the corrector is to add 3 months, 3 years, and 40 days
     * the result will be may 31st, 2003. Not something like february 28 2003 or may 29 2003.
     *
     */
    if(secs_int - timeCorrector_sec_int < 0)
    {
        secs_sum_int = secs_int - timeCorrector_sec_int + 60;
        mins_sum_int = mins_sum_int - 1;
    } else
    {
        secs_sum_int = secs_int - timeCorrector_sec_int;
    }


    if(mins_int - timeCorrector_min_int + mins_sum_int < 0)
    {
        mins_sum_int = mins_int - timeCorrector_min_int + mins_sum_int + 60;
        hours_sum_int = hours_sum_int - 1;
    } else
    {
        mins_sum_int = mins_int - timeCorrector_min_int + mins_sum_int;
    }

    if(hours_int - timeCorrector_hour_int + hours_sum_int < 0)
    {
        hours_sum_int = hours_int - timeCorrector_hour_int + hours_sum_int + 24;    //23 - 24 = -1, so 23
        day_sum_int = day_sum_int - 1;
    } else
    {
        hours_sum_int = hours_int - timeCorrector_hour_int + hours_sum_int;
    }

    if(day_int - timeCorrector_day_int + day_sum_int < 1)
    {
        //need to correct day based off of the month change.
        //january, march, may, july, august, october, and december have 31 days
        //april, june, september, and november have 30 days
        //february has 29 days every four years, and 28 days the rest
        //so calculate month first, then year, then days
        if(month_int - timeCorrector_month_int - 1 < 1)
        {
            month_sum_int = month_int - timeCorrector_month_int - 1 + 12;
            year_sum_int = year_sum_int - 1;
        } else
        {
            month_sum_int = month_int - timeCorrector_month_int - 1;
        }

        if(year_int - timeCorrector_year_int + year_sum_int < 1)
        {
            std::cout << "Error in subtractTime function in adjustTimeStamps class! timeCorrector bigger than originalTime! Can't go earlier than 1 A.D.!\n";
            std::exit(EXIT_FAILURE);
        } else
        {
            year_sum_int = year_int - timeCorrector_year_int + year_sum_int;
        }

        if(month_sum_int == 1 || month_sum_int == 3 || month_sum_int == 5 || month_sum_int == 7
                || month_sum_int == 8 || month_sum_int == 10 || month_sum_int == 12)
        {
            day_sum_int = day_int - timeCorrector_day_int + day_sum_int + 31;
        } else if(month_sum_int == 4 || month_sum_int == 6 || month_sum_int == 9 || month_sum_int == 11)
        {
            day_sum_int = day_int - timeCorrector_day_int + day_sum_int + 30;
        } else if(month_int - 1 == 2)
        {
            if(year_sum_int % 4 == 0)
            {
                day_sum_int = day_int - timeCorrector_day_int + day_sum_int + 29;
            } else
            {
                day_sum_int = day_int - timeCorrector_day_int + day_sum_int + 28;
            }
        } else
        {
            std::cout << "Error in subtractTime function in adjustTimeStamps class! "
                      << "Month is somehow out of range of 1 to 12!\n";
            std::exit(EXIT_FAILURE);
        }
    } else
    {
        day_sum_int = day_int - timeCorrector_day_int + day_sum_int;

        if(month_int - timeCorrector_month_int < 1)
        {
            month_sum_int = month_int - timeCorrector_month_int - 1 + 12;
            year_sum_int = year_sum_int - 1;
        } else
        {
            month_sum_int = month_int - timeCorrector_month_int;
        }

        if(year_int - timeCorrector_year_int + year_sum_int < 1)
        {
            std::cout << "Error in subtractTime function in adjustTimeStamps class! timeCorrector bigger than originalTime! Can't go earlier than 1 A.D.!\n";
            std::exit(EXIT_FAILURE);
        } else
        {
            year_sum_int = year_int - timeCorrector_year_int + year_sum_int;
        }
    }

    if(year_sum_int < 100)
    {
        calculatedTime = "00" + dblToString(year_sum_int) + "\\:" + dblToString(month_sum_int) + "\\:"
            + dblToString(day_sum_int) + " " + dblToString(hours_sum_int) + "\\:"
            + dblToString(mins_sum_int) + "\\:" + dblToString(secs_sum_int);
    } else if(year_sum_int < 1000)
    {
        calculatedTime = "0" + dblToString(year_sum_int) + "\\:" + dblToString(month_sum_int) + "\\:"
            + dblToString(day_sum_int) + " " + dblToString(hours_sum_int) + "\\:"
            + dblToString(mins_sum_int) + "\\:" + dblToString(secs_sum_int);
    } else
    {
        calculatedTime = dblToString(year_sum_int) + "\\:" + dblToString(month_sum_int) + "\\:"
            + dblToString(day_sum_int) + " " + dblToString(hours_sum_int) + "\\:"
            + dblToString(mins_sum_int) + "\\:" + dblToString(secs_sum_int);
    }
    std::cout << "calculatedTime = " << calculatedTime << "\n";

    std::cout << "finished subtracting time\n";
    return calculatedTime;
}

std::string adjustTimeStamps::addTime(std::string timeCorrector,std::string originalTime)
{
    std::cout << "adding time\n";
    //set up needed variables
    std::string calculatedTime;

    std::cout << "timeCorrector = " << timeCorrector << "\n";
    std::cout << "originalTime = " << originalTime << "\n";
    std::string year_string = originalTime.substr(0,4);
    std::string month_string = originalTime.substr(6,2);
    std::string day_string = originalTime.substr(10,2);
    std::string hours_string = originalTime.substr(13,2);
    std::string mins_string = originalTime.substr(17,2);
    std::string secs_string = originalTime.substr(21,2);
    int year_int = stringToInt(year_string);
    int month_int = stringToInt(month_string);
    int day_int = stringToInt(day_string);
    int hours_int = stringToInt(hours_string);
    int mins_int = stringToInt(mins_string);
    int secs_int = stringToInt(secs_string);

    int year_sum_int = 0;
    int month_sum_int = 0;
    int day_sum_int = 0;
    int hours_sum_int = 0;
    int mins_sum_int = 0;
    int secs_sum_int = 0;

    //now compute the time
    //notice that the input checker makes sure there can't be more than 24 hours time change
    if(secs_int + timeCorrector_sec_int >= 60)
    {
        secs_sum_int = secs_int + timeCorrector_sec_int - 60;
        mins_sum_int = mins_sum_int + 1;
    } else
    {
        secs_sum_int = secs_int + timeCorrector_sec_int;
    }


    if(mins_int + timeCorrector_min_int + mins_sum_int >= 60)
    {
        mins_sum_int = mins_int + timeCorrector_min_int + mins_sum_int - 60;
        hours_sum_int = hours_sum_int + 1;
    } else
    {
        mins_sum_int = mins_int + timeCorrector_min_int + mins_sum_int;
    }

    if(hours_int + timeCorrector_hour_int + hours_sum_int >= 24)
    {
        hours_sum_int = hours_int + timeCorrector_hour_int + hours_sum_int - 24;
        day_sum_int = day_sum_int + 1;
    } else
    {
        hours_sum_int = hours_int + timeCorrector_hour_int + hours_sum_int;
    }

    //hmm, can't even do the same initial comparison with day without doing a computation for the month
    //and possibly the year
    //So I'm going to calculate the month and year with what I've got, then correct them if I need to
    //and see what I get
    if(month_int + timeCorrector_month_int >= 12)
    {
        month_sum_int = month_int + timeCorrector_month_int - 12;
        year_sum_int = year_sum_int + 1;
    } else
    {
        month_sum_int = month_int + timeCorrector_month_int;
    }
    year_sum_int = year_int + timeCorrector_year_int + year_sum_int;

    //need to correct day based off of the month change.
    //january, march, may, july, august, october, and december have 31 days
    //april, june, september, and november have 30 days
    //february has 29 days every four years, and 28 days the rest
    //hmm so do an if statement for each month of so many days, but if the amount is exceeded,
    //do an if statement breaking up what to do depending on which set of months have which one behind them
    int daycount = day_int + timeCorrector_day_int + day_sum_int;

    //so the days, months, and years are technically already calculated,
    //so they are already set unless they need adjusted
    if(month_sum_int == 1)
    {
        if(daycount > 31)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 31;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 2)
    {
        if(year_sum_int % 4 == 0)
        {
            if(daycount > 29)
            {
                month_sum_int = month_sum_int + 1;
                day_sum_int = daycount - 29;
            } else if(daycount > 28)
            {
                month_sum_int = month_sum_int + 1;
                day_sum_int = daycount - 28;
            } else
            {
                day_sum_int = daycount;
            }
        }
    } else if(month_sum_int == 3)
    {
        if(daycount > 31)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 31;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 4)
    {
        if(daycount > 30)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 30;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 5)
    {
        if(daycount > 31)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 31;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 6)
    {
        if(daycount > 30)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 30;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 7)
    {
        if(daycount > 31)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 31;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 8)
    {
        if(daycount > 31)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 31;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 9)
    {
        if(daycount > 30)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 30;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 10)
    {
        if(daycount > 31)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 31;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 11)
    {
        if(daycount > 30)
        {
            month_sum_int = month_sum_int + 1;
            day_sum_int = daycount - 30;
        } else
        {
            day_sum_int = daycount;
        }
    } else if(month_sum_int == 12)
    {
        if(daycount > 31)
        {
            year_sum_int = year_sum_int + 1;
            month_sum_int = 1;  //notice this is not +1, resetting for the year
            day_sum_int = daycount - 31;
        } else
        {
            day_sum_int = daycount;
        }
    } else
    {
        std::cout << "Error in subtractTime function in adjustTimeStamps class! "
                  << "Month is somehow out of range of 1 to 12!\n";
        std::exit(EXIT_FAILURE);
    }

    calculatedTime = dblToString(year_sum_int) + "\\:" + dblToString(month_sum_int) + "\\:"
            + dblToString(day_sum_int) + " " + dblToString(hours_sum_int) + "\\:"
            + dblToString(mins_sum_int) + "\\:" + dblToString(secs_sum_int);
    std::cout << "calculatedTime = " << calculatedTime << "\n";

    std::cout << "finished adding time\n";
    return calculatedTime;
}

bool adjustTimeStamps::is_numeric(char const *string)
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

int adjustTimeStamps::stringToInt(std::string value)
{
    int Int = boost::lexical_cast<int>(value);
    return Int;
}

std::string adjustTimeStamps::dblToString(double value)
{
    std::string str;
    if(value < 10)
    {
        str = "0";
    }
    str = str + boost::lexical_cast<std::string>(value);

    return str;
}
