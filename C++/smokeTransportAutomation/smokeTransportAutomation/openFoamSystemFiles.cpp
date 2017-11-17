#include "openFoamSystemFiles.h"


OpenFoamSystemFiles::OpenFoamSystemFiles(bool firstTime,std::string outputPath,std::string startTime_value,
                                         std::string endTime_value,std::string writeInterval_value,
                                         std::string sourceMinCoordinates_value,std::string sourceMaxCoordinates_value,
                                         std::string sourceValue_value)
{

    debugMessage("\nSetting up OpenFOAM file system options");
    foam_version = "2.0";
    fzout = NULL;

//values used for system directory
    controlDictPath = outputPath + "system/controlDict";
    fvSchemesPath = outputPath + "system/fvSchemes";
    fvSolutionPath = outputPath + "system/fvSolution";
    setFieldsDictPath = outputPath + "system/setFieldsDict";


  //I thought about adding some of these to the config file, but they honestly look nice as they are.
  //In the end, the control dict file is the record showing what was actually used.
  //So any major changes should happen in here, since changes aren't likely
    //controlDict variables
    application = "myScalarTransportFoam";
    startFrom = "startTime";
    startTime = startTime_value;    // I feel like this varies a lot depending on the different situations. So I guess this is the default value
    stopAt = "endTime";
    endTime = endTime_value;    // I feel like this varies a lot depending on the different situations. So I guess this is the default value
    deltaT = "1.0";
    writeControl = "timeStep";
    writeInterval = writeInterval_value;    // I feel like this varies a lot depending on the different situations. So I guess this is the default value
    purgeWrite = "0";       // I like a value of 0 for smoke transport
    writeFormat = "ascii";
    writePrecision = "10";
    writeCompression = "uncompressed";
    timeFormat = "general";
    timePrecision = "10";
    runTimeModifiable = "true";

    //fvSchemes variables
    ddtSchemes_default = "Euler";
    gradSchemes_default = "Gauss linear";
    divSchemes_default = "none";
    divSchemes_divOfPhiAndT = "bounded Gauss upwind";
    laplacianSchemes_default = "Gauss linear limited 0.333";
    laplacianSchemes_laplacianOfDTandT = "Gauss linear corrected";
    interpolationSchemes_default = "linear";
    SnGradSchemes_default = "corrected";
    SnGradSchemes_SnGradOfT = "limited 0.5";
    fluxRequired_default = "no";
    fluxRequired_T = true;

    //fvSolution variables
    solvers_T_solver = "PBiCG";
    solvers_T_preconditioner = "DILU";
    solvers_T_tolerance = "1e-06";
    solvers_T_relTol = "0.2";
    SIMPLE_nNonOrthogonalCorrectors = "0";

    //setFieldsDict variables
    defaultSourceValue = "0";
    distributionType = "boxToCell";
    sourceMinCoordinates = sourceMinCoordinates_value;//"726019.742 5206748.293 1210";  //probably need to add this to the config file options
    sourceMaxCoordinates = sourceMaxCoordinates_value;//"726283 5207018.3 1440";  //probably need to add this to the config file options
    distributionSourceValue = sourceValue_value;

    writeSystemFiles(firstTime);
}

bool OpenFoamSystemFiles::writeSystemFiles(bool firstTime)
{
//now create the system files
    //write the controlDict file
    fzout = fopen(controlDictPath.c_str(), "w");
    makeFoamHeader("dictionary","controlDict","system");
    writeControlDict();
    makeFoamFooter();
    fclose(fzout);

    if(firstTime == true)
    {
        //write the fvSchemes file
        fzout = fopen(fvSchemesPath.c_str(),"w");
        makeFoamHeader("dictionary","fvSchemes","system");
        writeFvSchemes();
        makeFoamFooter();
        fclose(fzout);


        //write the fvSolution file
        fzout = fopen(fvSolutionPath.c_str(),"w");
        makeFoamHeader("dictionary","fvSolution","system");
        writeFvSolution();
        makeFoamFooter();
        fclose(fzout);
    }

    //write the setFieldsDict file
    fzout = fopen(setFieldsDictPath.c_str(),"w");
    makeFoamHeader("dictionary","setFieldsDict","system");
    writeSetFieldsDict();
    makeFoamFooter();
    fclose(fzout);

    return true;
}

void OpenFoamSystemFiles::makeFoamHeader(std::string theClassType, std::string theObjectType,
                                      std::string theFoamFileLocation)
{
    fprintf(fzout,"/*--------------------------------*- C++ -*----------------------------------*\\\n");
    fprintf(fzout,"| =========                 |                                                 |\n");
    fprintf(fzout,"| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |\n");
    fprintf(fzout,"|  \\\\    /   O peration     | Version:  2.2.0                                 |\n");
    fprintf(fzout,"|   \\\\  /    A nd           | Web:      www.OpenFOAM.org                      |\n");
    fprintf(fzout,"|    \\\\/     M anipulation  |                                                 |\n");
    fprintf(fzout,"\\*---------------------------------------------------------------------------*/\n");
    fprintf(fzout,"FoamFile\n{\n");
    fprintf(fzout,"    version     %s;\n",foam_version.c_str());
    fprintf(fzout,"    format      ascii;\n");
    fprintf(fzout,"    class       %s;\n",theClassType.c_str());
    if (theFoamFileLocation != "" ) //need better error handling code here
    {
        fprintf(fzout,"    location    \"%s\";\n",theFoamFileLocation.c_str());
    }
    fprintf(fzout,"    object      %s;\n",theObjectType.c_str());
    fprintf(fzout,"}\n// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //\n\n");
}

