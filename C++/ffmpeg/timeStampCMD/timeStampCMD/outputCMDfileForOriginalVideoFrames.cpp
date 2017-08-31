#include "outputCMDfileForOriginalVideoFrames.h"

outputCMDfileForOriginalVideoFrames::outputCMDfileForOriginalVideoFrames()
{
    /*
     * don't need to do anything here. The user needs to call
     * the other functions to do stuff with this class.
     * So this script should not do more than one set of files at a time
     */
}

void outputCMDfileForOriginalVideoFrames::outputCMDfile(std::string filePath,std::vector<double> videoFrames,
                   std::vector<std::string> videoTimes,std::vector<std::string> timeStamps)
{
    std::cout << "outputting for original video frames cmd file\n";
    checkInputs(videoFrames,videoTimes);
    std::cout << "frames size = " << videoFrames.size() << "\n";
    std::cout << "videoTimes size = " << videoTimes.size() << "\n";
    std::cout << "timeStamps size = " << timeStamps.size() << "\n";

    //now write the file
    FILE *fzout = fopen(filePath.c_str(),"w");
    //write first line without loop
    double framesCounter = 0;
    for (double timeStampsCounter = 0; timeStampsCounter < timeStamps.size(); timeStampsCounter++)
    {
        std::cout << "timeStampsCounter+1 = " << timeStampsCounter+1 << "\n";
        std::cout << "framesCounter = " << framesCounter << "\n";
        std::cout << "videoFrames[framesCounter] = " << videoFrames[framesCounter] << "\n";
        if(timeStampsCounter+1 == videoFrames[framesCounter])
        {
            if(framesCounter == 0)
            {
                fprintf(fzout,"00:00:00.00-%s [enter] drawtext reinit 'text=%s';\n",videoTimes[framesCounter].c_str(),timeStamps[timeStampsCounter].c_str());
            } else
            {
                fprintf(fzout,"%s-%s [enter] drawtext reinit 'text=%s';\n",videoTimes[framesCounter-1].c_str(),videoTimes[framesCounter].c_str(),timeStamps[timeStampsCounter].c_str());
            }
            framesCounter++;
        }
    }
    fclose(fzout);

    std::cout << "finished outputting for original video frames cmd file\n";
}

void outputCMDfileForOriginalVideoFrames::checkInputs(std::vector<double> videoFrames,std::vector<std::string> videoTimes)
{
    if(videoFrames.size() != videoTimes.size())
    {
        std::cout << "Error! videoFrames and videoTimes are not of the same size!\n";
        std::exit(EXIT_FAILURE);
    }
}
