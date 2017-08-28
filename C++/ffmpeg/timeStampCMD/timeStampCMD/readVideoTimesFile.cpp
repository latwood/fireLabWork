#include "readVideoTimesFile.h"

readVideoTimesFile::readVideoTimesFile()
{
    /*
     * don't need to do anything here. The user needs to call
     * the other functions to do stuff with this class.
     * So this script should not do more than one set of files at a time
     */
}

void readVideoTimesFile::readFile(std::string videoTimesFile)
{
    std::ifstream is_file(videoTimesFile);

    std::string currentVideoTime;

    while(std::getline(is_file,currentVideoTime))
    {
        if(checkVideoTime(currentVideoTime) == 0)
        {
            videoTimes.push_back(currentVideoTime);
        }
    }
}

bool readVideoTimesFile::checkVideoTime(std::string videoTime)
{
    bool failed = false;
    if(videoTime.length() != 11)
    {
        std::cout << "Error in readFile function in readVideoTimesFile class! videoTime length != 11\n";
        failed = true;
    }

    std::string hours = videoTime.substr(0,2);
    std::string mins = videoTime.substr(3,2);
    std::string secs = videoTime.substr(6,2);
    std::string microsecs = videoTime.substr(9,2);
    if(videoTime.at(3) != ":" || videoTime.at(6) != ":" || videoTime.at(9) != "."
            || isdigit(hours) != 0 || isdigit(mins) != 0
            || isdigit(secs) != 0 || isdigit(microsecs) != 0)
    {
        std::cout << "Error in readFile function in readVideoTimes class! videoTime is wrong format!\n";
        failed = true;
    }

    return failed;
}

std::vector<std::string> readVideoTimesFile::getVideoTimes()
{
    if(videoTimes.size() == 0)
    {
        std::cout << "Error in function getVideoTimes in readVideoTimesFile class! " <<
                     "videoTimes in this class is empty still! Call readFile first!\n";
        std::exit(EXIT_FAILURE);
    }
    return videoTimes;
}
