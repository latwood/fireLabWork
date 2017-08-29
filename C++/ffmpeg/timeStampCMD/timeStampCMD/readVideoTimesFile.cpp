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
    std::cout << "reading videoTimesFile\n";
    std::ifstream is_file(videoTimesFile.c_str());

    std::string currentVideoTime;

    while(std::getline(is_file,currentVideoTime))
    {
        if(checkVideoTime(currentVideoTime) == 0)
        {
            videoTimes.push_back(currentVideoTime);
        }
    }
    std::cout << "finished reading videoTimesFile\n";
}

bool readVideoTimesFile::checkVideoTime(std::string videoTime)
{
    bool failed = false;
    if(videoTime.length() != 11)
    {
        std::cout << "Error in readFile function in readVideoTimesFile class! videoTime length != 11\n";
        failed = true;
    }

    for(double i = 0; i < videoTime.length(); i++)
    {
        std::string chr = videoTime.substr(i,1);
        if(i == 2 || i == 5)
        {
            if(chr != ":")
            {
                std::cout << "Error in readFile function in readVideoTimes class! videoTime is wrong format!\n";
                failed = true;
            }
        } else if(i == 8)
        {
            if(chr != ".")
            {
                std::cout << "Error in readFile function in readVideoTimes class! videoTime is wrong format!\n";
                failed = true;
            }
        } else
        {
            if(is_numeric(chr.c_str()) != true)
            {
                std::cout << "Error in readFile function in readVideoTimes class! videoTime is wrong format!\n";
                failed = true;
            }
        }
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

bool readVideoTimesFile::is_numeric(char const *string)
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
