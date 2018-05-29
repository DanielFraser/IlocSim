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

int main(int argc, char **argv) {
    std::string filename = argv[1];
    std::ofstream fileC;

    filename = strcat(getDirectory(), "/") + filename;

    std::cout << "directory: ";
    std::cout << filename << std::endl;

    std::ifstream file(filename);
    std::string sent;
    init();
    while (std::getline(file, sent)) {
        std::cout << sent << std::endl;
        exec(sent);
    }
    file.close();
    return 0;
}

char *SplitFilename(const std::string &str) {
    std::size_t found = str.find_last_of('/');
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