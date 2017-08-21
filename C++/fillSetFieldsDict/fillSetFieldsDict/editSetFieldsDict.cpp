#include "editSetFieldsDict.h"

editSetFieldsDict::editSetFieldsDict(std::string setFieldsDictPath_value)
{
    setFieldsDictPath = setFieldsDictPath_value;

    readSetFieldsDict();
    //generateLinesToAdd linesToAddGenerator(363294,359766,45,39,6);
    generateLinesToAddRefinedMesh linesToAddGenerator(363294,359766,45,39,6,2);
    linesToAdd = linesToAddGenerator.getLines();
    writeSetFieldsDict();
}

void editSetFieldsDict::readSetFieldsDict()
{
    std::ifstream is_file(setFieldsDictPath.c_str());
    std::string line;

    while(std::getline(is_file,line))
    {
        /*
         * Now set some triggers, the idea being that triggers are when to act to ignore or skip lines.
         * Everything else should be stored, and when the triggers are processed, some more stuff can also
         * be stored. At a minimum, everything else other than the trigger should be stored.
         */

        /* at some point need to have a separate class called first that generates all the generic lines
         * that are simply form headers, but places keywords that are the triggers so that this function
         * can catch the right triggers in ways that are easier to debug.
         *
         * probably should keep the triggers, or rename the triggers or add two triggers for adding in the
         * stuff replaced by the program. One trigger is for the generic information that for now is constant
         * in windninja and other openfoam simulations while the other trigger is for the new information
         * specifically generated in different ways by how the user specifies it to be done in this program
         */
        if (line == "regions")
        {
            //finish collecting needed parts of the file, and skip through the delete/ignore part
            linesToKeep.push_back("regions");
            linesToKeep.push_back("(");
            addLineIndices.push_back(linesToKeep.size()-1);
            //go to the very end of the regions section. This should do it, since the others have tab );
            //though technically white space doesn't matter so this will need improved
            while(line != ");")
            {
                std::getline(is_file,line);
            }
            linesToKeep.push_back(");");//will be missed otherwise
        } else //assume everything other than all the triggers above is stored, and that all necessary placeholder information is set right
        {
            linesToKeep.push_back(line);
        }
    }
    is_file.close();    
}

void editSetFieldsDict::writeSetFieldsDict()
{
    //now write the file
    FILE *fzout = fopen(setFieldsDictPath.c_str(),"w");
    double addLineIndicesCounter = 0;
    for (double i = 0; i < linesToKeep.size(); i++)
    {
        fprintf(fzout,"%s\n",linesToKeep[i].c_str());
        if(i == addLineIndices[addLineIndicesCounter])
        {
            for(double j = 0; j < linesToAdd.size(); j++)
            {
                fprintf(fzout,"%s\n",linesToAdd[j].c_str());
            }
            addLineIndicesCounter = addLineIndicesCounter + 1;
        }
    }
    fclose(fzout);
}
