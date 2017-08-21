#include "Tvalue.h"

Tvalue::Tvalue()
{
    /*
     * the idea is to use the addTvalue. This is ideal for setting a single Tvalue instead of a vector of Tvalues
     *
     * Only useful if setting a single Tvalue, which the vector of Tvalues is better.
     * So eventually need to delete this constructor and disallow it
     */
}

Tvalue::Tvalue(std::string Tvalue_value)
{
    value = Tvalue_value;
}

void Tvalue::addTvalue(std::string Tvalue_value)
{
    value = Tvalue_value;
}

void Tvalue::addCellIndex(double cellIndexValue)
{
    cellIndices.push_back(cellIndexValue);
}

std::string Tvalue::getValue()
{
    return value;
}

std::vector<double> Tvalue::getCellIndices()
{
    return cellIndices;
}
