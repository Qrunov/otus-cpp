#pragma once
#include "interfaces.h"

class fileDestination : public observer
{
    void update(const block &b);
};
