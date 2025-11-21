#pragma once
#include <sstream>
#include <memory>
#include "interfaces.h"

class Command
{
public:
    Command(std::shared_ptr<IController> contr) : m_controller(contr) {}
    void execute(std::string cmd);
    std::string getResult()
    {
        std::string res = m_result.str();
        m_result.clear();
        m_result.str("");
        return res + "\n";
    }

private:
    std::shared_ptr<IController> m_controller;
    std::stringstream m_result;
};