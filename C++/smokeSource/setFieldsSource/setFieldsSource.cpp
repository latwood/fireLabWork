#include "setFieldsSource.h"

setFieldsSource::setFieldsSource()
{
    setupAvailableOptions();
    if(checkAvailableOptions() == false)
    {
        std::cout << "Error setting up available setFieldsSource Options!\n";
    };
}

void setFieldsSource::setupAvailableOptions()
{
    // probably should have a class of source options, or just lots of control here
    propertyTypes.push_back("source");
}

void setFieldsSource::setupModelTypes()
{
    // so this allows declaring strings for the model types, but there isn't really an easy way
    // to check to make sure those strings actually correspond to a function that is the model
    // just make sure that when you add a sourceDistributionType here, basically a model type,
    // that you create the necessary function to go with it, or something will break
    distributionTypes.push_back("boxToCell");
}

bool setFieldsSource::checkAvailableOptions()
{
    // need to look at time directories to see if this type exists?
    // or better yet this is making sure that vector counts match up between the types correctly
    // for now, just let it run and assume options are correct

    // also would be nice to be able to check the models ahead of time, but not sure I really can
    return true;
}

std::vector< std::vector<std::string> > setFieldsSource::getSource(std::string sourceProperty,std::vector<std::string> desiredDistribution,std::vector< std::vector<std::string> > modelInputs)
{
    std::vector< std::vector<std::string> > sourceValues;
    if(checkPropertyType(sourceProperty) == false)
    {
        std::cout << "Error in function call setFieldsSource::getSource!\n";
        std::cout << "sourceProperty " << sourceProperty << " is not an allowable type!\n";
        // need exit statement here, then can break up if else statements and just use separate ifs
    } else if(desiredDistribution.size() != modelInputs.size())
    {
        std::cout << "Error in function call setFieldsSource::getSource!\n";
        std::cout << "Number of desiredDistributions does not match number of separated modelInputs!\n";
    } else if(checkDesiredDistributionTypes(desiredDistribution) == false)
    {
        std::cout << "Error in function call setFieldsSource::getSource!\n";
        std::cout << "desiredDistributionTypes contains at least one not allowable type!\n";
        std::cout << "Available distributionTypes are: ";
        for(size_t j = 0; j < distributionTypes.size()-1; j++)
        {
            std::cout << distributionTypes[j] << ", ";
        }
        std::cout << distributionTypes[distributionTypes.size()-1] << "\n";
        // need exit statement here, then can break up if else statements and just use separate ifs
    } else
    {
        for(size_t j = 0; j < desiredDistribution.size(); j ++)
        {
            std::vector<std::string> singleModelValues;
            std::cout << "setFieldsSource::getSource Attempting to run model for the " << desiredDistribution[j] << " distribution\n";
            singleModelValues = getSingleModelValues(desiredDistribution[j],modelInputs[j]);
            sourceValues.push_back(singleModelValues);
        }
    }
    return sourceValues;
}

std::vector<std::string> setFieldsSource::getSingleModelValues(std::string distributionType,std::vector<std::string> modelInputs)
{
    std::vector<std::string> singleSourceValues;
    if(distributionType == "boxToCell")
    {
        singleSourceValues = distributionModel_boxToCell(modelInputs);
    } else if(distributionType == "model2")
    {
        //singleSourceValues = distributionModel2;
    }

    return singleSourceValues;
}

bool setFieldsSource::checkPropertyType(std::string propertyType)
{
    bool testResult = false;
    // check if this exists first
    for(size_t i = 0; i < propertyTypes.size(); i++)
    {
        if(propertyTypes[i] == propertyType)
        {
            testResult = true;
        }
    }
    return testResult;
}

bool setFieldsSource::checkDesiredDistributionTypes(std::vector<std::string> desiredDistribution)
{
    bool testResult = true;
    // make sure each one exists
    for(size_t j = 0; j < desiredDistribution.size(); j++)
    {
        bool typeExists = false;
        for(size_t i = 0; i < distributionTypes.size(); i++)
        {
            if(desiredDistribution[j] == distributionTypes[i])
            {
                typeExists = true;
                break;
            }
        }
        // if it is still false, it didn't find it in the available distribution types
        if(typeExists == false)
        {
            testResult = false;
        }
    }
    return testResult;
}


std::vector<std::string> setFieldsSource::distributionModel_boxToCell(std::vector<std::string> modelInputs)
{
    std::vector<std::string> sourceValues;
    if(modelInputs.size() != 5)
    {
        std::cout << "Error in setFieldsSource::distributionModel_boxToCell!\n";
        std::cout << "5 modelInputs are required and only " << modelInputs.size() << " are given!\n";
    }

    // hmm, I've been doing this to output values, but technically I need to output coordinates and values depending on an input value
    // This is super complicated simply because I don't know exactly how much or what output I need, and I almost have to receive input that is basically the output
    // so I think that this script is done for now. For something like boxToCell, this isn't useful at all. Just bypass something like this.
    // for other stuff like circularSource, could use boxToCell and other stuff, but it would be easier to write the setFieldsDict file than to output values
    // but the ideas here are useful

    return sourceValues;
}

