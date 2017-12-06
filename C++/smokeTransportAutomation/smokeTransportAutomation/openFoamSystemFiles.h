#ifndef OPENFOAMSYSTEMFILES_H
#define OPENFOAMSYSTEMFILES_H

#include <fstream>

#include "usefulFunctions.h"  //includes <iostream> and <sys/stat.h>

class OpenFoamSystemFiles
{
public:

    OpenFoamSystemFiles(bool firstTime,std::string outputPath,std::string startTime_value,
                        std::string endTime_value,std::string writeInterval_value,
                        std::string sourceMinCoordinates_value,std::string sourceMaxCoordinates_value,
                        std::string sourceValue_value);

    //this is the equivalent main function
    bool writeSystemFiles(bool firstTime);

    //technically not necessary, but it makes it nice for debugging since it makes it easier to replicate
    //and compare with OpenFoam tutorial files.
    void makeFoamHeader(std::string theClassType, std::string theObjectType, std::string theFoamFileLocation);

    //technically not necessary, but it makes it nice for debugging since it makes it easier to replicate
    //and compare with OpenFoam tutorial files.
    void makeFoamFooter();

    //creates the controlDict file
    void writeControlDict();

    //creates the fvSchemes file
    void writeFvSchemes();

    //creates the fvSolution file
    void writeFvSolution();

    //creates the setFieldsDict file
    void writeSetFieldsDict();

private:

    std::string foam_version;	//the foam version number, probably will get rid of this later
    FILE *fzout;		//changing file to which information will be written

//values used for system directory
    std::string controlDictPath;    //path to controlDict file
    std::string fvSchemesPath;      //path to fvSchemes file
    std::string fvSolutionPath;     //path to fvSolution file
    std::string setFieldsDictPath;  //path to setFieldsDict file

    //controlDict variables
    std::string application;        //the simulation type, the application variable in controlDict file
    std::string startFrom;          //controls the start time of the simulation. Options are: firstTime, startTime, latestTime
    std::string startTime;          //startTime for the simulation if startFrom is specified to be startTime
    std::string stopAt;             //controls the end time of the simulation. Options are: endTime, writeNow, noWriteNow, nextWrite
    std::string endTime;            //end time for the simulation when stopAt is specified to be endTime
    std::string deltaT;             //time step of the simulation
    std::string writeControl;       //controls the timing of write output to file. Options are: timeStep, runTime, adjustableRunTime, cpuTime, clockTime
    std::string writeInterval;      //frequency after startTime for when new time directories are written
    std::string purgeWrite;         //number of written out time directories to be kept for a given simulation
    std::string writeFormat;        //specifies the format of the data files. Options are: ascii or binary
    std::string writePrecision;     //number of decimal places for writing time directories
    std::string writeCompression;   //compress written out files with gzip yes or no
    std::string timeFormat;         //choice of name format for the time directories. Options are: fixed, scientific, general
    std::string timePrecision;      //number of decimal places for the time directory names
    std::string runTimeModifiable;  //allow controlDict to change, thus changing simulation, in the middle of simulation

    //fvSchemes variables
    std::string ddtSchemes_default;     //default ddtSchemes: Euler or SteadyState
    std::string gradSchemes_default;    //default gradSchemes: Gauss linear or cellMDlimited leastSquares 0.5, lots of combinations
    std::string divSchemes_default;     //default divSchemes: usually set to none or Gauss limitedLinear
    std::string divSchemes_divOfPhiAndT;    //divSchemes div(phi,T): Gauss limitedLinear 1 or bounded Gauss upwind
    std::string laplacianSchemes_default;   //default laplacianSchemes: usually set to none or Gauss linear limited
    std::string laplacianSchemes_laplacianOfDTandT; //laplacianSchemes laplacian(DT,T): usually Gauss linear corrected
    std::string interpolationSchemes_default;       //default interpolationSchemes: usually linear
    std::string SnGradSchemes_default;          //default SnGradSchemes: usually corrected
    std::string SnGradSchemes_SnGradOfT;        //SnGradSchemes SnGrad(T): limited 0.5
    std::string fluxRequired_default;       //default fluxRequired setting
    bool fluxRequired_T;         //Write T or not as needing a flux

    //fvSolution variables
    //note this is trickier than the others because there are so many extra things done for the momentum solver
    //and the structure is different so will need a lot of work to change stuff here eventually, or just simply write out every single option
    std::string solvers_T_solver;       //the solver used to solve for T found in the solvers section
    std::string solvers_T_preconditioner;   //the preconditioner used for T found in the solvers section
    std::string solvers_T_tolerance;        //the tolerance used to solve for T found in the solvers section
    std::string solvers_T_relTol;       //the relative tolerance used to solve for T found int he solvers section
    std::string SIMPLE_nNonOrthogonalCorrectors;    //how to correct for NonOrthogonality
    //there's a ton more stuff, almost would be better to copy and paste a file that is the same every time
    //for now, just won't write any of it and hopefully it doesn't change the simulation at all

    //not entirely sure how to do this for other ways to specify values. Right now I'm just writing this
    //for the current scalar transport case. Will probably want some kind of pointer function so it is easy
    //to know what to pass in
    //setFieldsDict variables
    std::string defaultSourceValue; //default source value, so value to be placed everywhere where not specified
    std::string distributionType;   //method to be used. If boxToCell, then will use a bounding box for specifying the source
    std::string sourceMinCoordinates;  //set of xmin, ymin, and zmin coordinates for the bounding box if using boxToCell distribution method
    std::string sourceMaxCoordinates;  //set of xmax, ymax, and zmax coordinates for the bounding box if using boxToCell distribution method
    std::string distributionSourceValue;    //value of source in the location to be specified

    usefulFunctions handy;
};

#endif // OPENFOAMSYSTEMFILES_H
