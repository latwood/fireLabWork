#include "generateLinesToAddRefinedMesh.h"

generateLinesToAddRefinedMesh::generateLinesToAddRefinedMesh(double minZstartFace_value, double northFaceStartFace_value,
                                       double dx_value, double dy_value, double dz_value, double timesRefined_value)
{
    //now create the stuff that will need to be added in to replace this deleted/ignored stuff
    minZstartFace = minZstartFace_value;
    northFaceStartFace = northFaceStartFace_value;
    dx = dx_value;
    dy = dy_value;
    dz = dz_value;
    timesRefined = timesRefined_value;
    dx_refined = pow(2,timesRefined);
    dy_refined = pow(2,timesRefined);
    dz_refined = pow(2,timesRefined);

    minZ_defaultTvalue.generateTprofile("300");
    minZ_Tvalues.generateTprofile("310");
    //minZ_Tvalues.generateLinearTprofile(310,300,dy);

    northFace_defaultTvalue.generateTprofile("300");
    northFace_Tvalues.generateTprofile("310");
    //northFace_Tvalues.generateLinearTprofile(310,300,dz);

    westFace_defaultTvalue.generateTprofile("300");
    //westFace_Tvalues.generateTprofile("310");
    /*
    std::vector<std::string> westFace_desiredTvalues;
    westFace_desiredTvalues.push_back("310");
    westFace_desiredTvalues.push_back("305");
    westFace_desiredTvalues.push_back("300");
    westFace_Tvalues.generateLinearTprofile(westFace_desiredTvalues);
    */
    westFace_Tvalues.generateLinearTprofile(305,300,6);

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

    //could have an input checker function here before getting into the execution

    generateExampleMinZ();
    //generateAdjustableMinZ_topToBot();
    //generateAdjustableMinZ_simplest_topToBot();
    //generateAdjustableMinZ_refineCells_topToBot();
    //generateAdjustableMinZ_leftToRight();
    //generateAdjustableMinZ_simplest_leftToRight();
    //generateAdjustableMinZ_refineCells_leftToRight();
    generateExampleNorthFace();
    //generateAdjustableNorthFace_simplest();
    //generateAdjustableNorthFace_refineCells();

    fillLinesToAdd(minZ_defaultTvalue,minZ_Tvalues);
    fillLinesToAdd(northFace_defaultTvalue,northFace_Tvalues);

}

std::vector<std::string> generateLinesToAddRefinedMesh::getLines()
{
    return linesToAdd;
}

void generateLinesToAddRefinedMesh::fillLinesToAdd(Tprofile defaultValue,Tprofile theValues)
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

void generateLinesToAddRefinedMesh::generateExampleMinZ()
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
        std::cout << "Error, cannot perform function generateExampleMinZ if " <<
                     "the Tprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double k = 0; k < dx_refined*dy_refined; k++)  //if timesRefined == 0, this still works
    {
        for(double j = 0; j < dx*dy; j++)
        {
            minZ_defaultTvalue[0].addCellIndex(minZstartFace+dx*dy*k+j);
        }
    }

    //this is how to fill the first set of points, the cells from before refine mesh
    for(double k = 0; k < 1; k++)
    {
        for(double j = 0; j < dy; j++)
        {
            for(double i = 0; i < dx; i++)
            {
                minZ_Tvalues[0].addCellIndex(minZstartFace+dx*dy*k+dx*j+i);
            }
        }
    }

    if(timesRefined != 0)   //only needed to fill out the above points if timesRefined == 0
    {
        //fill in the first set of split cells from refine mesh
        //fills rest of the outer corners of the box first
        for(double j = 0; j < (dy*3)*dx; j++)
        {
            minZ_Tvalues[0].addCellIndex(minZstartFace+dx*dy*1+j);
        }

        if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
        {
            //fill in the second part of split cells from refine mesh
            //seems to fill in from the outside
            for(double j = 0; j < (dy*3)*dx; j++)
            {
                minZ_Tvalues[0].addCellIndex(minZstartFace+dx*dy*4+j);
            }

            //fills in the third and final bunch of split cells from refine mesh
            //seems to only fill the diagonals, then let the second set fill the rest of the diagonal
            for(double j = 0; j < 9*dy*dx; j++)
            {
                minZ_Tvalues[0].addCellIndex(minZstartFace+dx*dy*7+j);
            }
        }
    }
}

