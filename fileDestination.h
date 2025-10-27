#pragma once
#include "interfaces.h"
#include <chrono>

class fileDestination : public observer
{
public:
    void update(const block &b);
private:
    int 	counter{0};
    time_t	last{0};
};
