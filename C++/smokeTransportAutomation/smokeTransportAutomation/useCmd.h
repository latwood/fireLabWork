#ifndef USECMD_H
#define USECMD_H

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

// I found this code online to replace my system calls
// I made some changes so that it outputs, and while I think it is safer than system calls,
// it might still not be the right way to do this in a safe and efficient manner
// but it does seem to work for now
// see this article for why system commands are bad: http://www.cplusplus.com/forum/articles/11153/
// I found this function here: https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
inline std::string exec_cmd(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
                std::cout << buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

#endif // USECMD_H
