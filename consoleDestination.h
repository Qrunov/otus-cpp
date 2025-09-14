#pragma once
#include "interfaces.h"
#include "consoleDestination.h"
class consoleDestination : public observer
{
    void update(const block &b);
};
