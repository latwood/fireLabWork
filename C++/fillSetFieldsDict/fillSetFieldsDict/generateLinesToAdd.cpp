#include "generateLinesToAdd.h"

generateLinesToAdd::generateLinesToAdd(double minZstartFace_value, double northFaceStartFace_value,
                                       double dx_value, double dy_value, double dz_value)
{
    //now create the stuff that will need to be added in to replace this deleted/ignored stuff
    minZstartFace = minZstartFace_value;
    northFaceStartFace = northFaceStartFace_value;
    dx = dx_value;
    dy = dy_value;
    dz = dz_value;

    minZ_defaultTvalue.addTvalue("300");
    /*
    std::vector<std::string> minZ_desiredTvalues;
    minZ_desiredTvalues.push_back("310");
    minZ_desiredTvalues.push_back("305");
    minZ_desiredTvalues.push_back("300");
    minZ_desiredTvalues.push_back("305");
    minZ_desiredTvalues.push_back("310");
    minZ_Tvalues.generateLinearTprofile(minZ_desiredTvalues);
    */
    minZ_Tvalues.generateTprofile("310");
    //minZ_Tvalues.generateLinearTprofile(310,300,dx);

    northFace_defaultTvalue.addTvalue("300");
    /*
    std::vector<std::string> northFace_desiredTvalues;
    northFace_desiredTvalues.push_back("310");
    northFace_desiredTvalues.push_back("305");
    northFace_desiredTvalues.push_back("300");
    northFace_Tvalues.generateLinearTprofile(northFace_desiredTvalues);
    */
    //northFace_Tvalues.generateTprofile("310");
    northFace_Tvalues.generateLinearTprofile(310,300,6);

    westFace_defaultTvalue.addTvalue("300");
    //westFace_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> westFace_desiredTvalues;
    westFace_desiredTvalues.push_back("310");
    westFace_desiredTvalues.push_back("305");
    westFace_desiredTvalues.push_back("300");
    westFace_Tvalues.generateLinearTprofile(westFace_desiredTvalues);
    */
    westFace_Tvalues.generateLinearTprofile(305,300,6);

    eastFace_defaultTvalue.addTvalue("300");
    //eastFace_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> eastFace_desiredTvalues;
    eastFace_desiredTvalues.push_back("310");
    eastFace_desiredTvalues.push_back("305");
    eastFace_desiredTvalues.push_back("300");
    eastFace_Tvalues.generateLinearTprofile(eastFace_desiredTvalues);
    */
    eastFace_Tvalues.generateLinearTprofile(305,300,6);

    southFace_defaultTvalue.addTvalue("300");
    //southFace_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> southFace_desiredTvalues;
    southFace_desiredTvalues.push_back("310");
    southFace_desiredTvalues.push_back("305");
    southFace_desiredTvalues.push_back("300");
    southFace_Tvalues.generateLinearTprofile(southFace_desiredTvalues);
    */
    southFace_Tvalues.generateLinearTprofile(305,300,6);

    maxZ_defaultTvalue.addTvalue("300");
    //maxZ_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> maxZ_desiredTvalues;
    maxZ_desiredTvalues.push_back("310");
    maxZ_desiredTvalues.push_back("305");
    maxZ_desiredTvalues.push_back("300");
    maxZ_Tvalues.generateLinearTprofile(maxZ_desiredTvalues);
    */
    maxZ_Tvalues.generateLinearTprofile(305,300,6);

    internalField_defaultTvalue.addTvalue("300");
    //internalField_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> internalField_desiredTvalues;
    internalField_desiredTvalues.push_back("310");
    internalField_desiredTvalues.push_back("305");
    internalField_desiredTvalues.push_back("300");
    internalField_Tvalues.generateLinearTprofile(internalField_desiredTvalues);
    */
    internalField_Tvalues.generateLinearTprofile(305,300,6);

    generateExampleMinZ();
    //generateAdjustableMinZ_topToBot();
    //generateAdjustableMinZ_leftToRight();
    //generateExampleNorthFace();
    generateAdjustableNorthFace();

    fillLinesToAdd();

}

std::vector<std::string> generateLinesToAdd::getLines()
{
    return linesToAdd;
}

