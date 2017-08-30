#include "outputCMDfileForAllTimeStamps.h"

outputCMDfileForAllTimeStamps::outputCMDfileForAllTimeStamps()
{
    /*
     * don't need to do anything here. The user needs to call
     * the other functions to do stuff with this class.
     * So this script should not do more than one set of files at a time
     */
}

void outputCMDfileForAllTimeStamps::outputCMDfile(std::string filePath,std::vector<std::string> videoTimes,
              std::vector<std::string> timeStamps)
{
    std::cout << "outputting for all time stamps cmd file\n";
    checkInputs(videoTimes,timeStamps);
    std::cout << "videoTimes size = " << videoTimes.size() << "\n";
    std::cout << "timeStamps size = " << timeStamps.size() << "\n";

    //now write the file
    FILE *fzout = fopen(filePath.c_str(),"w");
    //write first line without loop
    fprintf(fzout,"00:00:00.00-%s [enter] drawtext reinit 'text=%s';\n",videoTimes[1].c_str(),timeStamps[0].c_str());
    for (double i = 1; i < (videoTimes.size()-1); i++)
    {
        fprintf(fzout,"%s-%s [enter] drawtext reinit 'text=%s';\n",videoTimes[i].c_str(),videoTimes[i+1].c_str(),timeStamps[i].c_str());
    }
    //write final line without loop
    fprintf(fzout,"%s-%s [enter] drawtext reinit 'text=%s';\n",videoTimes[videoTimes.size()-1].c_str(),videoTimes[videoTimes.size()-1].c_str(),timeStamps[timeStamps.size()-1].c_str());
    fclose(fzout);

    std::cout << "finished outputting for all time stamps cmd file\n";
}

void outputCMDfileForAllTimeStamps::checkInputs(std::vector<std::string> videoTimes,
                                                std::vector<std::string> timeStamps)
{
    if(videoTimes.size() != timeStamps.size())
    {
        std::cout << "Error! videoTimes and timeStamp size are not equal!\n";
        std::exit(EXIT_FAILURE);
    }
}
