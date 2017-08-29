#ifndef READFRAMESFILE_H
#define READFRAMESFILE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <string>
#include <fstream>
#include <istream>

class readFramesFile
{

public:

    /*
     * This class is for reading in the frames file, making sure all the inputs are of the right format.
     *
     * Eventually need to figure out how to use pointers with vectors so that I'm not storing two vectors
     * for a single data type as I do here with frames and the same with other types in all other classes.
     * Basically I have one in this class, but then have a copy of it in the outer class, so two copies.
     */

    readFramesFile();

    void readFile(std::string framesFile);
    std::vector<std::string> getFrames();

private:

    std::vector<std::string> frames;
    bool checkFrame(std::string frame);
    bool is_numeric(char const *string);

};

#endif // READFRAMESFILE_H
