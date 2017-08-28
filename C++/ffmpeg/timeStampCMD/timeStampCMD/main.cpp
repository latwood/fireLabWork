
#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>

#include "readTimeStampsFile.h"
#include "readFramesFile.h"
#include "readVideoTimesFile.h"
#include "generateVideoTimes.h"
#include "outputCMDfileForAllTimeStamps.h"
#include "outputCMDfileForOriginalVideoFrames.h"

using namespace std;

int main()
{
    /*
     * The goal of this script is to be able to take a file containing a list of the picture timestamps
     * and a properly formatted log file of an ffmpeg run that is a list of the progress output messages
     * and create a cmd file for running ffmpeg with the sendcmd functionality, creating timestamps.
     *
     * Because the list of progress output messages is smaller than the list of the picture timestamps,
     * the program needs to create the cmd file using all the times in the ffmpeg log output, taking
     * the timestamps that only occur at those times; or the program needs to generate a list of times
     * that are as big as the list of timestamps and use that for the cmd file.
     *
     * Variables:
     * I'm thinking I need a separate string for each filename, separate vectors of the ffmpeg log
     * frames and times, a vector of generated times, and a vector containing the timestamps
     *
     * Could probably create a class for any times to be generated,
     * which has a function for time addition
     *
     * Hm, should probably just generate both cmd files, one for each method
     *
     */

    std::string mainDirectory = "/home/latwood/Documents/missoulaFireVideos/loloPeakPlume/allTogetherWithoutTanner/";
    std::string timeStampsFile = "timestamps.txt";
    std::string framesFile = "frames.txt";
    std::string videoTimesFile = "videoTimes.txt";

    std::vector<std::string> timeStamps;
    std::vector<std::string> frames_str;
    std::vector<double> frames_dbl;
    std::vector<std::string> videoTimes;
    std::vector<std::string> generatedVideoTimes;

    readTimeStampsFile toReadTimeStampsFile;
    readFramesFile toReadFramesFile;
    readVideoTimesFile toReadVideoTimesFile;
    generateVideoTimes toGenerateVideoTimes;
    outputCMDfileForAllTimeStamps toOutputCMDfileForAllTimeStamps;
    outputCMDfileForOriginalVideoFrames toOutputCMDfileForOriginalVideoFrames;

    //read in the files with error handling to make sure it's the right format
    toReadTimeStampsFile.readFile(mainDirectory+timeStampsFile);
    timeStamps = toReadTimeStampsFile.getTimeStamps();
    toReadFramesFile.readFile(mainDirectory+framesFile);
    frames_str = toReadFramesFile.getFrames();
    toReadVideoTimesFile.readFile(mainDirectory+videoTimesFile);
    videoTimes = toReadVideoTimesFile.getVideoTimes();

    //create frames in form of doubles
    for(double i = 0; i < frames_str.size(); i ++)
    {
        double dbl = boost::lexical_cast<double>(frames_str[i]);
        frames_dbl.push_back(dbl);
    }

    //generate video times
    toGenerateVideoTimes.fillVideoTimes(frames_dbl[1],frames_dbl[0],videoTimes[1],videoTimes[0],frames_dbl[frames_dbl.size()-1]);
    generatedVideoTimes = toGenerateVideoTimes.getVideoTimes();

    //output cmd files
    std::string outputCMDfile_allTimeStamps = "timestamps-all.cmd";
    std::string outputCMDfile_toVideoFrames = "timestamps-toVideoFrames.cmd";



    cout << "Finished generating timestamp cmd file!" << endl;
    return 0;
}
