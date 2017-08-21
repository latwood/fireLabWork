#include <iostream>
#include <vector>
#include <string>

#include <WindNinjaRun.h>

using namespace std;

int main()
{
    //set up config options separate for windninja and for windninja automation
    //have paths to windninja automation config files.
    //windninja automation config files will then use the inputs to write windninja config files and run windninja
    //the idea is to only have a single input for a given scenario
    //then the read config file needs to be changed so it needs some kind of options as the input, or a setoptions function
    //so I guess the readconfig file will be a datatype that is windninja or windninja automation or smoke transport
    //but will use those as the options. So is there a way to allow a function in a class to allow a separate class
    //of slightly changing type as an input? Or need to just create a class called options that the other options inherit from?

    const int numberOfWindSims = 2;

    std::cout << "creating windsims\n";

    WindNinjaRun windRuns[numberOfWindSims] =
    {
        WindNinjaRun("5","210"),
        WindNinjaRun("5","30")
    };

    return 0;
}
