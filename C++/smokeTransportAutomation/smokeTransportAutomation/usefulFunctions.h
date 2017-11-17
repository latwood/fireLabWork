#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H

#include <iostream>
#include <sys/stat.h>

#define UNUSED(x) (void)(x) // use this function to suppress unused variable messages, by replacing x with the unused variable

inline bool fileExists(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

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

inline void splitDebugMessage(std::string theMessage)
{
#ifdef YESDEBUG
    std::cout << theMessage;
#else
    UNUSED(theMessage);
#endif
}

#endif // USEFULFUNCTIONS_H
