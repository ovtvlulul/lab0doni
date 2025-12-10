#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string getCurrentDateTime();
int getIntegerInput(const std::string &prompt);
double getDoubleInput(const std::string &prompt);
std::string getStringInput(const std::string &prompt);

#endif