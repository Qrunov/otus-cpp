#pragma once
#include <vector>
#include <string>
using opResult = std::vector<std::tuple<int, std::string, std::string>>;

class IController
{
public:
    virtual std::string truncate(std::string) = 0;
    virtual std::string insert(std::string, int, std::string) = 0;
    virtual std::pair<opResult, std::string> intersect(const std::string &, const std::string &) = 0;
    virtual std::pair<opResult, std::string> symmetric_difference(const std::string &, const std::string &) = 0;
};
