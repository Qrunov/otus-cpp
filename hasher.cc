#include "hasher.h"

using namespace std;

class MD_hasher : public hasher
{
    string compute(const char *data, size_t size)
    {
        boost::uuids::detail::md5 hash;
        boost::uuids::detail::md5::digest_type digest;
        hash.process_bytes(data, size);
        hash.get_digest(digest);
        return to_string(*digest);
    }
};

class CRC_hasher : public hasher
{
    string compute(const char *data, size_t size)
    {
        boost::crc_32_type result;
        result.process_bytes(data, size);
        return to_string(result.checksum());
    }
};

shared_ptr<hasherFactory> hasherFactory::m_instance = nullptr;

hasherFactory::hasherFactory()
{
    registerHash<MD_hasher>("md5");
    registerHash<CRC_hasher>("crc32");
    m_default = "crc32";
}

shared_ptr<hasher> hasherFactory::createDefault()
{
    return create(m_default);
}

shared_ptr<hasher> hasherFactory::create(string typeName)
{
    auto it = m_registry.find(typeName);
    if (it != m_registry.end())
    {
        return it->second();
    }
    return nullptr;
}

vector<string> hasherFactory::algorithmList()
{
    vector<string> list;
    for (auto e : m_registry)
        list.push_back(e.first);
    return list;
}

shared_ptr<hasherFactory> hasherFactory::getInstance()
{
    if (!m_instance)
        m_instance.reset(new hasherFactory());
    return m_instance;
}