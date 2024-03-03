#include "Utility.h"

std::string& lower(std::string& str)
{
    for (auto& c : str)
        c = tolower(c);
    return str;
}

std::string& upper(std::string& str)
{
    for (auto& c : str)
        c = toupper(c);
    return str;
}