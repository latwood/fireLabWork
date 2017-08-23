#include "generateLinesToAdd.h"

generateLinesToAdd::generateLinesToAdd(double minZstartFace_value,double northFaceStartFace_value,
                                       double westFaceStartFace_value,double eastFaceStartFace_value,
                                       double southFaceStartFace_value,double maxZstartFace_value,
                                       double dx_value, double dy_value, double dz_value)
{
    //now create the stuff that will need to be added in to replace this deleted/ignored stuff
    minZstartFace = minZstartFace_value;
    northFaceStartFace = northFaceStartFace_value;
    westFaceStartFace = westFaceStartFace_value;
    eastFaceStartFace = eastFaceStartFace_value;
    southFaceStartFace = southFaceStartFace_value;
    maxZstartFace = maxZstartFace_value;
    dx = dx_value;
    dy = dy_value;
    dz = dz_value;

    minZ_defaultTvalue.generateTprofile("300");
    /*
    std::vector<std::string> minZ_desiredTvalues;
    minZ_desiredTvalues.push_back("310");
    minZ_desiredTvalues.push_back("305");
    minZ_desiredTvalues.push_back("300");
    minZ_desiredTvalues.push_back("305");
    minZ_desiredTvalues.push_back("310");
    minZ_Tvalues.generateLinearTprofile(minZ_desiredTvalues);
    */
    minZ_Tvalues.generateTprofile("300");
    //minZ_Tvalues.generateLinearTprofile(310,300,5);

    northFace_defaultTvalue.generateTprofile("300");
    northFace_Tvalues.generateTprofile("300");
    //northFace_Tvalues.generateLinearTprofile(310,300,2);

    westFace_defaultTvalue.generateTprofile("300");
    //westFace_Tvalues.generateTprofile("310");
    westFace_Tvalues.generateLinearTprofile(305,300,5);

    eastFace_defaultTvalue.generateTprofile("300");
    //eastFace_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> eastFace_desiredTvalues;
    eastFace_desiredTvalues.push_back("310");
    eastFace_desiredTvalues.push_back("305");
    eastFace_desiredTvalues.push_back("300");
    eastFace_Tvalues.generateLinearTprofile(eastFace_desiredTvalues);
    */
    eastFace_Tvalues.generateLinearTprofile(305,300,6);

    southFace_defaultTvalue.generateTprofile("300");
    //southFace_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> southFace_desiredTvalues;
    southFace_desiredTvalues.push_back("310");
    southFace_desiredTvalues.push_back("305");
    southFace_desiredTvalues.push_back("300");
    southFace_Tvalues.generateLinearTprofile(southFace_desiredTvalues);
    */
    southFace_Tvalues.generateLinearTprofile(305,300,6);

    maxZ_defaultTvalue.generateTprofile("300");
    //maxZ_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> maxZ_desiredTvalues;
    maxZ_desiredTvalues.push_back("310");
    maxZ_desiredTvalues.push_back("305");
    maxZ_desiredTvalues.push_back("300");
    maxZ_Tvalues.generateLinearTprofile(maxZ_desiredTvalues);
    */
    maxZ_Tvalues.generateLinearTprofile(305,300,6);

    internalField_defaultTvalue.generateTprofile("300");
    //internalField_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> internalField_desiredTvalues;
    internalField_desiredTvalues.push_back("310");
    internalField_desiredTvalues.push_back("305");
    internalField_desiredTvalues.push_back("300");
    internalField_Tvalues.generateLinearTprofile(internalField_desiredTvalues);
    */
    internalField_Tvalues.generateLinearTprofile(305,300,6);

    //generateExampleMinZ();
    //generateAdjustableMinZ_topToBot();
    //generateAdjustableMinZ_leftToRight();
    //generateExampleNorthFace();
    //generateAdjustableNorthFace();
    //generateExampleWestFace();
    generateAdjustableWestFace();
    //generateExampleEastFace();
    //generateAdjustableEastFace();
    //generateExampleSouthFace();
    //generateAdjustableSouthFace();

    fillLinesToAdd(minZ_defaultTvalue,minZ_Tvalues);
    fillLinesToAdd(northFace_defaultTvalue,northFace_Tvalues);
    fillLinesToAdd(westFace_defaultTvalue,westFace_Tvalues);
    fillLinesToAdd(eastFace_defaultTvalue,eastFace_Tvalues);
    fillLinesToAdd(southFace_defaultTvalue,southFace_Tvalues);
    fillLinesToAdd(maxZ_defaultTvalue,maxZ_Tvalues);

}

std::vector<std::string> generateLinesToAdd::getLines()
{
    return linesToAdd;
}

void generateLinesToAdd::fillLinesToAdd(Tprofile defaultValue,Tprofile theValues)
{
    //default values reset all the values before doing what really needs to be filled
    for(size_t t = 0; t < defaultValue.size(); t++)
    {
        std::vector<double> cellIndices = defaultValue[t].getCellIndices();
        if(cellIndices.size() != 0)
        {
            linesToAdd.push_back("	labelToFace");
            linesToAdd.push_back("	{");
            linesToAdd.push_back("		value");
            linesToAdd.push_back("		(");
            for(double i = 0; i < cellIndices.size(); i++)
            {
                std::ostringstream strm;
                strm << cellIndices[i];
                std::string str = strm.str();
                linesToAdd.push_back("		"+str);
            }
            linesToAdd.push_back("		);");
            linesToAdd.push_back("		fieldValues");
            linesToAdd.push_back("		(");
            linesToAdd.push_back("			volScalarFieldValue T "+defaultValue[t].getValue());
            linesToAdd.push_back("		);");
            linesToAdd.push_back("	}");
        }
    }

    //now add all the rest of the values
    for(size_t t = 0; t < theValues.size(); t++)
    {
        std::vector<double> cellIndices = theValues[t].getCellIndices();
        if(cellIndices.size() != 0)
        {
            linesToAdd.push_back("	labelToFace");
            linesToAdd.push_back("	{");
            linesToAdd.push_back("		value");
            linesToAdd.push_back("		(");
            for(double i = 0; i < cellIndices.size(); i++)
            {
                std::ostringstream strm;
                strm << cellIndices[i];
                std::string str = strm.str();
                linesToAdd.push_back("		"+str);
            }
            linesToAdd.push_back("		);");
            linesToAdd.push_back("		fieldValues");
            linesToAdd.push_back("		(");
            linesToAdd.push_back("			volScalarFieldValue T "+theValues[t].getValue());
            linesToAdd.push_back("		);");
            linesToAdd.push_back("	}");
        }
    }
}

