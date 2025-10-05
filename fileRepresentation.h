#pragma once
#include <boost/program_options.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <iostream>
#include <string>
#include <list>
#include <cstdint>
#include <filesystem>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <atomic>
#include <memory>
#include <regex>
#include "hasher.h"

namespace fs = std::filesystem;
namespace ipc = boost::interprocess;
using namespace std;

constexpr size_t defaultBlockSize = 4096;

class MockMapping
{
public:
    MockMapping(string n, decltype(ipc::read_only)) : m_name(n) {};

    string name()
    {
        return m_name;
    }

private:
    string m_name;
};

class MockRegion
{
public:
    MockRegion(MockMapping m, decltype(ipc::read_only), size_t o, size_t l) : m_mm(m), offset(o), length(l) {}

    size_t get_size()
    {
        return length;
    }

    const char *get_address()
    {
        mem.reset(new char[length]);
        string pattern = m_mm.name();
        pattern = pattern.substr(5);
        if (pattern.length())
            for (uint32_t i = 0; i < length; i++)
                mem.get()[i] = pattern[(offset + i) % pattern.length()];
        //	memset(mem.get(), t, length);
        return mem.get();
    }

private:
    MockMapping m_mm;

    size_t offset;
    size_t length;
    unique_ptr<char> mem;
};

template <class IMapping = ipc::file_mapping, class IRegion = ipc::mapped_region>
class fileRepresentation
{
public:
    fileRepresentation(fs::path p, size_t s, size_t bs = defaultBlockSize, shared_ptr<hasher> h = nullptr)
        : path(p), fileSize(s), blockSize(bs), m_hasher(h) {};
    bool operator==(fileRepresentation &other);
    size_t size() const
    {
        return fileSize;
    }

    string fullPath() const
    {
        return path.c_str();
    }

private:
    void computeNextPart();

    string getHashPart(size_t n);

    vector<string> hashParts;
    fs::path path;
    size_t fileSize;
    size_t blockSize;
    shared_ptr<hasher> m_hasher;
};
