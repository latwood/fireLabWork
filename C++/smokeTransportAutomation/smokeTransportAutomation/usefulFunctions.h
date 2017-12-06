#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H

#include <iostream>
#include <sys/stat.h>
#include <istream>
#include <fstream>
#include <sstream>

#define YESDEBUG   // comment this out if you don't want debug messages
// use this function to suppress unused variable messages, by replacing x with the unused variable
#define UNUSED(x) (void)(x)

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
    void excessDebugMessage(std::string theMessage);    // some things are normal debug, this is for extra debug for more annoying problems in code

    // check data type functions. Notice that you can't use the stringstream again
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

    bool excessDebug;  // This is in addition to #define YESDEBUG

};

#endif // USEFULFUNCTIONS_H
