#ifndef READTIMESTAMPSFILE_H
#define READTIMESTAMPSFILE_H

#include <vector>
//#include <istream>
//#include <fstream>

class readTimeStampsFile
{

public:

    /*
     * This class is for reading in the frames file, making sure all the inputs are of the right format.
     */

    readTimeStampsFile();

    void readFile(std::string timeStampsFile);
    std::vector<std::string> getTimeStamps();

private:

    std::vector<std::string> timeStamps;
    bool checkTimeStamp(std::string timeStamp);

};

#endif // READTIMESTAMPSFILE_H
