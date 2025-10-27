#include "collector.h"
#include <iostream>
#include <memory>


using namespace std;

void collector::beginBlock()
{
    currentBlock = make_shared<block>();
    currentBlock->time = std::chrono::system_clock::to_time_t(chrono::system_clock::now());
}

void collector::addCmd(const string &cmd)
{
    if (currentBlock)
        currentBlock->cmd.push_back(cmd);
    else
        cout << "Fix it! Attemp to add cmd before block openning!" << endl;
}

void collector::endBlock()
{
    notify(*currentBlock);
    currentBlock.reset();
}
