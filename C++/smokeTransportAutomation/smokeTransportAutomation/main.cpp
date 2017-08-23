#include <iostream>
#include <string>

#include <smokeTransportRun.h>
#include <smokeTransportOptions.h>
#include <readConfigFile.h>

using namespace std;

int main()
{
    //initialize the classes before the loop
    smokeTransportOptions smokeOptions;
    readConfigFile configFile(smokeOptions.get_theOptions());

    const int numberOfConfigFiles = 1;

    std::string configFilePaths[numberOfConfigFiles] =
    {
        "/home/latwood/Documents/qtCreatorProjects/reworks/exampleSmokeTransportConfig.cfg"
    };

    for(int i = 0; i < numberOfConfigFiles; i++)
    {
        configFile.newConfigFile(configFilePaths[i]);
        smokeTransportRun smokeRun(configFile);     //will be something like configFile.getData()
    }

    return 0;
}