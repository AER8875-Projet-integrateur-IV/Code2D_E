#include "./stringTools.hpp"

bool StringTools::ends_With(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

bool StringTools::starts_With(const std::string &str, const std::string &prefix)
{
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

bool StringTools::ends_With(const std::string &str, const char *suffix, unsigned suffixLen)
{
    return str.size() >= suffixLen && 0 == str.compare(str.size() - suffixLen, suffixLen, suffix, suffixLen);
}

bool StringTools::ends_With(const std::string &str, const char *suffix)
{
    return ends_With(str, suffix, std::string::traits_type::length(suffix));
}

bool StringTools::starts_With(const std::string &str, const char *prefix, unsigned prefixLen)
{
    return str.size() >= prefixLen && 0 == str.compare(0, prefixLen, prefix, prefixLen);
}

bool StringTools::starts_With(const std::string &str, const char *prefix)
{
    return starts_With(str, prefix, std::string::traits_type::length(prefix));
}

string StringTools::getEqualValue(string ligne)
{
    for (size_t i = 0; i < ligne.size(); i++)
    {
        if (ligne[i] == '=')
        {
            return ligne.substr(i + 1, ligne.size());
        }
    }
    printf("ERREUR: la chaine de caractère ne contient pas le symbole '='.\n");
    exit(1);
}

vector<string> StringTools::parseString(string ligne, string delimiter)
{
    regex re(delimiter);
    vector<string> tokens{
        sregex_token_iterator(ligne.begin(), ligne.end(), re, -1), {}};
    size_t i = 0;
    while (i < tokens.size())
    {
        if (tokens[i] == "")
        {
            tokens.erase(tokens.begin() + i);
        }
        else
        {
            i++;
        }
    }

    /*   
    size_t pos = 0;
    //std::string token;
    vector<string> tokens;
    while ((pos = ligne.find(delimiter)) != std::string::npos)
    {
        tokens.push_back(ligne.substr(0, pos));
        ligne.erase(0, pos + delimiter.length());
    }
    tokens.push_back(ligne); 
    */
    return tokens;
}