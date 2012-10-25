#ifndef __STRING_PARSE__
#define __STRING_PARSE__

#include <string>
#include <vector>

class StringParse
{
    public:
        typedef std::vector<std::string>::const_iterator iterator;

    public:
        StringParse(const std::string &str);

        iterator begin() const;
        iterator end() const;

    private:
        std::vector<std::string> m_strings;
};


#endif
