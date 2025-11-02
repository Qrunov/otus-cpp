#pragma once
#include <string>
#include <list>
#include <memory>

struct commandBlock
{
    time_t time;
    std::list<std::string> cmd;
};

class ISource
{
public:
    virtual std::string getData() = 0;
    virtual bool wasFinished() const = 0;
};

class observer
{
public:
    virtual void update(const commandBlock &) = 0;
};

class ICollector
{
public:
    virtual void beginBlock() = 0;
    virtual void addCmd(const std::string &) = 0;
    virtual void endBlock() = 0;
};

class subject
{
public:
    void registration(const std::shared_ptr<observer>& ob);

protected:
    void notify(const commandBlock &b) const;

private:
    std::list<std::shared_ptr<observer>> m_observer_list;
};
