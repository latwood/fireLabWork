#include "readFramesFile.h"

readFramesFile::readFramesFile()
{
    /*
     * don't need to do anything here. The user needs to call
     * the other functions to do stuff with this class.
     * So this script should not do more than one set of files at a time
     */
}

void readFramesFile::readFile(std::string framesFile)
{
    std::cout << "reading frames file\n";
    std::ifstream is_file(framesFile.c_str());

    std::string currentFrame;

    while(std::getline(is_file,currentFrame))
    {
        if(checkFrame(currentFrame) == 0)
        {
            frames.push_back(currentFrame);
        }
    }
    std::cout << "finished reading frames file\n";
}

bool readFramesFile::checkFrame(std::string frame)
{
    bool failed = false;
    if(is_numeric(frame.c_str()) != true)
    {
        std::cout << "Error in readFile function in readFramesFile class! frame is not a digit!\n";
        failed = true;
    }

    return failed;
}

std::vector<std::string> readFramesFile::getFrames()
{
    if(frames.size() == 0)
    {
        std::cout << "Error in function getFrames in readFramesFile class! " <<
                     "frames in this class is empty still! Call readFile first!\n";
        std::exit(EXIT_FAILURE);
    }
    return frames;
}

bool readFramesFile::is_numeric(char const *string)
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
