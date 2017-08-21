#ifndef EDITSETFIELDSDICT_H
#define EDITSETFIELDSDICT_H

#include <vector>
#include <fstream>

#include "generateLinesToAdd.h"
#include "generateLinesToAddRefinedMesh.h"

class editSetFieldsDict
{
public:

    //constructor
    editSetFieldsDict(std::string setFieldsDictPath_value);

private:

    //first read the file. Includes the generateLinesToAddRefinedMesh function
    void readSetFieldsDict();

    //uses the linesToKeep and linesToAdd information to write the new setFieldsDict. All other lines are left out.
    void writeSetFieldsDict();

    std::string setFieldsDictPath;

    std::vector<std::string> linesToKeep;
    std::vector<double> addLineIndices; //hm, need a way to know how many lines to add if adding more than one section
    std::vector<std::string> linesToAdd;
};

#endif // EDITSETFIELDSDICT_H
