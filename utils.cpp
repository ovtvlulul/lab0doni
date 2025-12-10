#include "utils.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cctype>

std::string getCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

int getIntegerInput(const std::string &prompt)
{
    while (true)
    {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        // Проверка на ведущие нули
        if (!input.empty() && input[0] == '0' && input.length() > 1)
        {
            std::cout << "Error! Numbers with leading zeros are not allowed. Try again." << std::endl;
            continue;
        }

        // Проверка на буквы и символы
        bool valid = true;
        for (char c : input)
        {
            if (!std::isdigit(c))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            std::cout << "Error! Enter a valid integer. Try again." << std::endl;
            continue;
        }

        try
        {
            return std::stoi(input);
        }
        catch (...)
        {
            std::cout << "Error! Enter a valid integer. Try again." << std::endl;
        }
    }
}

double getDoubleInput(const std::string &prompt)
{
    while (true)
    {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        // Проверка на ведущие нули
        if (!input.empty() && input[0] == '0' && input.length() > 1 && input[1] != '.')
        {
            std::cout << "Error! Numbers with leading zeros are not allowed. Try again." << std::endl;
            continue;
        }

        // Проверка на буквы и символы
        bool valid = true;
        bool hasDecimal = false;
        for (char c : input)
        {
            if (!std::isdigit(c) && c != '.' && c != '-')
            {
                valid = false;
                break;
            }
            if (c == '.')
            {
                if (hasDecimal)
                {
                    valid = false;
                    break;
                }
                hasDecimal = true;
            }
        }

        if (!valid)
        {
            std::cout << "Error! Enter a valid number. Try again." << std::endl;
            continue;
        }

        try
        {
            return std::stod(input);
        }
        catch (...)
        {
            std::cout << "Error! Enter a valid number. Try again." << std::endl;
        }
    }
}

std::string getStringInput(const std::string &prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}