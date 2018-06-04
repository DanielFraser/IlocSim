#ifndef ILOCSIM_ILOC_H
#define ILOCSIM_ILOC_H

#include <iostream>
#include <regex>

void exec(std::string);
void math(std::string, char op);
void store(std::string);
void load(std::string);
void print(std::string);
void init();
void shift(std::string);

#endif //ILOCSIM_ILOC_H
