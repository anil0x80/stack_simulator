#include "util.h"

std::vector<std::string> util::tokenize(std::string s, std::string del)
{
    std::vector<std::string> ret;
    int start = 0;
    int end = s.find(del);
    while (end != std::string::npos) {
        ret.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    ret.push_back(s.substr(start, end - start));

    return ret;
}