void generateLinesToAdd::fillLinesToAdd()
{
  //fill minZ first
    //add minZ default values
    //default values reset all the values before doing what really needs to be filled
    std::vector<double> minZcellIndices = minZ_defaultTvalue.getCellIndices();
    if(minZcellIndices.size() != 0)
    {
        if(minZ_defaultTvalue.getValue() == "")
        {
            std::cout << "Error! minZ_defaultTvalue filled, but has no value!\n";
            std::exit(EXIT_FAILURE);
        }
        linesToAdd.push_back("	labelToFace");
        linesToAdd.push_back("	{");
        linesToAdd.push_back("		value");
        linesToAdd.push_back("		(");
        for(double i = 0; i < minZcellIndices.size(); i++)
        {
            std::ostringstream strm;
            strm << minZcellIndices[i];
            std::string str = strm.str();
            linesToAdd.push_back("		"+str);
        }
        linesToAdd.push_back("		);");
        linesToAdd.push_back("		fieldValues");
        linesToAdd.push_back("		(");
        linesToAdd.push_back("			volScalarFieldValue T "+minZ_defaultTvalue.getValue());
        linesToAdd.push_back("		);");
        linesToAdd.push_back("	}");
    }

    //now add all the rest of the minZ values
    for(size_t t = 0; t < minZ_Tvalues.size(); t++)
    {
        minZcellIndices = minZ_Tvalues[t].getCellIndices();
        if(minZcellIndices.size() != 0)
        {
            linesToAdd.push_back("	labelToFace");
            linesToAdd.push_back("	{");
            linesToAdd.push_back("		value");
            linesToAdd.push_back("		(");
            for(double i = 0; i < minZcellIndices.size(); i++)
            {
                std::ostringstream strm;
                strm << minZcellIndices[i];
                std::string str = strm.str();
                linesToAdd.push_back("		"+str);
            }
            linesToAdd.push_back("		);");
            linesToAdd.push_back("		fieldValues");
            linesToAdd.push_back("		(");
            linesToAdd.push_back("			volScalarFieldValue T "+minZ_Tvalues[t].getValue());
            linesToAdd.push_back("		);");
            linesToAdd.push_back("	}");
        }
    }

  //now fill north_face
    //add north_face default values
    //default values reset all the values before doing what really needs to be filled
    std::vector<double> northFaceCellIndices = northFace_defaultTvalue.getCellIndices();
    if(northFaceCellIndices.size() != 0)
    {
        if(northFace_defaultTvalue.getValue() == "")
        {
            std::cout << "Error! northFace_defaultTvalue filled, but has no value!\n";
            std::exit(EXIT_FAILURE);
        }
        linesToAdd.push_back("	labelToFace");
        linesToAdd.push_back("	{");
        linesToAdd.push_back("		value");
        linesToAdd.push_back("		(");
        for(double i = 0; i < northFaceCellIndices.size(); i++)
        {
            std::ostringstream strm;
            strm << northFaceCellIndices[i];
            std::string str = strm.str();
            linesToAdd.push_back("		"+str);
        }
        linesToAdd.push_back("		);");
        linesToAdd.push_back("		fieldValues");
        linesToAdd.push_back("		(");
        linesToAdd.push_back("			volScalarFieldValue T "+northFace_defaultTvalue.getValue());
        linesToAdd.push_back("		);");
        linesToAdd.push_back("	}");
    }

    //now add all the rest of the north_face values
    for(size_t t = 0; t < northFace_Tvalues.size(); t++)
    {
        northFaceCellIndices = northFace_Tvalues[t].getCellIndices();
        if(northFaceCellIndices.size() != 0)
            {
            linesToAdd.push_back("	labelToFace");
            linesToAdd.push_back("	{");
            linesToAdd.push_back("		value");
            linesToAdd.push_back("		(");
            for(double i = 0; i < northFaceCellIndices.size(); i++)
            {
                std::ostringstream strm;
                strm << northFaceCellIndices[i];
                std::string str = strm.str();
                linesToAdd.push_back("		"+str);
            }
            linesToAdd.push_back("		);");
            linesToAdd.push_back("		fieldValues");
            linesToAdd.push_back("		(");
            linesToAdd.push_back("			volScalarFieldValue T "+northFace_Tvalues[t].getValue());
            linesToAdd.push_back("		);");
            linesToAdd.push_back("	}");
        }
    }
}

