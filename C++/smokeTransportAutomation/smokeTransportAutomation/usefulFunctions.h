#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H

#include <iostream>
#include <sys/stat.h>
#include <istream>
#include <fstream>
#include <sstream>

class usefulFunctions
{
public:

    usefulFunctions();

    // make sure file exists before opening it
    bool fileExists(const std::string& name);

    // message functions
    void message(std::string theMessage);
    void exitMessage(std::string theMessage);
    void debugMessage(std::string theMessage);
    // the idea here is that the debug message is split up, so always follow this type with a regular debugMessage
    void splitDebugMessage(std::string theMessage);

    // check data type functions
    // do we even need all these types? Need to see what is actually being used by the program
    bool is_size_t(std::string s);
    bool is_int(std::string s);
    bool is_long(std::string s);
    bool is_float(std::string s);
    bool is_double(std::string s);
    bool is_bool(std::string s);

    // string and value conversion functions
    int stringToInt(std::string s);
    std::string intToString(int n);

private:

    bool debug;  // This is in addition to #define YESDEBUG

};

#endif // USEFULFUNCTIONS_H
