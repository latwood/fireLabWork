#include "Tprofile.h"

Tprofile::Tprofile()
{

}

size_t Tprofile::size()
{
    return theTprofile.size();
}

Tvalue& Tprofile::operator[] (const double index)
{
    return theTprofile[index];
}

void Tprofile::cleanTprofile()
{
    theTprofile.clear();    //always call this before generating new stuff in a Tprofile
}

void Tprofile::generateTprofile(std::string singleValue)
{
    cleanTprofile();
    theTprofile.push_back(Tvalue(singleValue));
}

void Tprofile::generateLinearTprofile(std::vector<std::string> givenValues)
{
    cleanTprofile();
    for(size_t k = 0; k < givenValues.size(); k++)
    {
        theTprofile.push_back(givenValues[k]);
    }
}

void Tprofile::generateLinearTprofile(double lowTvalue,double highTvalue,double cellsInProfile)
{
    cleanTprofile();
    double Tslope = (highTvalue-lowTvalue)/(cellsInProfile-1);   //note the cell index is 0, so number of cells in profile is one too big
    for (double k = 0; k < cellsInProfile; k++)
    {
        double theTvalue = Tslope*k+lowTvalue;
        std::ostringstream strm;
        strm << theTvalue;
        std::string str = strm.str();
        theTprofile.push_back(Tvalue(str));
    }
}
