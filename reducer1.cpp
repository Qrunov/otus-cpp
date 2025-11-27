#include <iostream>
#include <string>
#include <cstdint>
#include "disp.h"


int main()
{
    size_t count = 0;
    std::string line;
    uint64_t	sum = 0;
    while (std::getline(std::cin, line))
    {
        sum += atoi(line.c_str());
	count++;
    }
    std::cout << (double)sum / count << std::endl;
    return 0;
}