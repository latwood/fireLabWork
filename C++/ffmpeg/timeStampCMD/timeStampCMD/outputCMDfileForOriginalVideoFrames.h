#ifndef OUTPUTCMDFILEFORORIGINALVIDEOFRAMES_H
#define OUTPUTCMDFILEFORORIGINALVIDEOFRAMES_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <istream>

class outputCMDfileForOriginalVideoFrames
{

public:

    /*
     * This class is for outputting the cmd file that uses only part of the timestamps
     * and the log file videoTimes.
     *
     * The tricky part will be to make sure that the timestamps go with the correct video times,
     * which will be determined by the frames from the log file and the position in the timestamps
     *
     * Hopefully this class unclutters the main function and makes stuff easier to read.
     *
     * Should put the error handling to make sure the stuff coming in is the right size,
     * basically a last minute check to make sure the filler functions worked correctly.
     *
     */

    outputCMDfileForOriginalVideoFrames();

    void outputCMDfile(std::string filePath,std::vector<double> videoFrames,
                       std::vector<std::string> videoTimes,std::vector<std::string> timeStamps);

private:

    void checkInputs(std::vector<double> videoFrames,std::vector<std::string> videoTimes);

};

#endif // OUTPUTCMDFILEFORORIGINALVIDEOFRAMES_H
