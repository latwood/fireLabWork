#ifndef ADJUSTTIMESTAMPS_H
#define ADJUSTTIMESTAMPS_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <istream>

class adjustTimeStamps
{

public:

    /*
     * This class is for adjusting the timestamps according to a given hour for adjusting
     *
     * The idea is that the timestamps are in one timezone, which is not correct,
     * and the proper time zone is x hours less than the current timestamp timezone.
     *
     * This needs to do stuff similar to generateVideoTimes to for error correction,
     * and maybe for adjusting the times. Need to adjust the hour, but if the hour changes,
     * adjust the date. Need to watch out for month changes and year changes in the date.
     *
     */

    adjustTimeStamps();

    std::vector<std::string> adjustTime(std::string timeCorrector,std::vector<std::string> timeStamps);

private:

    std::string timeCorrector_year_string;
    std::string timeCorrector_month_string;
    std::string timeCorrector_day_string;
    std::string timeCorrector_hour_string;
    std::string timeCorrector_min_string;
    std::string timeCorrector_sec_string;
    int timeCorrector_year_int;
    int timeCorrector_month_int;
    int timeCorrector_day_int;
    int timeCorrector_hour_int;
    int timeCorrector_min_int;
    int timeCorrector_sec_int;

    std::vector<std::string> correctedTimeStamps;

    void checkInputs(std::string timeCorrector,std::vector<std::string> timeStamps);
    std::string subtractTime(std::string timeCorrector,std::string originalTime);
    std::string addTime(std::string timeCorrector,std::string originalTime);
    bool is_numeric(char const *string);
    int stringToInt(std::string value);
    std::string dblToString(double value);

};

#endif // ADJUSTTIMESTAMPS_H
