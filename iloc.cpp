#include <unordered_map>
#include "iloc.h"

int memory[20480];
std::map<int, int> registers;

void exec(std::string code) {
    if (code.substr(0, 2) != "//" && !code.empty()) {
        std::regex cons(R"((\w+))");
        std::smatch matches;
        regex_search(code, matches, cons);

        if (matches.size() > 1) {
            auto action = matches.str(1); //what tokens should we expected next

            if (action.substr(0, 3) == "add" || action.substr(0, 3) == "sub" || action.substr(0, 3) == "mul")
                math(code, action[0]);
            else if (action.substr(0, 4) == "load")
                load(code);
            else if (action.substr(0, 4) == "stor")
                store(code);
            else if (action.substr(0, 4) == "outp")
                print(code);
            else if (action.substr(1, 6) == "shift")
                shift(code);
            else
                printf("unknown: %s\n", action.c_str());
        }
    }
}

void init() {
    memory[1024] = 1;
    memory[1028] = 1;
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
        int output = std::stoi(matches.str(3).substr(1));
        registers[output] = registers[std::stoi(matches.str(1).substr(1))];

        std::string arg2 = matches.str(2);
        int val;
        if (arg2[0] == 'r')
            val = registers[std::stoi(arg2.substr(1))];
        else
            val = std::stoi(arg2);

        switch (op) {
            case 'a': //addition
                registers[output] += val;
                break;
            case 's': //subtraction
                registers[output] -= val;
                break;
            case 'm': //multiplication
                registers[output] *= val;
                break;
        }

        //printf("value: %d\n", registers[output]);
    }
}

void shift(std::string line){
    std::regex regs(R"((r\d+)\s*,\s*(r\d+)\s*=>\s*(r\d+))"); //r#, # => r#
    std::smatch matches;
    regex_search(line, matches, regs);
    int output = std::stoi(matches.str(3).substr(1));
    int reg1 = registers[std::stoi(matches.str(1).substr(1))];
    int reg2 = registers[std::stoi(matches.str(2).substr(1))];
    if(line[0] == 'l') //left shift
        registers[output] = reg1 << reg2;
    else //right shift
        registers[output] = reg1 >> reg2;
}

void load(std::string line) {
    std::regex regs(R"((r\d+)\s*,\s*(\d+)\s*=>\s*(r\d+))"); //r#, # => r#
    std::smatch matches;
    regex_search(line, matches, regs);
    if (matches.empty()) {
        std::regex regs1(R"((r\d+)\s*=>\s*(r\d+))"); // r# => r#
        regex_search(line, matches, regs1);
    }
    if (matches.empty()) {
        std::regex cons(R"((\d+)\s*=>\s*(r\d+))"); //# => r#
        regex_search(line, matches, cons);
    }

    if (matches.size() == 3) {
        int output = std::stoi(matches.str(2).substr(1));
        int reg = matches.str(1)[0] == 'r' ? memory[registers[std::stoi(matches.str(1).substr(1))]] : std::stoi(
                matches.str(1));
        registers[output] = reg;
    } else {
        int output = std::stoi(matches.str(3).substr(1));
        int reg = registers[std::stoi(matches.str(1).substr(1))];
        int addVal = std::stoi(matches.str(2));
        registers[output] = memory[reg + addVal];
    }
}

void store(std::string line) {
    std::regex regs(R"((r\d+)\s*=>\s*(r\d+)\s*,\s*(\d+)\s*)"); //r#, # => r#
    std::smatch matches;
    regex_search(line, matches, regs);
    if (matches.empty()) {
        std::regex sim(R"((r\d+)\s*=>\s*(r\d+))"); //r# => r#
        regex_search(line, matches, sim);
    }

    if (matches.size() == 3) {
        int output = registers[std::stoi(matches.str(2).substr(1))];
        int reg = std::stoi(matches.str(1).substr(1));
        memory[output] = registers[reg];
    } else { //r# => r0, 0
        int input = std::stoi(matches.str(1).substr(1));
        int reg = registers[std::stoi(matches.str(2).substr(1))];
        int addVal = std::stoi(matches.str(3));
        memory[reg + addVal] = registers[input];
    }
}

void print(std::string line) {
    std::regex regs(R"((r\d+)\s*,\s*(\d+))"); // r# => r#
    std::smatch matches;
    regex_search(line, matches, regs);
    if (matches.empty()) {
        std::regex cons(R"((\d+))");
        regex_search(line, matches, cons);
    }

    if (matches.size() == 2) {
        printf("%d\n", memory[std::stoi(matches.str(1))]);
    } else {
        printf("%d\n", memory[registers[std::stoi(matches.str(1).substr(1))] + std::stoi(matches.str(2))]);
    }
}