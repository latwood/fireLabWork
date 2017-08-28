#ifndef READVIDEOTIMESFILE_H
#define READVIDEOTIMESFILE_H

#include <vector>
//#include <istream>
//#include <fstream>

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

};

#endif // READVIDEOTIMESFILE_H
