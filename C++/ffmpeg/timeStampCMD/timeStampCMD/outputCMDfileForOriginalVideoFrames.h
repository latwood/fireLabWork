#ifndef OUTPUTCMDFILEFORORIGINALVIDEOFRAMES_H
#define OUTPUTCMDFILEFORORIGINALVIDEOFRAMES_H


class outputCMDfileForOriginalVideoFrames
{

public:

    /*
     * This class is for generating outputting the cmd file that uses only part of the timestamps
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

private:


};

#endif // OUTPUTCMDFILEFORORIGINALVIDEOFRAMES_H