void generateLinesToAdd::generateExampleMinZ()
{
//This section is for filling in minZ with methods that aren't as structured. These are more easily
//manipulated to look at how each individual point is filled in.
    if(minZ_Tvalues.size() != 1)
    {
        std::cout <<  "Error, cannot perform function generateExampleMinZ if " <<
                      "the Tprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    } else
    {
        //this resets all the values before doing what really needs to be filled
        for(double j = 0; j < dx*dy; j++)
        {
            minZ_defaultTvalue.addCellIndex(minZstartFace+j);
        }

        //this how to fill in the points breaking it out into multiple loops
        for(double j = 0; j < dy; j++)
        {
            for(double i = 0; i < dx; i++)
            {
                minZ_Tvalues[0].addCellIndex(minZstartFace+dx*j+i);
            }
        }
    }
}

void generateLinesToAdd::generateAdjustableMinZ_topToBot()
{
//This section is for filling in minZ from top to bottom with methods that are structured.
//This supports having multiple values for the Tprofile
    if(minZ_Tvalues.size() <= 0)
    {
        std::cout <<  "Error, cannot perform function generateAdjustableMinZ_topToBot if " <<
                      "the Tprofile is size <= 0!\n";
        std::exit(EXIT_FAILURE);
    } else
    {
        //this resets all the values before doing what really needs to be filled
        for(double j = 0; j < dx*dy; j++)
        {
                minZ_defaultTvalue.addCellIndex(minZstartFace+j);
        }

        double divisor = minZ_Tvalues.size();
        for(size_t t = 0; t < divisor; t++)
        {
            //this how to fill in the points breaking it out into multiple loops
            for(double i = 0; i < dx; i++)
            {
                for(double j = t*dy/divisor; j < (t+1)*dy/divisor; j++)
                {
                    minZ_Tvalues[t].addCellIndex(minZstartFace+dy*i+j);
                }
            }
        }
    }
}

void generateLinesToAdd::generateAdjustableMinZ_leftToRight()
{
//This section is for filling in minZ from left to right with methods that are structured.
//This supports having multiple values for the Tprofile
    if(minZ_Tvalues.size() <= 0)
    {
        std::cout <<  "Error, cannot perform function generateAdjustableMinZ_leftToRight if " <<
                      "the Tprofile is size <= 0!\n";
        std::exit(EXIT_FAILURE);
    } else
    {
        //this resets all the values before doing what really needs to be filled
        for(double j = 0; j < dx*dy; j++)
        {
            minZ_defaultTvalue.addCellIndex(minZstartFace+j);
        }

        double divisor = minZ_Tvalues.size();
        for(size_t t = 0; t < divisor; t++)
        {
            //this how to fill in the points breaking it out into multiple loops
            for(double i = t*dx/divisor; i < (t+1)*dx/divisor; i++)
            {
                for(double j = 0; j < dy; j++)
                {
                    minZ_Tvalues[t].addCellIndex(minZstartFace+dy*i+j);
                }
            }
        }
    }
}

void generateLinesToAdd::generateExampleNorthFace()
{
//This section is for filling in north_face with methods that aren't as structured. These are more easily
//manipulated to look at how each individual point is filled in.
    if(northFace_Tvalues.size() != 1)
    {
        std::cout <<  "Error, cannot perform function generateExampleNorthFace if " <<
                      "the Tprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    } else
    {
        //this resets all the values before doing what really needs to be filled
        for(double i = 0; i < dx*dz; i++)
        {
            northFace_defaultTvalue.addCellIndex(northFaceStartFace+i);
        }

        //this how to fill in the points breaking it out into multiple loops
        for(double i = 0; i < dx; i++)
        {
            for(double k = 0; k < dz; k++)
            {
                northFace_Tvalues[0].addCellIndex(northFaceStartFace+dz*i+k);
            }
        }
    }
}

void generateLinesToAdd::generateAdjustableNorthFace()
{
//This section is for filling in north_face from bottom to top with methods that are structured.
    if(northFace_Tvalues.size() <= 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableNorthFace if " <<
                     "the Tprofile is size <= 0!\n";
        std::exit(EXIT_FAILURE);
    } else
    {
        //this resets all the values before doing what really needs to be filled
        for(double i = 0; i < dx*dz; i++)
        {
            northFace_defaultTvalue.addCellIndex(northFaceStartFace+i);
        }

        double divisor = northFace_Tvalues.size();
        for(size_t t = 0; t < divisor; t++)
        {
            //this how to fill in the points breaking it out into multiple loops
            for(double i = 0; i < dx; i++)
            {
                for(double k = t*dz/divisor; k < (t+1)*dz/divisor; k++)
                {
                    northFace_Tvalues[t].addCellIndex(northFaceStartFace+dz*i+k);
                }
            }
        }
    }
}
