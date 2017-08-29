#ifndef READVIDEOTIMESFILE_H
#define READVIDEOTIMESFILE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <istream>

class readVideoTimesFile
{

public:

    /*
     * This class is for reading in the videoTimes file, making sure all the inputs are of the right format.
     */

    readVideoTimesFile();

    void readFile(std::string videoTimesFile);
    std::vector<std::string> getVideoTimes();

private:

    std::vector<std::string> videoTimes;
    bool checkVideoTime(std::string videoTime);
    bool is_numeric(char const *string);

};

#endif // READVIDEOTIMESFILE_H
