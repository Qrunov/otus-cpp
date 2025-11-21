#pragma once
#include <memory>
#include <map>
#include <string>
#include <thread>
#include <shared_mutex>
#include <tuple>
#include "interfaces.h"
#include "table.h"

class Controller : public IController
{

public:
    static std::shared_ptr<Controller> getInstance();
    ~Controller() =default;

    std::string truncate(std::string) override;
    std::string insert(std::string, int, std::string) override;
    std::pair<opResult, std::string> intersect(const std::string &, const std::string &) override;
    std::pair<opResult, std::string> symmetric_difference(const std::string &, const std::string &) override;

private:
    explicit Controller();
    std::shared_mutex m_globalLock;

    static std::shared_ptr<Controller> m_instance;

    std::map<std::string, std::unique_ptr<Table>> m_tables;
};