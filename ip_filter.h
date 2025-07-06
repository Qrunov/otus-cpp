#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <list>
#include <limits>
#include <array>
#include <functional>
using namespace std;

class ip
{
public:
    ip() : n(0) {};
    bool operator<=(const ip &c) const;
    string get_string() const;
    u_char get_field(int i) const;
    void set_field(int i, u_char f);

private:
    u_int32_t n;
};

class ipParser
{
public:
    enum class Error_code
    {
        no_error,
        input_error,
        range_error
    };

    static tuple<Error_code, vector<ip>> parseStream(istream &);
};