void generateLinesToAddRefinedMesh::generateAdjustableMinZ_topToBot()
{
//This section is for filling in minZ from top to bottom with methods that are structured.
//This supports having multiple values for the Tprofile, but the number of Tprofile values has to be
//a divisor of dy, or stuff gets placed out of order
    if(timesRefined == 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_topToBot if " <<
                     "timesRefined == 0! Use generateExampleMinZ instead!\n";
        std::exit(EXIT_FAILURE);
    }
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
    for(double k = 0; k < dx_refined*dy_refined; k++)
    {
        for(double j = 0; j < dx*dy; j++)
        {
            minZ_defaultTvalue[0].addCellIndex(minZstartFace+dx*dy*k+j);
        }
    }

    double divisor = minZ_Tvalues.size();
    for(size_t t = 0; t < divisor; t++)
    {
        //this is how to fill the first set of points, the cells from before refine mesh
        for(double k = 0; k < 1; k++)
        {
            for(double i = 0; i < dx; i++)
            {
                for(double j = t*dy/divisor; j < (t+1)*dy/divisor; j++)
                {
                    minZ_Tvalues[t].addCellIndex(minZstartFace+dx*dy*k+dy*i+j);
                }
            }
        }

        //fill in the first set of split cells from refine mesh
        //fills rest of the outer corners of the box first
        for(double i = 0; i < dx; i++)
        {
            for(double j = t*dy/divisor; j < (t+1)*dy/divisor; j++)
            {
                for(double r = 0; r < 3; r++)
                {
                    minZ_Tvalues[t].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+r);
                }
            }
        }

        if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
        {
            //fill in the second part of split cells from refine mesh
            //seems to fill in from the outside
            for(double i = 0; i < dx; i++)
            {
                for(double j = t*dy/divisor; j < (t+1)*dy/divisor; j++)
                {
                    for(double r = 0; r < 3; r++)
                    {
                        minZ_Tvalues[t].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+r);
                    }
                }
            }

            //fills in the third and final bunch of split cells from refine mesh
            //seems to only fill the diagonals, then let the second set fill the rest of the diagonal
            for(double i = 0; i < dx; i++)
            {
                for(double j = t*dy/divisor; j < (t+1)*dy/divisor; j++)
                {
                    for(double r = 0; r < 9; r++)
                    {
                        minZ_Tvalues[t].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+r);
                    }
                }
            }
        }
    }
}

void generateLinesToAddRefinedMesh::generateAdjustableMinZ_simplest_topToBot()
{
//This section is for filling in minZ from top to bottom with methods that are structured.
//The number of Tprofile values has to be equal to dy, or stuff gets placed out of order
    if(timesRefined == 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_simplest_topToBot if " <<
                     "timesRefined == 0! Use generateExampleMinZ instead!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_simplest_topToBot if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_Tvalues.size() != dy)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_simplest_topToBot if " <<
                     "the Tprofile doesn't have size dy!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double k = 0; k < dx_refined*dy_refined; k++)
    {
        for(double j = 0; j < dx*dy; j++)
        {
            minZ_defaultTvalue[0].addCellIndex(minZstartFace+dx*dy*k+j);
        }
    }

    //this is how to fill the first set of points, the cells from before refine mesh
    for(double k = 0; k < 1; k++)
    {
        for(double i = 0; i < dx; i++)
        {
            for(double j = 0; j < dy; j++)
            {
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*k+dy*i+j);
            }
        }
    }

    //fill in the first set of split cells from refine mesh
    //fills rest of the outer corners of the box first
    for(double i = 0; i < dx; i++)
    {
        for(double j = 0; j < dy; j++)
        {
            minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3); //I bet this 3 is some kind of dy_refined -1.
            minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+1);
            minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+2);
        }
    }

    if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
    {
        //fill in the second part of split cells from refine mesh
        //seems to fill in from the outside
        for(double i = 0; i < dx; i++)
        {
            for(double j = 0; j < dy; j++)
            {
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+1);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+2);
            }
        }
        //fills in the third and final bunch of split cells from refine mesh
        //seems to only fill the diagonals, then let the second set fill the rest of the diagonal
        for(double i = 0; i < dx; i++)
        {
            for(double j = 0; j < dy; j++)
            {
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+1);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+2);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+3);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+4);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+5);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+6);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+7);
                minZ_Tvalues[j].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+8);
            }
        }
    }
}

