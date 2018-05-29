#include <unordered_map>
#include "iloc.h"

int *memory = new int[10240];
std::map<int, int> registers;

void exec(std::string code) {

    auto action = code.substr(0, code.find(' ')); //what tokens should we expected next
    std::cout << std::endl;
    if (action.substr(0, 3) == "add" || action.substr(0, 3) == "sub" || action.substr(0, 3) == "mul")
        math(code, action[0]);
    else if (action.substr(0, 4) == "load")
        load(code);
    else if (action.substr(0, 4) == "stor")
        store(code);
}

/**
 * deals with the addition/subtraction/multiplication code
 *
 * @param line the line of code
 */
void math(std::string line, char op) {
    std::regex cons(R"((r\d+)\s*,\s*(\d+)\s*=>\s*(r\d+))");
    std::smatch matches;
    regex_search(line, matches, cons);
    if (matches.empty()) {
        std::regex regs(R"((r\d+)\s*,\s*(r\d+)\s*=>\s*(r\d+))");
        regex_search(line, matches, regs);
    }
    if (!matches.empty()) {
        registers[std::atoi(matches.str(3).substr(1).c_str())] = registers[std::atoi(matches.str(1).c_str())];
        std::string arg2 = matches.str(2);
        int val;
        if (arg2[0] == 'r')
            val = registers[std::atoi(arg2.substr(1).c_str())];
        else
            val = std::atoi(arg2.c_str());

        switch (op) {
            case 'a': //addition
                registers[std::atoi(matches.str(3).substr(1).c_str())] += val;
                break;
            case 's': //subtraction
                registers[std::atoi(matches.str(3).substr(1).c_str())] -= val;
                break;
            case 'm': //multiplication
                registers[std::atoi(matches.str(3).substr(1).c_str())] *= val;
                break;
        }
    }
}

void load(std::string line) {
    std::regex cons(R"((\d+)\s*=>\s*(r\d+))"); //# => r#
    std::smatch matches;
    regex_search(line, matches, cons);
    if (matches.empty()) {
        std::regex regs(R"((r\d+)\s*=>\s*(r\d+))"); // r# => r#
        regex_search(line, matches, regs);
    }
    if (matches.empty()) {
        std::regex regs(R"((r\d+)\s*,\s*(\d+)\s*=>\s*(r\d+))"); //r#, # => r#
        regex_search(line, matches, regs);
    }

    if (matches.size() == 2) {
        int output = std::atoi(matches.str(2).substr(1).c_str());
        int reg = matches.str(1)[0] == 'r' ? std::atoi(matches.str(1).substr(1).c_str()) : std::atoi(matches.str(1).c_str());
        registers[output] = memory[reg];
    } else {
        int output = std::atoi(matches.str(3).substr(1).c_str());
        int reg = std::atoi(matches.str(1).substr(1).c_str());
        int addVal = std::atoi(matches.str(2).c_str());
        registers[output] = memory[reg+addVal];
    }
}

void store(std::string line) {
    std::regex cons(R"((\d+)\s*=>\s*(r\d+))"); //# => r#
    std::smatch matches;
    regex_search(line, matches, cons);
    if (matches.empty()) {
        std::regex regs(R"((r\d+)\s*=>\s*(r\d+))"); // r# => r#
        regex_search(line, matches, regs);
    }
    if (matches.empty()) {
        std::regex regs(R"((r\d+)\s*,\s*(\d+)\s*=>\s*(r\d+))"); //r#, # => r#
        regex_search(line, matches, regs);
    }

    if (matches.size() == 2) {
        int output = std::atoi(matches.str(1).substr(1).c_str());
        int reg = std::atoi(matches.str(2).substr(1).c_str());
        memory[output] = registers[reg];
    } else { //r# => r0, 0
        int input = std::atoi(matches.str(1).substr(1).c_str());
        int reg = std::atoi(matches.str(2).substr(1).c_str());
        int addVal = std::atoi(matches.str(3).c_str());
        memory[reg+addVal] = registers[input];
    }
}
