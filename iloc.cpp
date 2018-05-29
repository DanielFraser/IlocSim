#include "iloc.h"

char *memory = new char[10240];

void exec(std::string code) {

    auto action = code.substr(0, code.find(' ')); //what tokens should we expected next
    std::cout << std::endl;
    if (action.substr(0, 3) == "add") {
        add(code);
    } else if (action.substr(0, 3) == "sub") {
        sub(code);
    }
}

void add(std::string line) {
    std::regex cons("(r\\d+)\\s*,\\s*(\\d+)\\s*=>\\s*(r\\d+)");
    std::smatch matches;
    regex_search(line, matches, cons);
    if (matches.empty()) {
        std::regex regs("(r\\d+)\\s*,\\s*(r\\d+)\\s*=>\\s*(r\\d+)");
        regex_search(line, matches, cons);
        std::cout << "hello" << std::endl;
    }
    if (!matches.empty()) {
        for (int i = 1; i < matches.size(); i++)
            std::cout << matches[i] << " ";
        std::cout << std::endl;
    }
}

void sub(std::string line) {

}