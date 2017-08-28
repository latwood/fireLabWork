#include "readTimeStampsFile.h"

readTimeStampsFile::readTimeStampsFile()
{
    /*
     * don't need to do anything here. The user needs to call
     * the other functions to do stuff with this class.
     * So this script should not do more than one set of files at a time
     */
}

void readTimeStampsFile::readFile(std::string timeStampsFile)
{
    std::ifstream is_file(timeStampsFile);

    std::string currentTimeStamp;

    while(std::getline(is_file,currentTimeStamp))
    {
        if(checkTimeStamp(currentTimeStamp) == 0)
        {
            timeStamps.push_back(currentTimeStamp);
        }
    }
}

bool readTimeStampsFile::checkTimeStamp(std::string timeStamp)
{
    bool failed = false;
    if(timeStamp.length() != 23)
    {
        std::cout << "Error in readFile function in readTimeStampsFile class! timeStamp length != 23\n";
        failed = true;
    }

    std::string year = timeStamp.substr(0,4);
    std::string firstBreak = timeStamp.substr(4,2);
    std::string month = timeStamp.substr(6,2);
    std::string secondBreak = timeStamp.substr(8,2);
    std::string day = timeStamp.substr(10,2);
    std::string thirdBreak = timeStamp.substr(12,1);
    std::string hours = timeStamp.substr(13,2);
    std::string fourthBreak = timeStamp.substr(16,2);
    std::string mins = timeStamp.substr(17,2);
    std::string fifthBreak = timeStamp.substr(19,2);
    std::string secs = timeStamp.substr(21,2);
    if(isdigit(year) != 0 || firstBreak != "\:" || isdigit(month) != 0 || secondBreak != "\:"
            || isdigit(day) != 0 || thirdBreak != " " || isdigit(hours) != 0 || fourthBreak != "\:"
            || isdigit(mins) != 0 || fifthBreak != "\:" || isdigit(secs) != 0)
    {
        std::cout << "Error in readFile function in readTimeStampsFile class! timeStamp is wrong format!\n";
        failed = true;
    }

    return failed;
}

std::vector<std::string> readTimeStampsFile::getTimeStamps()
{
    if(timeStamps.size() == 0)
    {
        std::cout << "Error in function getTimeStamps in readTimeStampsFile class! " <<
                     "timeStamps in this class is empty still! Call readFile first!\n";
        std::exit(EXIT_FAILURE);
    }
    return timeStamps;
}