void generateLinesToAddRefinedMesh::generateAdjustableMinZ_refineCells_topToBot()
{
    //This section is for filling in minZ from top to bottom with methods that are structured.
    //The number of Tprofile values has to be equal to dy*dy_refined, or stuff gets placed out of order
    if(timesRefined == 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_refineCells_topToBot if " <<
                     "timesRefined == 0! Use generateExampleMinZ instead!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_refineCells_topToBot if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_Tvalues.size() != dy*dy_refined)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_refineCells_topToBot if " <<
                     "the Tprofile doesn't have size dy*dy_refined!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double k = 0; k < dx_refined*dy_refined; k++)
    {
        for(double j = 0; j < dx*dy; j++)
        {
            minZ_defaultTvalue[0].addCellIndex(minZstartFace+dx*dy*k+j);
        }
    }

    //this is how to fill the first set of points, the cells from before refine mesh
    for(double k = 0; k < 1; k++)
    {
        for(double i = 0; i < dx; i++)
        {
            for(double j = 0; j < dy; j++)
            {
                minZ_Tvalues[j*dy_refined].addCellIndex(minZstartFace+dx*dy*k+dy*i+j);
            }
        }
    }

    //fill in the first set of split cells from refine mesh
    //fills rest of the outer corners of the box first
    for(double i = 0; i < dx; i++)
    {
        for(double j = 0; j < dy; j++)
        {
            minZ_Tvalues[j*dy_refined+dy_refined-1].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3); //I bet this 3 is some kind of dy_refined -1.
            minZ_Tvalues[j*dy_refined+dy_refined-1].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+1);
            minZ_Tvalues[j*dy_refined].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+2);
        }
    }

    if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
    {
        //fill in the second part of split cells from refine mesh
        //seems to fill in from the outside
        for(double i = 0; i < dx; i++)
        {
            for(double j = 0; j < dy; j++)
            {
                minZ_Tvalues[j*dy_refined+1].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3);
                minZ_Tvalues[j*dy_refined+1].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+1);
                minZ_Tvalues[j*dy_refined].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+2);
            }
        }
        //fills in the third and final bunch of split cells from refine mesh
        //seems to only fill the diagonals, then let the second set fill the rest of the diagonal
        for(double i = 0; i < dx; i++)
        {
            for(double j = 0; j < dy; j++)
            {
                minZ_Tvalues[j*dy_refined+dy_refined-1].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9);
                minZ_Tvalues[j*dy_refined+dy_refined-2].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+1);
                minZ_Tvalues[j*dy_refined+dy_refined-2].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+2);
                minZ_Tvalues[j*dy_refined+dy_refined-2].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+3);
                minZ_Tvalues[j*dy_refined+dy_refined-2].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+4);
                minZ_Tvalues[j*dy_refined+dy_refined-1].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+5);
                minZ_Tvalues[j*dy_refined].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+6);
                minZ_Tvalues[j*dy_refined+1].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+7);
                minZ_Tvalues[j*dy_refined+1].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+8);
            }
        }
    }
}

