#pragma once
#include <sstream>
#include <string>
#include "interfaces.h"


class grubFromCin: public sourceInterface
{
public:
    std::string getData() override;
    bool wasFinished() override;
};


class grubFromString: public sourceInterface
{
public:
    std::string getData() override;
    bool wasFinished() override;
    void setData(std::string d)
    {
	string_data << d;
    }
private:
    std::stringstream string_data;
};

