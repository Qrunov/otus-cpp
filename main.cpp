#include <string>
#include <tuple>
#include <cassert>

template <typename... Ts>
class custom_tie_
{
      std::tuple<Ts&...> stored;
public:
      explicit custom_tie_(Ts&... args): stored(args...) {};
      custom_tie_& operator=(const std::tuple<Ts...> &t)
      {
          stored = t;
          return *this;
      }
};

template <typename... Ts>
auto custom_tie(Ts&... args) 
{
    return custom_tie_<Ts...>(args...);
}

auto getPerson() {
 const std::string name = "Petia";
 const std::string secondName = "Ivanoff";
 const std::size_t age = 23;
 const std::string department = "Sale";
 return std::make_tuple(
 name, secondName, age, department
 );
}
int main(int, char **) {
 std::string name, secondName, department;
 std::size_t age;
 custom_tie(name, secondName, age, department) = getPerson();

 assert(name == "Petia");
 assert(secondName == "Ivanoff");
 assert(age == 23);
 assert(department == "Sale");
 return 0;
}





