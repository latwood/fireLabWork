#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H

#define YESDEBUG   // comment this out if you don't want debug messages

#include <iostream>
#include <sys/stat.h>
#include <istream>
#include <fstream>
#include <sstream>

// use this function to suppress unused variable messages, by replacing x with the unused variable
#define UNUSED(x) (void)(x)

// make sure file exists before opening it
inline bool fileExists(const std::string& name)
{
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

// message functions
inline void message(std::string theMessage)
{
    std::cout << theMessage << "\n";
    system("sleep 0.25");
}

inline void exitMessage(std::string theMessage)
{
    std::cout << theMessage << "\nEnding program now!\n";
    exit(EXIT_FAILURE);
}

inline void debugMessage(std::string theMessage)
{
#ifdef YESDEBUG
    std::cout << theMessage << "\n";
    system("sleep 0.25");
#else
    UNUSED(theMessage);
#endif
}

// the idea here is that the debug message is split up, so always follow this type with a regular debugMessage
inline void splitDebugMessage(std::string theMessage)
{
#ifdef YESDEBUG
    std::cout << theMessage;
#else
    UNUSED(theMessage);
#endif
}


// check data type functions. Notice that you can't use the stringstream again
// do we even need all these types? Need to see what is actually being used by the program
inline bool is_size_t(std::string s)
{
    bool isType = true;
    std::istringstream strm;
    strm.str(s);
    size_t n = 0;
    if((strm >> n).fail())
    {
        strm.clear();
        n = 0;
        isType = false;
    }
    return isType;
}

inline bool is_int(std::string s)
{
    bool isType = true;
    std::istringstream strm;
    strm.str(s);
    int n = 0;
    if((strm >> n).fail())
    {
        strm.clear();
        isType = false;
    }
    return isType;
}

inline bool is_long(std::string s)
{
    bool isType = true;
    std::istringstream strm;
    strm.str(s);
    long n = 0;
    if((strm >> n).fail())
    {
        strm.clear();
        isType = false;
    }
    return isType;
}

inline bool is_float(std::string s)
{
    bool isType = true;
    std::istringstream strm;
    strm.str(s);
    float n = 0;
    if((strm >> n).fail())
    {
        strm.clear();
        isType = false;
    }
    return isType;
}

inline bool is_double(std::string s)
{
    bool isType = true;
    std::istringstream strm;
    strm.str(s);
    double n = 0;
    if((strm >> n).fail())
    {
        strm.clear();
        isType = false;
    }
    return isType;
}

inline bool is_bool(std::string s)
{
    bool isType = true;
    std::istringstream strm;
    strm.str(s);
    bool n = 0;
    if((strm >> n).fail())
    {
        strm.clear();
        isType = false;
    }
    return isType;
}

// string and value conversion functions
inline int stringToInt(std::string s)
{
    std::istringstream strm;
    strm.str(s);
    int n = 0;
    if((strm >> n).fail())
    {
        strm.clear();
        exitMessage("conversion of string to int failed!");
    }
    return n;
}

inline std::string intToString(int n)
{
    std::ostringstream strm; // this is supposed to use o instead of i stringstream since creating a string
    if((strm << n).fail())
    {
        strm.clear();
        exitMessage("conversion of int to string failed!");
    }
    return strm.str();
}


#endif // USEFULFUNCTIONS_H
