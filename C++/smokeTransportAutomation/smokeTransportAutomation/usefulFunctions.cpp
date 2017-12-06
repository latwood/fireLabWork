#include "usefulFunctions.h"

usefulFunctions::usefulFunctions()
{
    excessDebug = true;  // in addition to #define YESDEBUG
}

// make sure file exists before opening it
bool usefulFunctions::fileExists(const std::string& name)
{
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

// message functions
void usefulFunctions::message(std::string theMessage)
{
    std::cout << theMessage << "\n";
    system("sleep 0.25");
}

void usefulFunctions::exitMessage(std::string theMessage)
{
    std::cout << theMessage << "\nEnding program now!\n";
    exit(EXIT_FAILURE);
}

void usefulFunctions::debugMessage(std::string theMessage)
{
#ifdef YESDEBUG
    std::cout << theMessage << "\n";
    system("sleep 0.25");
#else
    UNUSED(theMessage);
#endif
}

// the idea here is that the debug message is split up, so always follow this type with a regular debugMessage
void usefulFunctions::splitDebugMessage(std::string theMessage)
{
    std::cout << "ugh";
//#ifdef YESDEBUG
    std::cout << theMessage;
//#else
    UNUSED(theMessage);
//#endif
}

void usefulFunctions::excessDebugMessage(std::string theMessage)
{
    if(excessDebug == true)
    {
        std::cout << theMessage << "\n";
    }
}


// check data type functions. Notice that you can't use the stringstream again
// do we even need all these types? Need to see what is actually being used by the program
bool usefulFunctions::is_size_t(std::string s)
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

bool usefulFunctions::is_int(std::string s)
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

bool usefulFunctions::is_long(std::string s)
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

bool usefulFunctions::is_float(std::string s)
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

bool usefulFunctions::is_double(std::string s)
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

bool usefulFunctions::is_bool(std::string s)
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
int usefulFunctions::stringToInt(std::string s)
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

std::string usefulFunctions::intToString(int n)
{
    std::ostringstream strm; // this is supposed to use o instead of i stringstream since creating a string
    if((strm << n).fail())
    {
        strm.clear();
        exitMessage("conversion of int to string failed!");
    }
    return strm.str();
}
