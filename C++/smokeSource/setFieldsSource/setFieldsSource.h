#ifndef SETFIELDSSOURCE_H
#define SETFIELDSSOURCE_H

#include <iostream>
#include <vector>
#include <string>

class setFieldsSource
{

public:

    /*
     * This sets up available smoke source models, with functions that output necessary
     * information for setting them up using OpenFOAM's setFieldsDict.
     *
     * Could possibly rename this to foamSources at some time, depending on how this changes over time
     */

    setFieldsSource();

    // returns source values for the desired sourceModel
    std::vector< std::vector<std::string> > getSource(std::string sourceProperty,std::vector<std::string> desiredDistribution,std::vector< std::vector<std::string> > modelInputs);

private:

    void setupAvailableOptions();
    void setupModelTypes();
    bool checkAvailableOptions();

    std::vector<std::string> getSingleModelValues(std::string distributionType,std::vector<std::string> modelInputs);

    bool checkPropertyType(std::string propertyType);
    bool checkDesiredDistributionTypes(std::vector<std::string> desiredDistribution);

    std::vector<std::string> distributionModel_boxToCell(std::vector<std::string> modelInputs);


    std::vector<std::string> propertyTypes;
    std::vector<std::string> distributionTypes;

};

#endif // SETFIELDSSOURCE_H
