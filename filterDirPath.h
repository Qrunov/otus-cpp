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

namespace fs = std::filesystem;
class filterDirPath
{
public:
    static std::vector<fs::path> filter(std::vector<std::string> in);

private:
    static bool isDirRelative(fs::path dir1, fs::path dir2);
};
