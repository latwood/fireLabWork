#include "smokeTransportRun.h"

smokeTransportRun::smokeTransportRun(readConfigFile configFile)
{
    //need to change all these to: getOption("configFileName") . . .
    //thought about passing the config file on to openFoamSystemFiles, but in the end, each variable that
    //needs to change majorly is changed here in loop statements
    //this means that all the other stuff doesn't much need to change. In fact, it has a record of what has been chosen,
    //so any major change would be internal to the program, at least for now
    //all the other variables just don't change so frequently

    //Heck this could eventually be a way to run any OpenFoam solver . . . a lot of work,
    //would be better to copy and paste the config file stuff and adapt the smoke transport
    //or openfoam system files for the new application first, then with enough of them, combine.

    configFileName = "smokeTransportConfig.cfg";

    /*
    configFilePath = configFile.getConfigFilePath();
    parentDirectory = configFile.getParentDirectory();
    startCase = configFile.getStartCase();
    initialTime = configFile.getInitialTime();
    numberOfSteps = configFile.getNumberOfSteps();
    endTimes = configFile.getEndTimes();
    writeIntervals = configFile.getWriteIntervals();
    sourceMinCoordinates = configFile.getSourceMinCoordinates();
    sourceMaxCoordinates = configFile.getSourceMaxCoordinates();
    sourceValues = configFile.getSourceValues();
    UfileLocations = configFile.getUfileLocations();
    missingTimes = configFile.getMissingTimes();
    missingTimeDirectories = configFile.getMissingTimeDirectories();
    */

    configFilePath = configFile.get_optionValues_singleString("configFilePath");
    parentDirectory = configFile.get_optionValues_singleString("parentDirectory");
    startCase = configFile.get_optionValues_singleString("startCase");
    initialTime = configFile.get_optionValues_singleString("initialTime");
    numberOfSteps = configFile.get_optionValues_singleInt("numberOfSteps");
    endTimes = configFile.get_optionValues_multiString("endTimes");
    writeIntervals = configFile.get_optionValues_multiString("writeIntervals");
    sourceMinCoordinates = configFile.get_optionValues_singleString("sourceMinCoordinates");
    for(size_t i = 0; i < sourceMinCoordinates.length(); i++)
    {
        if(sourceMinCoordinates.substr(i,1) == ":")
        {
            sourceMinCoordinates = sourceMinCoordinates.substr(0,i) + " " + sourceMinCoordinates.substr(i+1,sourceMinCoordinates.size());
        }
    }
    std::cout << "sourceMinCoordinates = " << sourceMinCoordinates << endl;
    sourceMaxCoordinates = configFile.get_optionValues_singleString("sourceMaxCoordinates");
    for(size_t i = 0; i < sourceMaxCoordinates.length(); i++)
    {
        if(sourceMaxCoordinates.substr(i,1) == ":")
        {
            sourceMaxCoordinates = sourceMaxCoordinates.substr(0,i) + " " + sourceMaxCoordinates.substr(i+1,sourceMaxCoordinates.size());
        }
    }
    std::cout << "sourceMaxCoordinates = " << sourceMaxCoordinates << endl;
    sourceValues = configFile.get_optionValues_multiString("sourceValues");
    UfileLocations = configFile.get_optionValues_multiString("UfileLocations");
    missingTimes = configFile.get_optionValues_singleInt("missingTimes");
    missingTimeDirectories = configFile.get_optionValues_multiString("missingTimeDirectories");

    transportCase = parentDirectory + "combined/";

    createTransportCase();  //warning, this also deletes the existing case if there is one, since it will be generated from scratch
    copySmokeTransportConfigFile();
    updateTimeDirectories();

    for(int i = 0; i < numberOfSteps; i++)
    {
        updateSystemFiles(i);
        if(i == 0)
        {
            runSetFields();
        } else
        {
            if(UfileLocations[i] != UfileLocations[i-1])
            {
                updateUfile(i);
            }
            if(sourceValues[i] != sourceValues[i-1])
            {
                runSetFields();
            }
        }
        runSmokeTransport();
    }
}

void smokeTransportRun::createTransportCase()
{
    smokeMessage("creating transport case\n");
    std::string removeOldCaseCommand = "rm -rf " + transportCase;
    system(removeOldCaseCommand.c_str());
    std::string createTransportCaseCommand = "cp -rT " + parentDirectory + startCase +
            " " + transportCase;
    system(createTransportCaseCommand.c_str());
}

void smokeTransportRun::copySmokeTransportConfigFile()
{
    smokeMessage("transferring transport config file\n");
    std::string copySmokeTransportConfigFileCommand = "cp " + configFilePath + " " + transportCase + configFileName;
    system(copySmokeTransportConfigFileCommand.c_str());
}

void smokeTransportRun::updateTimeDirectories()
{
    smokeMessage("adding missing time directories\n");
    std::string createTimeDirectoryCommand;
    for(int i = 0; i < missingTimes; i++)
    {
        createTimeDirectoryCommand = "cp -rT " + transportCase + "0" +
                " " + transportCase + missingTimeDirectories[i];
        system(createTimeDirectoryCommand.c_str());
    }
}

void smokeTransportRun::updateSystemFiles(int i)
{
    smokeMessage("updating system files\n");
    if(i == 0)
    {
        OpenFoamSystemFiles updateSystem(true,transportCase,initialTime,endTimes[i],writeIntervals[i],
                                         sourceMinCoordinates,sourceMaxCoordinates,sourceValues[i]);
    } else
    {
        OpenFoamSystemFiles updateSystem(false,transportCase,endTimes[i-1],endTimes[i],writeIntervals[i],
                sourceMinCoordinates,sourceMaxCoordinates,sourceValues[i]);
    }
}

void smokeTransportRun::updateUfile(int i)
{
    smokeMessage("updating velocity files\n");
    //it is i-1 because this is endTImes not startTimes. The UfileLocations is still i since that is the current case
    std::string copyUfileCommand = "cp -rT " + parentDirectory + UfileLocations[i] + "U" +
            " " + transportCase+endTimes[i-1]+"/U";
    system(copyUfileCommand.c_str());
    std::string deletePhiFileCommand = "rm -f " + transportCase + endTimes[i-1] + "/phi";  //doesn't error if it isn't already there
    system(deletePhiFileCommand.c_str());
}

void smokeTransportRun::runSetFields()
{
    smokeMessage("running setFields command\n");
    std::string setFieldsCommand = "setFields -case " + transportCase;
    system(setFieldsCommand.c_str());
}

void smokeTransportRun::runSmokeTransport()
{
    smokeMessage("running myScalarTransportFoam command\n");
    std::string runSmokeTransportCommand = "myScalarTransportFoam -case " + transportCase;
    system(runSmokeTransportCommand.c_str());
}

void smokeTransportRun::smokeMessage(std::string theMessage)
{
    std::cout << theMessage;
    system("sleep 0.5");
}
