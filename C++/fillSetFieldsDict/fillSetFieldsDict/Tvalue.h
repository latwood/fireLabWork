#ifndef TVALUE_H
#define TVALUE_H

#include <iostream>
#include <vector>

class Tvalue
{
public:

    /*
     * create class Tvalue where Tvalue has a name (the T value)
     * and a vector of all the cell indices with that value
     * create vectors of Tvalues like topToBot_Tvalues and leftToRight_Tvalues
     * (this idea of vectors eventually became Tprofiles)
     *
     * The idea is that you fill the Tvalues with indices first using
     * all the loops and stuff that deal with the strange patterns of the cells in the mesh
     * Then you fill the linesToAdd using the Tvalues and their indices.
     *
     * The right container for all the work is a separate vector of Tvalues for each patch
     */

    Tvalue();
    Tvalue(std::string Tvalue_value);

    void addTvalue(std::string Tvalue_value);
    void addCellIndex(double cellIndexValue);

    std::string getValue();
    std::vector<double> getCellIndices();

private:

    std::string value;
    std::vector<double> cellIndices;

};

#endif // TVALUE_H