void generateLinesToAdd::generateExampleMinZ()
{
//This section is for filling in minZ with methods that aren't as structured. These are more easily
//manipulated to look at how each individual point is filled in.
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateExampleMinZ if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_Tvalues.size() != 1)
    {
        std::cout <<  "Error, cannot perform function generateExampleMinZ if " <<
                      "the Tprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double j = 0; j < dx*dy; j++)
    {
        minZ_defaultTvalue[0].addCellIndex(minZstartFace+j);
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

void generateLinesToAdd::generateAdjustableMinZ_topToBot()
{
//This section is for filling in minZ from top to bottom with methods that are structured.
//This supports having multiple values for the Tprofile, but the number of Tprofile values has to be
//a divisor of dy, or stuff gets placed out of order
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_topToBot if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_Tvalues.size() <= 0 || fmod(dy,minZ_Tvalues.size()) != 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_topToBot if " <<
                     "the Tprofile doesn't have size that can evenly divide dy!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double j = 0; j < dx*dy; j++)
    {
            minZ_defaultTvalue[0].addCellIndex(minZstartFace+j);
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

void generateLinesToAdd::generateAdjustableMinZ_leftToRight()
{
//This section is for filling in minZ from left to right with methods that are structured.
//This supports having multiple values for the Tprofile, but the number of Tprofile values has to be
//a divisor of dx, or stuff gets placed out of order
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_leftToRight if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_Tvalues.size() <= 0 || fmod(dx,minZ_Tvalues.size()))
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_leftToRight if " <<
                     "the Tprofile doesn't have size that can evenly divide dx!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double j = 0; j < dx*dy; j++)
    {
        minZ_defaultTvalue[0].addCellIndex(minZstartFace+j);
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

void generateLinesToAdd::generateExampleNorthFace()
{
//This section is for filling in north_face with methods that aren't as structured. These are more easily
//manipulated to look at how each individual point is filled in.
    if(northFace_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateExampleNorthFace if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(northFace_Tvalues.size() != 1)
    {
        std::cout <<  "Error, cannot perform function generateExampleNorthFace if " <<
                      "the Tprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double i = 0; i < dx*dz; i++)
    {
        northFace_defaultTvalue[0].addCellIndex(northFaceStartFace+i);
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

void generateLinesToAdd::generateAdjustableNorthFace()
{
//This section is for filling in north_face from bottom to top with methods that are structured.
    if(northFace_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableNorthFace if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(northFace_Tvalues.size() <= 0 || fmod(dz,northFace_Tvalues.size()))
    {
        std::cout << "Error, cannot perform function generateAdjustableNorthFace if " <<
                     "the Tprofile doesn't have size that can evenly divide dz!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double i = 0; i < dx*dz; i++)
    {
        northFace_defaultTvalue[0].addCellIndex(northFaceStartFace+i);
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

void generateLinesToAdd::generateExampleWestFace()
{
//This section is for filling in north_face with methods that aren't as structured. These are more easily
//manipulated to look at how each individual point is filled in.
    if(westFace_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateExampleWestFace if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(westFace_Tvalues.size() != 1)
    {
        std::cout <<  "Error, cannot perform function generateExampleWestFace if " <<
                      "the Tprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double j = 0; j < dy*dz; j++)
    {
        westFace_defaultTvalue[0].addCellIndex(westFaceStartFace+j);
    }

    //this how to fill in the points breaking it out into multiple loops
    for(double k = 0; k < dz; k++)
    {
        for(double j = 0; j < dy; j++)
        {
            westFace_Tvalues[0].addCellIndex(westFaceStartFace+dy*k+j);
        }
    }
}

void generateLinesToAdd::generateAdjustableWestFace()
{
//This section is for filling in north_face from bottom to top with methods that are structured.
    if(westFace_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableWestFace if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(westFace_Tvalues.size() <= 0 || fmod(dz,westFace_Tvalues.size()))
    {
        std::cout << "Error, cannot perform function generateAdjustableWestFace if " <<
                     "the Tprofile doesn't have size that can evenly divide dz!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double j = 0; j < dy*dz; j++)
    {
        westFace_defaultTvalue[0].addCellIndex(westFaceStartFace+j);
    }

    double divisor = westFace_Tvalues.size();
    for(size_t t = 0; t < divisor; t++)
    {
        //this how to fill in the points breaking it out into multiple loops
        for(double k = t*dz/divisor; k < (t+1)*dz/divisor; k++)
        {
            for(double j = 0; j < dy; j++)
            {
                westFace_Tvalues[t].addCellIndex(westFaceStartFace+dy*k+j);
            }
        }
    }
}
