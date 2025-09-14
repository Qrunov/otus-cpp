#pragma once
#include "interfaces.h"

class grubFromCin : public sourceInterface
{
public:
    std::string getData() override;
    bool wasFinished() override;
};
