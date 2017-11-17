#ifndef SMOKETRANSPORTRUN_H
#define SMOKETRANSPORTRUN_H

#include <vector>
#include <string>
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include <openFoamSystemFiles.h>

#include <readConfigFile.h>

#include "usefulFunctions.h"  //includes <iostream> and <sys/stat.h>
#include "useCmd.h"

class smokeTransportRun
{
public:

    smokeTransportRun(readConfigFile configFile);
    void createTransportCase();
    void copySmokeTransportConfigFile();
    void updateTimeDirectories();
    void updateSystemFiles(int i);
    void updateUfile(int i);
    void runSetFields();
    void runSmokeTransport();

private:

    std::string configFileName;
    std::string configFilePath;

    std::string parentDirectory;
    std::string startCase;

    std::string transportCase;

    int numberOfSteps;
    std::string initialTime;
    std::vector<std::string> endTimes;
    std::vector<std::string> writeIntervals;
    std::string sourceMinCoordinates;
    std::string sourceMaxCoordinates;
    std::vector<std::string> sourceValues;
    std::vector<std::string> UfileLocations;
    int missingTimes;
    std::vector<std::string> missingTimeDirectories;

};

#endif // SMOKETRANSPORTRUN_H
