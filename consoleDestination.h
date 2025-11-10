#pragma once
#include "interfaces.h"
#include "consoleDestination.h"
class consoleDestination : public observer
{
public:
    void update(const commandBlock &b);
};