void OpenFoamSystemFiles::makeFoamFooter()
{
    fprintf(fzout,"\n\n// ************************************************************************* //");
}

void OpenFoamSystemFiles::writeControlDict()
{
    fprintf(fzout,"application     %s;\n\n",application.c_str());
    fprintf(fzout,"startFrom       %s;\n\n",startFrom.c_str());
    fprintf(fzout,"startTime       %s;\n\n",startTime.c_str());
    fprintf(fzout,"stopAt          %s;\n\n",stopAt.c_str());
    fprintf(fzout,"endTime         %s;\n\n",endTime.c_str());
    fprintf(fzout,"deltaT          %s;\n\n",deltaT.c_str());
    fprintf(fzout,"writeControl    %s;\n\n",writeControl.c_str());
    fprintf(fzout,"writeInterval   %s;\n\n",writeInterval.c_str());
    fprintf(fzout,"purgeWrite      %s;\n\n",purgeWrite.c_str());
    fprintf(fzout,"writeFormat     %s;\n\n",writeFormat.c_str());
    fprintf(fzout,"writePrecision  %s;\n\n",writePrecision.c_str());
    fprintf(fzout,"writeCompression %s;\n\n",writeCompression.c_str());
    fprintf(fzout,"timeFormat      %s;\n\n",timeFormat.c_str());
    fprintf(fzout,"timePrecision   %s;\n\n",timePrecision.c_str());
    fprintf(fzout,"runTimeModifiable %s;\n",runTimeModifiable.c_str()); //one less new line since last line
}

void OpenFoamSystemFiles::writeFvSchemes()
{
    //could possibly write something that is called writeScheme that takes in
    //the scheme name and an array with what parts of the scheme will be written

    //write the ddtSchemes
    fprintf(fzout,"ddtSchemes\n{\n");
    fprintf(fzout,"    default         %s;\n",ddtSchemes_default.c_str());
    fprintf(fzout,"}\n\n");

    //write the gradSchemes
    fprintf(fzout,"gradSchemes\n{\n");
    fprintf(fzout,"    default         %s;\n",gradSchemes_default.c_str());
    fprintf(fzout,"}\n\n");

    //write the divSchemes
    fprintf(fzout,"divSchemes\n{\n");
    fprintf(fzout,"    default         %s;\n",divSchemes_default.c_str());
    fprintf(fzout,"    div(phi,T)      %s;\n",divSchemes_divOfPhiAndT.c_str());
    fprintf(fzout,"}\n\n");

    //write the laplacianSchemes
    fprintf(fzout,"laplacianSchemes\n{\n");
    fprintf(fzout,"    default         %s;\n",laplacianSchemes_default.c_str());
    fprintf(fzout,"    laplacian(DT,T) %s;\n",laplacianSchemes_laplacianOfDTandT.c_str());
    fprintf(fzout,"}\n\n");

    //write the interpolationSchemes
    fprintf(fzout,"interpolationSchemes\n{\n");
    fprintf(fzout,"    default         %s;\n",interpolationSchemes_default.c_str());
    fprintf(fzout,"}\n\n");

    //write the SnGradSchemes
    fprintf(fzout,"SnGradSchemes\n{\n");
    fprintf(fzout,"    default         %s;\n",SnGradSchemes_default.c_str());
    fprintf(fzout,"    SnGrad(T)       %s;\n",SnGradSchemes_SnGradOfT.c_str());
    fprintf(fzout,"}\n\n");

    //write the fluxRequired part
    fprintf(fzout,"fluxRequired\n{\n");
    fprintf(fzout,"    default         %s;\n",fluxRequired_default.c_str());
    if(fluxRequired_T == true)
    {
        fprintf(fzout,"    T;\n");
    }
    fprintf(fzout,"}\n");
}

void OpenFoamSystemFiles::writeFvSolution()
{
    fprintf(fzout,"solvers\n{\n");

    fprintf(fzout,"    T\n    {\n");
    fprintf(fzout,"        solver          %s;\n",solvers_T_solver.c_str());
    fprintf(fzout,"        preconditioner  %s;\n",solvers_T_preconditioner.c_str());
    fprintf(fzout,"        tolerance       %s;\n",solvers_T_tolerance.c_str());
    fprintf(fzout,"        relTol          %s;\n",solvers_T_relTol.c_str());
    fprintf(fzout,"    }\n");

    fprintf(fzout,"}\n\n");

    fprintf(fzout,"SIMPLE\n{\n");

    fprintf(fzout,"    nNonOrthogonalCorrectors %s;\n",SIMPLE_nNonOrthogonalCorrectors.c_str());

    fprintf(fzout,"}\n");
}

void OpenFoamSystemFiles::writeSetFieldsDict()
{
    fprintf(fzout,"defaultFieldValues\n(\n");
    fprintf(fzout,"    volScalarFieldValue source %s\n",defaultSourceValue.c_str());
    fprintf(fzout,");\n\n");

    fprintf(fzout,"regions\n(\n");
    if(distributionType == "boxToCell")
    {
        fprintf(fzout,"    boxToCell\n    {\n");
        fprintf(fzout,"        box (%s) (%s);\n",sourceMinCoordinates.c_str(),sourceMaxCoordinates.c_str());
        fprintf(fzout,"        fieldValues\n        (\n");
        fprintf(fzout,"            volScalarFieldValue source %s\n",distributionSourceValue.c_str());
        fprintf(fzout,"        );\n    }\n");
    }
    fprintf(fzout,");\n");
}
