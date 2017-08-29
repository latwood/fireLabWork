#ifndef GENERATEVIDEOTIMES_H
#define GENERATEVIDEOTIMES_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <istream>

class generateVideoTimes
{

public:

    /*
     * This class is for generating a new list of video times that matches the number of timestamps.
     * The idea is that it uses the ffmpeg log file's frames and video times to find the proper video times
     * for each frame not shown in the log.
     *
     * So this class needs a vector, with a function that calls it to be filled
     * that uses the original video frames and times.
     * Also need a function to call the values of the vector and a function
     * for doing the necessary calculations with time.
     *
     */

    generateVideoTimes();
    void fillVideoTimes(double secondFrame, double firstFrame, std::string secondTime, std::string firstTime, double lastFrame);
    std::vector<std::string> getVideoTimes();

private:

    std::string timeInterval;
    std::vector<std::string> generatedVideoTimes;

    void checkFillVideoTimesInput(double secondFrame, double firstFrame, std::string secondTime, std::string firstTime, double lastFrame);
    std::string subtractTime(std::string timeBase, std::string timeToSubtract);
    std::string divideTimeByDouble(std::string timevalue, double doublevalue);
    std::string addTime(std::string timeBase, std::string timeToAdd);
    bool is_numeric(char const *string);
    int stringToInt(std::string value);
    std::string dblToString(double value);

};

#endif // GENERATEVIDEOTIMES_H
