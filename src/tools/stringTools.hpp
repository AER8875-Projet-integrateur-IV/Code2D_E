#include <string>

static bool endsWith(const std::string &str, const std::string &suffix);
static bool startsWith(const std::string &str, const std::string &prefix);
static bool endsWith(const std::string &str, const char *suffix, unsigned suffixLen);
static bool endsWith(const std::string &str, const char *suffix);
static bool startsWith(const std::string &str, const char *prefix, unsigned prefixLen);
static bool startsWith(const std::string &str, const char *prefix);