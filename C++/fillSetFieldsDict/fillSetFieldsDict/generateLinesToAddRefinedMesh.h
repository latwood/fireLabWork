#ifndef GENERATELINESTOADDREFINEDMESH_H
#define GENERATELINESTOADDREFINEDMESH_H

#include <vector>
#include <sstream>
#include <cmath>
#include <cstdlib>

#include "Tvalue.h"
#include "Tprofile.h"

class generateLinesToAddRefinedMesh
{
public:

    generateLinesToAddRefinedMesh(double minZstartFace_value,double northFaceStartFace_value,
                                  double westFaceStartFace_value,double eastFaceStartFace_value,
                                  double southFaceStartFace_value,double maxZstartFace_value,
                                  double dx_value, double dy_value, double dz_value, double timesRefined_value);

    std::vector<std::string> getLines();

private:

    void fillLinesToAdd(Tprofile defaultValue,Tprofile theValues,std::string fillType);

    void generateExampleMinZ();
    void generateAdjustableMinZ_topToBot();
    void generateAdjustableMinZ_simplest_topToBot();
    void generateAdjustableMinZ_refineCells_topToBot();
    void generateAdjustableMinZ_leftToRight();
    void generateAdjustableMinZ_simplest_leftToRight();
    void generateAdjustableMinZ_refineCells_leftToRight();
    void generateExampleNorthFace();
    void generateAdjustableNorthFace_simplest();
    void generateAdjustableNorthFace_refineCells();
    void generateExampleWestFace();
    void generateAdjustableWestFace_simplest();
    void generateAdjustableWestFace_refineCells();
    void generateExampleEastFace();
    void generateAdjustableEastFace_simplest();
    void generateAdjustableEastFace_refineCells();
    void generateExampleSouthFace();
    void generateAdjustableSouthFace_simplest();
    void generateAdjustableSouthFace_refineCells();
    void generateExampleMaxZ();
    void generateAdjustableMaxZ_topToBot();
    void generateAdjustableMaxZ_simplest_topToBot();
    void generateAdjustableMaxZ_refineCells_topToBot();
    void generateAdjustableMaxZ_leftToRight();
    void generateAdjustableMaxZ_simplest_leftToRight();
    void generateAdjustableMaxZ_refineCells_leftToRight();

    /*
     * maybe someday the problem is that adjusting one set of values means a need to adjust another set
     * of values. So set the minZ but setting north_face means a change in minZ and in other faces
     * so maybe add a function to adjust certain Tprofiles to account for these changes,
     * or to simply overwrite the values set by minZ in a separate labelsToFace for these changes
     *
     * Or just simply plan ahead when setting up the Tprofiles
     */

    std::vector<std::string> linesToAdd;
    double minZstartFace;
    double northFaceStartFace;
    double westFaceStartFace;
    double eastFaceStartFace;
    double southFaceStartFace;
    double maxZstartFace;
    double dx;  //number of cells in the x direction before refine mesh
    double dy;  //number of cells in the y direction before refine mesh
    double dz;  //number of cells in the z direction before refine mesh
    double timesRefined;    //number of times refined
    double dx_refined;
    double dy_refined;
    double dz_refined;

    /*
     * Hm, so right now the error checking is to make sure the profiles have certain aspects so the stuff doesn't break
     * What if instead of doing different checks on the profile size for each function, the only check were that
     * the profile has the same number of values as there are cells or faces in a given patch of the mesh?
     * I would have to adapt the profile generators to give a vector of profiles the same size as the mesh.
     *
     * Hm, the trouble with this is that maybe I would transfer the error checking to not check the size,
     * but now need to check the profile value locations. Or do we care what the profile looks like as long
     * as it puts values in a certain way into the mesh? Then there would be a function for filling from
     * top to bot or left to right and an example, and that would be it. The user would then be in charge of
     * making sure they used the right setup of temperature profile, as long as it is the right size, it will
     * fill out the mesh in a certain order every time.
     *
     * Hopefully this makes it easier to change from just using T profiles to profiles. This project is
     * getting huge/too long and complex very quickly
     * So let's finish this one to a certain point, say till the both generateLinesToAdd have the full functions
     * for affecting each part of the mesh. Then we save and archive it, and do a separate project
     * that goes from scratch, using all this stuff, but switching to only filling in by direction and using
     * profiles of the size of the mesh. Then it can use multiple profile types and it's basically doing the same
     * thing, but simplifying it down so it is easier to read and modify
     * The new program can also use a write file function instead of a read and replace function
     */
    Tprofile minZ_defaultTvalue;
    Tprofile minZ_Tvalues;
    Tprofile northFace_defaultTvalue;
    Tprofile northFace_Tvalues;
    Tprofile westFace_defaultTvalue;
    Tprofile westFace_Tvalues;
    Tprofile eastFace_defaultTvalue;
    Tprofile eastFace_Tvalues;
    Tprofile southFace_defaultTvalue;
    Tprofile southFace_Tvalues;
    Tprofile maxZ_defaultTvalue;
    Tprofile maxZ_Tvalues;
    Tprofile internalField_defaultTvalue;
    Tprofile internalField_Tvalues;

};

#endif // GENERATELINESTOADDREFINEDMESH_H