void generateLinesToAddRefinedMesh::generateAdjustableMinZ_leftToRight()
{
//This section is for filling in minZ from left to right with methods that are structured.
//This supports having multiple values for the Tprofile, but the number of Tprofile values has to be
//a divisor of dx, or stuff gets placed out of order
    if(timesRefined == 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_leftToRight if " <<
                     "timesRefined == 0! Use generateExampleMinZ instead!\n";
        std::exit(EXIT_FAILURE);
    }
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
    for(double k = 0; k < dx_refined*dy_refined; k++)
    {
        for(double j = 0; j < dx*dy; j++)
        {
            minZ_defaultTvalue[0].addCellIndex(minZstartFace+dx*dy*k+j);
        }
    }

    double divisor = minZ_Tvalues.size();
    for(size_t t = 0; t < divisor; t++)
    {
        //this is how to fill the first set of points, the cells from before refine mesh
        for(double k = 0; k < 1; k++)
        {
            for(double i = t*dx/divisor; i < (t+1)*dx/divisor; i++)
            {
                for(double j = 0; j < dy; j++)
                {
                    minZ_Tvalues[t].addCellIndex(minZstartFace+dx*dy*k+dy*i+j);
                }
            }
        }

        //fill in the first set of split cells from refine mesh
        //fills rest of the outer corners of the box first
        for(double i = t*dx/divisor; i < (t+1)*dx/divisor; i++)
        {
            for(double j = 0; j < dy; j++)
            {
                for(double r = 0; r < 3; r++)
                {
                    minZ_Tvalues[t].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+r);
                }
            }
        }

        if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
        {
            //fill in the second part of split cells from refine mesh
            //seems to fill in from the outside
            for(double i = t*dx/divisor; i < (t+1)*dx/divisor; i++)
            {
                for(double j = 0; j < dy; j++)
                {
                    for(double r = 0; r < 3; r++)
                    {
                        minZ_Tvalues[t].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+r);
                    }
                }
            }

            //fills in the third and final bunch of split cells from refine mesh
            //seems to only fill the diagonals, then let the second set fill the rest of the diagonal
            for(double i = t*dx/divisor; i < (t+1)*dx/divisor; i++)
            {
                for(double j = 0; j < dy; j++)
                {
                    for(double r = 0; r < 9; r++)
                    {
                        minZ_Tvalues[t].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+r);
                    }
                }
            }
        }
    }
}

void generateLinesToAddRefinedMesh::generateAdjustableMinZ_simplest_leftToRight()
{
//This section is for filling in minZ from left to right with methods that are structured.
//The number of Tprofile values has to be equal to dx, or stuff gets placed out of order
    if(timesRefined == 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_simplest_leftToRight if " <<
                     "timesRefined == 0! Use generateExampleMinZ instead!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_simplest_leftToRight if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_Tvalues.size() != dx)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_simplest_leftToRight " <<
                     "if the Tprofile doesn't have size dx!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double k = 0; k < dx_refined*dy_refined; k++)
    {
        for(double j = 0; j < dx*dy; j++)
        {
            minZ_defaultTvalue[0].addCellIndex(minZstartFace+dx*dy*k+j);
        }
    }

    //this is how to fill the first set of points, the cells from before refine mesh
    for(double k = 0; k < 1; k++)
    {
        for(double j = 0; j < dy; j++)
        {
            for(double i = 0; i < dx; i++)
            {
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*k+dy*i+j);
            }
        }
    }

    //fill in the first set of split cells from refine mesh
    //fills rest of the outer corners of the box first
    for(double j = 0; j < dy; j++)
    {
        for(double i = 0; i < dx; i++)
        {
            minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3); //I bet this 3 is some kind of dy_refined -1.
            minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+1);
            minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+2);
        }
    }

    if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
    {
        //fill in the second part of split cells from refine mesh
        //seems to fill in from the outside
        for(double j = 0; j < dy; j++)
        {
            for(double i = 0; i < dx; i++)
            {
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+1);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+2);
            }
        }
        //fills in the third and final bunch of split cells from refine mesh
        //seems to only fill the diagonals, then let the second set fill the rest of the diagonal
        for(double j = 0; j < dy; j++)
        {
            for(double i = 0; i < dx; i++)
            {
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+1);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+2);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+3);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+4);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+5);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+6);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+7);
                minZ_Tvalues[i].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+8);
            }
        }
    }
}

