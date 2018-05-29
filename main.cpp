#include <iostream>
#include <fstream>
#include <string>
#include <w32api/libloaderapi.h>
#include <cstdio>  /* defines FILENAME_MAX */
#include <fstream>
#include "main.h"
#include "iloc.h"

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#include <cerrno>
#define GetCurrentDir getcwd
#endif

int main() {
    std::string filename;
    std::ofstream fileC;

    std::cout << "which file do you want to open? ";
    std::cin >> filename;

    filename = strcat(getDirectory(), "/") + filename;

    std::cout << "directory: ";
    std::cout << filename << std::endl;

    std::ifstream file(filename);
    std::string sent;
    while (std::getline(file, sent))
    {
        std::cout << sent << std::endl;
        exec(sent);
    }
    return 0;
}

char* SplitFilename(const std::string &str) {
//    std::cout << "Splitting: " << str << '\n';
    std::size_t found = str.find_last_of('/');
//    std::cout << " path: " << str.substr(0, found) << '\n';
//    std::cout << " file: " << str.substr(found + 1) << '\n';
    return const_cast<char *>(str.substr(0, found).c_str());
}

char *getDirectory() {
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
        printf("error: %d", errno);
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

    return SplitFilename(cCurrentPath);
}

