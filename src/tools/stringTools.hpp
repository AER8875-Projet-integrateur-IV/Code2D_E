#ifndef STRINGTOOLS_HPP
#define STRINGTOOLS_HPP
#include <iostream>
#include <string>

using namespace std;

class StringTools
{
public:
    static bool ends_With(const std::string &str, const std::string &suffix);
    static bool starts_With(const std::string &str, const std::string &prefix);
    static bool ends_With(const std::string &str, const char *suffix, unsigned suffixLen);
    static bool ends_With(const std::string &str, const char *suffix);
    static bool starts_With(const std::string &str, const char *prefix, unsigned prefixLen);
    static bool starts_With(const std::string &str, const char *prefix);
    string getEqualValue(string ligne);
};

#endif