void generateLinesToAddRefinedMesh::generateAdjustableMinZ_refineCells_leftToRight()
{
//This section is for filling in minZ from left to right with methods that are structured.
//The number of Tprofile values has to be equal to dx*dx_refined, or stuff gets placed out of order
    if(timesRefined == 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_refineCells_leftToRight if " <<
                     "timesRefined == 0! Use generateExampleMinZ instead!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableMinZ_refineCells_leftToRight if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_Tvalues.size() != dx*dx_refined)
    {
        std::cout <<  "Error, cannot perform function generateAdjustableMinZ_refineCells_leftToRight " <<
                      "if the Tprofile doesn't have size dx*dx_refined!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    for(double k = 0; k < dx_refined*dy_refined; k++)
    {
        for(double j = 0; j < dx*dy; j++)
        {
            minZ_defaultTvalue[0].addCellIndex(minZstartFace+dx*dy*k+j);
        }
    }

    //this is how to fill the first set of points, the cells from before refine mesh
    for(double k = 0; k < 1; k++)
    {
        for(double j = 0; j < dy; j++)
        {
            for(double i = 0; i < dx; i++)
            {
                minZ_Tvalues[i*dx_refined].addCellIndex(minZstartFace+dx*dy*k+dy*i+j);
            }
        }
    }

    //fill in the first set of split cells from refine mesh
    //fills rest of the outer corners of the box first
    for(double j = 0; j < dy; j++)
    {
        for(double i = 0; i < dx; i++)
        {
            minZ_Tvalues[i*dx_refined].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3); //I bet this 3 is some kind of dy_refined -1.
            minZ_Tvalues[i*dx_refined+dx_refined-1].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+1);
            minZ_Tvalues[i*dx_refined+dx_refined-1].addCellIndex(minZstartFace+dx*dy*1+(dy*i+j)*3+2);
        }
    }

    if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
    {
        //fill in the second part of split cells from refine mesh
        //seems to fill in from the outside
        for(double j = 0; j < dy; j++)
        {
            for(double i = 0; i < dx; i++)
            {
                minZ_Tvalues[i*dx_refined].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3);
                minZ_Tvalues[i*dx_refined+1].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+1);
                minZ_Tvalues[i*dx_refined+1].addCellIndex(minZstartFace+dx*dy*4+(dy*i+j)*3+2);
            }
        }
        //fills in the third and final bunch of split cells from refine mesh
        //seems to only fill the diagonals, then let the second set fill the rest of the diagonal
        for(double j = 0; j < dy; j++)
        {
            for(double i = 0; i < dx; i++)
            {
                minZ_Tvalues[i*dx_refined+1].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9);
                minZ_Tvalues[i*dx_refined+1].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+1);
                minZ_Tvalues[i*dx_refined].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+2);
                minZ_Tvalues[i*dx_refined+dx_refined-1].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+3);
                minZ_Tvalues[i*dx_refined+dx_refined-2].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+4);
                minZ_Tvalues[i*dx_refined+dx_refined-2].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+5);
                minZ_Tvalues[i*dx_refined+dx_refined-2].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+6);
                minZ_Tvalues[i*dx_refined+dx_refined-2].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+7);
                minZ_Tvalues[i*dx_refined+dx_refined-1].addCellIndex(minZstartFace+dx*dy*7+(dy*i+j)*9+8);
            }
        }
    }
}

