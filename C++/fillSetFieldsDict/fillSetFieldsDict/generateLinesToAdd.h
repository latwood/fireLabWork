#ifndef GENERATELINESTOADD_H
#define GENERATELINESTOADD_H

#include <vector>
#include <sstream>
#include <cmath>
#include <cstdlib>

#include "Tvalue.h"
#include "Tprofile.h"

class generateLinesToAdd
{
public:

    generateLinesToAdd(double minZstartFace_value,double northFaceStartFace_value,double westFaceStartFace_value,
                       double eastFaceStartFace_value,double southFaceStartFace_value,double maxZstartFace_value,
                       double dx_value, double dy_value, double dz_value);

    std::vector<std::string> getLines();

private:

    void fillLinesToAdd(Tprofile defaultValue,Tprofile theValues);

    void generateExampleMinZ();
    void generateAdjustableMinZ_topToBot();
    void generateAdjustableMinZ_leftToRight();
    void generateExampleNorthFace();
    void generateAdjustableNorthFace();
    void generateExampleWestFace();
    void generateAdjustableWestFace();
    void generateExampleEastFace();
    void generateAdjustableEastFace();
    void generateExampleSouthFace();
    void generateAdjustableSouthFace();

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

    /*
     * could change all the default values from Tvalues to Tprofiles with a single Tvalue,
     * but it currently works and the same amount of error checking would probably be necessary
     * the error checking would just get a different form.
     *
     * Could add in error handling to make sure that they don't use more Tvalues in a Tprofile
     * than there are locations in the face, but I think that it will be all right since it will just
     * truncate whatever stuff is used. It will just use as many values in the Tprofile as there are locations
     * to use them, and ignore the rest
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

#endif // GENERATELINESTOADD_H
