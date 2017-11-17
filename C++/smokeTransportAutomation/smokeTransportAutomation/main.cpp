#include <iostream>
#include <string>

#include <smokeTransportRun.h>
#include <smokeTransportOptions.h>
#include <readConfigFile.h>

using namespace std;

int main()
{
    // if you want debug messages, uncomment the #define YESDEBUG found in usefulFunction.h

    //initialize the classes before the loop
    smokeTransportOptions smokeOptions;
    readConfigFile configFile(smokeOptions.get_theOptions());

    const int numberOfConfigFiles = 1;

    std::string configFilePaths[numberOfConfigFiles] =
    {
        "/home/latwood/Downloads/exampleSmokeTransportConfig.cfg"
    };

    for(int i = 0; i < numberOfConfigFiles; i++)
    {
        configFile.newConfigFile(configFilePaths[i]);
        smokeTransportRun smokeRun(configFile);     //will be something like configFile.getData()
    }


    return 0;
}