void generateLinesToAddRefinedMesh::generateExampleNorthFace()
{
//This section is for filling in north_face with methods that aren't as structured. These are more easily
//manipulated to look at how each individual point is filled in.
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateExampleNorthFace if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(northFace_Tvalues.size() != 1)
    {
        std::cout << "Error, cannot perform function generateExampleNorthFace if " <<
                     "the Tprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    //fill in the original cells first
    for(double i = 0; i < dx*dz; i++)
    {
        northFace_defaultTvalue[0].addCellIndex(northFaceStartFace+i);
    }
    //now fill in the refined cells. Notice that 2^0 = 1, so this becomes dx*0
    //and the loop isn't entered if timesRefined = 0.
    for(double i = 0; i < dx*(dx_refined*dz_refined-1); i++)
    {
        northFace_defaultTvalue[0].addCellIndex(northFaceStartFace+dx*dz+i);
    }

    //fill the first set of points, the cells from before refine mesh
    for(double i = 0; i < dx*dz; i++)
    {
        northFace_Tvalues[0].addCellIndex(northFaceStartFace+i);
    }

    if(timesRefined != 0)   //only needed to fill out the above points if timesRefined == 0
    {
        //fill in the first set of split cells from refine mesh
        //fills rest of the outer corners of the box first
        //starts at top right, then top left, then bot left
        for(double i = 0; i < 3*dx; i++)
        {
            northFace_Tvalues[0].addCellIndex(northFaceStartFace+dx*dz+i);
        }

        if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
        {
            //fill in the second part of split cells from refine mesh
            //fills bot right corner
            //starts by going out in the z direction from the point, then goes out in the x direction
            //from the point, then fills in the cell between these two just filled cells
            for(double i = 0; i < 3*dx; i++)
            {
                northFace_Tvalues[0].addCellIndex(northFaceStartFace+dx*dz+dx*3+i);
            }

            //fill in the third part of split cells from refine mesh
            //fills rest of the corners
            //starts at top right, then top left, then bot left
            for(double i = 0; i < 9*dx; i++)
            {
                northFace_Tvalues[0].addCellIndex(northFaceStartFace+dx*dz+dx*6+i);
            }
        }
    }
}

void generateLinesToAddRefinedMesh::generateAdjustableNorthFace_simplest()
{
//This section is for filling in north_face from bottom to top with methods that are structured.
//This assumes that all the refined cells have the same value as the bottom cell from before refine mesh
//This means that the Tprofile has to be of size dz to work, regardless of the value of timesRefined
//If you want a single value for the north_face, use generateExampleNorthFace
    if(timesRefined == 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableNorthFace_simplest if " <<
                     "timesRefined == 0! Use generateExampleNorthFace instead!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableNorthFace_simplest if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(northFace_Tvalues.size() != dz)
    {
        std::cout << "Error, cannot perform function generateAdjustableNorthFace_simplest if " <<
                     "the Tprofile doesn't have the same size as the dz of the mesh!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    //fill in the original cells first
    for(double i = 0; i < dx*dz; i++)
    {
        northFace_defaultTvalue[0].addCellIndex(northFaceStartFace+i);
    }
    //now fill in the refined cells
    for(double i = 0; i < dx*(dx_refined*dz_refined-1); i++)
    {
        northFace_defaultTvalue[0].addCellIndex(northFaceStartFace+dx*dz+i);
    }

    //fill the first set of points, the cells from before refine mesh
    for(double i = 0; i < dx; i++)
    {
        for(double t = 0; t < northFace_Tvalues.size(); t++)
        {
            northFace_Tvalues[t].addCellIndex(northFaceStartFace+dz*i+t);
        }
    }

    //fill in the first set of split cells from refine mesh
    //fills rest of the outer corners of the box first
    //starts at top right, then top left, then bot left
    for(double i = 0; i < dx; i++)
    {//fill in the original cells first
        for(double r = 0; r < 3; r++)
        {
            northFace_Tvalues[0].addCellIndex(northFaceStartFace+dx*dz+3*i+r);
        }
    }

    if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
    {
        //fill in the second part of split cells from refine mesh
        //fills bot right corner
        //starts by going out in the z direction from the point, then goes out in the x direction
        //from the point, then fills in the cell between these two just filled cells
        for(double i = 0; i < dx; i++)
        {
            for(double r = 0; r < 3; r++)
            {
                northFace_Tvalues[0].addCellIndex(northFaceStartFace+dx*dz+dx*3+3*i+r);
            }
        }

        //fill in the third part of split cells from refine mesh
        //fills rest of the corners
        //starts at top right, then top left, then bot left
        for(double i = 0; i < dx; i++)
        {
            for(double r = 0; r < 3; r++)
            {
                for(double p = 0; p < 3; p++)
                {
                    northFace_Tvalues[0].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i+3*r+p);
                }
            }
        }
    }
}

void generateLinesToAddRefinedMesh::generateAdjustableNorthFace_refineCells()
{
//This section is for filling in north_face from bottom to top with methods that are structured.
//This also fills in the refined cell layers separately from the other layers
//The Tprofile has to be of the same size as the number of cells in the z direction
//This includes the cells from before refine mesh as well as the cells after refine mesh
//If you want a single value for the north_face, use generateExampleNorthFace
    if(timesRefined == 0)
    {
        std::cout << "Error, cannot perform function generateAdjustableNorthFace_refineCells if " <<
                     "timesRefined == 0! Use generateExampleNorthFace instead!\n";
        std::exit(EXIT_FAILURE);
    }
    if(minZ_defaultTvalue.size() != 1)
    {
        std::cout << "Error, cannot perform function generateAdjustableNorthFace_refineCells if " <<
                     "the defaultTprofile doesn't have size 1!\n";
        std::exit(EXIT_FAILURE);
    }
    if(northFace_Tvalues.size() != dz+dz_refined-1)
    {
        std::cout << "Error, cannot perform function generateAdjustableNorthFace_refineCells if " <<
                     "the Tprofile doesn't have the same size as dz+dz_refined-1!\n";
        std::exit(EXIT_FAILURE);
    }

    //this resets all the values before doing what really needs to be filled
    //fill in the original cells first
    for(double i = 0; i < dx*dz; i++)
    {
        northFace_defaultTvalue[0].addCellIndex(northFaceStartFace+i);
    }
    //now fill in the refined cells
    for(double i = 0; i < dx*(dx_refined*dz_refined-1); i++)
    {
        northFace_defaultTvalue[0].addCellIndex(northFaceStartFace+dx*dz+i);
    }

    for(double i = 0; i < dx; i++)
    {
        for(double t = 0; t < dz; t++)
        {
            if(t == 0)
            {
                northFace_Tvalues[t].addCellIndex(northFaceStartFace+dz*i+t);
            } else
            {
                northFace_Tvalues[t+dz_refined-1].addCellIndex(northFaceStartFace+dz*i+t);
            }
        }
    }

    //fill in the first set of split cells from refine mesh
    //fills rest of the outer corners of the box first
    //starts at top right, then top left, then bot left
    for(double i = 0; i < dx; i++)
    {
        northFace_Tvalues[dz_refined-1].addCellIndex(northFaceStartFace+dx*dz+3*i);   //top right face
        northFace_Tvalues[dz_refined-1].addCellIndex(northFaceStartFace+dx*dz+3*i+1);   //top left face
        northFace_Tvalues[0].addCellIndex(northFaceStartFace+dx*dz+3*i+2);              //bottom left face
    }

    if(timesRefined == 2)   //would do != 1, but this might need changed for timesRefined > 2.
    {
        //fill in the second part of split cells from refine mesh
        //fills bot right corner
        //starts by going out in the z direction from the point, then goes out in the x direction
        //from the point, then fills in the cell between these two just filled cells
        for(double i = 0; i < dx; i++)
        {
            northFace_Tvalues[timesRefined-1].addCellIndex(northFaceStartFace+dx*dz+dx*3+3*i);
            northFace_Tvalues[timesRefined-1].addCellIndex(northFaceStartFace+dx*dz+dx*3+3*i+1);
            northFace_Tvalues[timesRefined-2].addCellIndex(northFaceStartFace+dx*dz+dx*3+3*i+2);
        }

        //fill in the third part of split cells from refine mesh
        //fills rest of the corners
        //starts at top right, then top left, then bot left
        for(double i = 0; i < dx; i++)
        {
            northFace_Tvalues[dz_refined-1].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i);
            northFace_Tvalues[dz_refined-2].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i+1);
            northFace_Tvalues[dz_refined-2].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i+2);
            northFace_Tvalues[dz_refined-2].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i+3);
            northFace_Tvalues[dz_refined-2].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i+4);
            northFace_Tvalues[dz_refined-1].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i+5);
            northFace_Tvalues[timesRefined-2].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i+6);
            northFace_Tvalues[timesRefined-1].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i+7);
            northFace_Tvalues[timesRefined-1].addCellIndex(northFaceStartFace+dx*dz+dx*6+3*3*i+8);
        }
    }
}
