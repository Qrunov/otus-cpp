#pragma once
#include <sstream>
#include <string>
#include "interfaces.h"

class grubFromCin : public ISource
{
public:
    std::string getData() override;
    bool wasFinished() const override;
};

class grubFromString : public ISource
{
public:
    std::string getData()  override;
    bool wasFinished() const override;
    void setData(std::string d)
    {
        string_data << d;
    }

private:
    std::stringstream string_data;
};
