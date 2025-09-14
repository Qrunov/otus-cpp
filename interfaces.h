#pragma once
#include <string>
#include <list>
#include <memory>

struct block
{
    time_t time;
    std::list<std::string> cmd;
};

class sourceInterface
{
public:
    virtual std::string getData() = 0;
    virtual bool wasFinished() = 0;
};

class observer
{
public:
    virtual void update(const block &) = 0;
};

class collectorInterface
{
public:
    virtual void beginBlock() = 0;
    virtual void addCmd(const std::string &) = 0;
    virtual void endBlock() = 0;
};

class subject
{
public:
    void registration(std::shared_ptr<observer> ob);

protected:
    void notify(const block &b);

private:
    std::list<std::shared_ptr<observer>> m_observer_list;
};
