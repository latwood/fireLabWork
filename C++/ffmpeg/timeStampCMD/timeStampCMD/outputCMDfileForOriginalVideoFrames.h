#ifndef OUTPUTCMDFILEFORORIGINALVIDEOFRAMES_H
#define OUTPUTCMDFILEFORORIGINALVIDEOFRAMES_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <istream>

class outputCMDfileForOriginalVideoFrames
{

public:

    /*
     * This class is for outputting the cmd file that uses only part of the timestamps
     * and the log file videoTimes.
     *
     * The tricky part will be to make sure that the timestamps go with the correct video times,
     * which will be determined by the frames from the log file and the position in the timestamps
     *
     * Hopefully this class unclutters the main function and makes stuff easier to read.
     *
     * Should put the error handling to make sure the stuff coming in is the right size,
     * basically a last minute check to make sure the filler functions worked correctly.
     *
     */

    outputCMDfileForOriginalVideoFrames();

    void outputCMDfile(std::string filePath,std::vector<double> videoFrames,
                       std::vector<std::string> videoTimes,std::vector<std::string> timeStamps);

    /*
     * okay currently, it works, but opening it in vlc on windows makes it strange to watch
     * speeds up and slows down. The important thing is that you can pause and see the first
     * and last frames. It looks like using this class's output results in it beginning
     * and ending with the right timestamps. It almost looks better because you can see it change
     * in a nice manner. Wasn't expecting this to be the better video lol.
     *
     * Hm, just had an interesting experience where the ffmpeg log output the final frame 2 times.
     * Looks like the process still works if a repeated frame happens at the end, but if it happens
     * in the middle, I bet that would cause a lot of fun.
     * man so many different error trouble shoots. It's getting difficult to manage it all
     * Seems like the frame numbers change if the computer has differing loads since ffmpeg uses all
     * processors. So this process isn't the best, and the for all timestamps is probably safer
     */

private:

    void checkInputs(std::vector<double> videoFrames,std::vector<std::string> videoTimes);

};

#endif // OUTPUTCMDFILEFORORIGINALVIDEOFRAMES_H
