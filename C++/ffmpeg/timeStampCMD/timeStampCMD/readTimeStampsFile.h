#ifndef READTIMESTAMPSFILE_H
#define READTIMESTAMPSFILE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <istream>

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
    bool is_numeric(char const *string);

};

#endif // READTIMESTAMPSFILE_H
