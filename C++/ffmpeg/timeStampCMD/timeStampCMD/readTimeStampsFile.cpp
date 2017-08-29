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
    std::cout << "reading timeStampsFile\n";
    std::ifstream is_file(timeStampsFile.c_str());

    std::string currentTimeStamp;

    while(std::getline(is_file,currentTimeStamp))
    {
        if(checkTimeStamp(currentTimeStamp) == 0)
        {
            timeStamps.push_back(currentTimeStamp);
        }
    }
    std::cout << "finished reading timeStampsFile\n";
}

bool readTimeStampsFile::checkTimeStamp(std::string timeStamp)
{
    bool failed = false;
    if(timeStamp.length() != 23)
    {
        std::cout << "Error in readFile function in readTimeStampsFile class! timeStamp length != 23\n";
        failed = true;
    }

    for(double i = 0; i < timeStamp.length(); i++)
    {
        std::string chr = timeStamp.substr(i,1);
        if(i == 4 || i == 8 || i == 15 || i == 19)
        {
            if(chr != "\\")
            {
                std::cout << "Error in readFile function in readTimeStampsFile class! timeStamp is wrong format!\n";
                failed = true;
            }
        } else if(i == 5 || i == 9 || i == 16 || i == 20)
        {
            if(chr != ":")
            {
                std::cout << "Error in readFile function in readTimeStampsFile class! timeStamp is wrong format!\n";
                failed = true;
            }
        } else if(i == 12)
        {
            if(chr != " ")
            {
                std::cout << "Error in readFile function in readTimeStampsFile class! timeStamp is wrong format!\n";
                failed = true;
            }
        } else
        {
            if(is_numeric(chr.c_str()) != true)
            {
                std::cout << "Error in readFile function in readTimeStampsFile class! timeStamp is wrong format!\n";
                failed = true;
            }
        }
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

bool readTimeStampsFile::is_numeric(char const *string)
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
