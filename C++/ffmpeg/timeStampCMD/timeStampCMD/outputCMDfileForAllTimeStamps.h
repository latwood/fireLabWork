#ifndef OUTPUTCMDFILEFORALLTIMESTAMPS_H
#define OUTPUTCMDFILEFORALLTIMESTAMPS_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <istream>

class outputCMDfileForAllTimeStamps
{

public:

    /*
     * This class is for outputting the cmd file that uses all the timestamps
     * with the generatedVideoTimes. Shouldn't need much, probably not even too much error handling,
     * since the timestamps and generatedVideoTimes should be the same size.
     *
     * Hopefully this class unclutters the main function and makes stuff easier to read.
     *
     * Should put the error handling to make sure the stuff coming in is the right size,
     * basically a last minute check to make sure the filler functions worked correctly.
     *
     */

    outputCMDfileForAllTimeStamps();

    void outputCMDfile(std::string filePath,std::vector<std::string> videoTimes,
                  std::vector<std::string> timeStamps);

    /*
     * okay currently, it works, but opening it in vlc on windows makes it strange to watch
     * speeds up and slows down. The important thing is that you can pause and see the first
     * and last frames. It looks like using this class's output results in it ending with
     * a slightly off timestamp. It looks like this is because it doesn't reinit on the
     * last frame or something.
     *
     * Probably just needs to start the whole thing at the zero to the second time. The idea is that when
     * it pops up a certain frame, the reinit happens on that frame.
     * So the reinit isn't happening on the last frame because it is supposed to happen on three frames before?
     * so we want to end with time end to later, this thing. So not from time 0 to time 1 use time 1,
     * but from time 0 to time 2 use time 1, from time 2 to time 3 use time 2, from time 3 to time 4 use time 3
     * . . . from time end to later use time end
     *
     * Hm, tried this out, and while it got another time at the end, it didn't end on the right timestamp.
     * So I'm thinking that the original method of 0 to 1 use time 1, 1 to 2 use time 2 and so on is the
     * best way to deal with this problem. It is probably the more accurate time. Would have to ask
     * for preferences from others that know better.
     */

private:

    void checkInputs(std::vector<std::string> videoTimes,std::vector<std::string> timeStamps);

};

#endif // OUTPUTCMDFILEFORALLTIMESTAMPS_H
