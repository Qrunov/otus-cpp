#pragma once
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <map>
#include <memory>
#include <vector>

class hasher
{
public:
    virtual std::string compute(const char *data, std::size_t size) = 0;
};

class hasherFactory
{
public:
    std::shared_ptr<hasher> create(std::string typeName);
    std::vector<std::string> algorithmList();

    std::string getDefault()
    {
        return m_default;
    }
    std::shared_ptr<hasher> createDefault();
    static std::shared_ptr<hasherFactory> getInstance();

private:
    hasherFactory();
    template <typename H>
    void registerHash(const std::string &typeName)
    {
        m_registry[typeName] = []() -> std::shared_ptr<hasher>
        {
            return std::make_shared<H>();
        };
    }

    using FactoryFunction = std::shared_ptr<hasher> (*)();
    std::map<std::string, FactoryFunction> m_registry;
    std::string m_default;
    static std::shared_ptr<hasherFactory> m_instance;
};
