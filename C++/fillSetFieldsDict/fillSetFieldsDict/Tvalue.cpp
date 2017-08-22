#include "Tvalue.h"

Tvalue::Tvalue(std::string Tvalue_value)
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
