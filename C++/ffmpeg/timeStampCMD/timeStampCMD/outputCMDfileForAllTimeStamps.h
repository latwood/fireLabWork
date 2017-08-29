#ifndef OUTPUTCMDFILEFORALLTIMESTAMPS_H
#define OUTPUTCMDFILEFORALLTIMESTAMPS_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <istream>

class outputCMDfileForAllTimeStamps
{

public:

    /*
     * This class is for outputting the cmd file that uses all the timestamps
     * with the generatedVideoTimes. Shouldn't need much, probably not even too much error handling,
     * since the timestamps and generatedVideoTimes should be the same size.
     *
     * Hopefully this class unclutters the main function and makes stuff easier to read.
     *
     * Should put the error handling to make sure the stuff coming in is the right size,
     * basically a last minute check to make sure the filler functions worked correctly.
     *
     */

    outputCMDfileForAllTimeStamps();

    void outputCMDfile(std::string filePath,std::vector<std::string> videoTimes,
                  std::vector<std::string> timeStamps);

private:

    void checkInputs(std::vector<std::string> videoTimes,std::vector<std::string> timeStamps);

};

#endif // OUTPUTCMDFILEFORALLTIMESTAMPS_H
