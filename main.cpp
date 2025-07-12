#include <string>
#include <tuple>
#include <cassert>

template <typename... Ts>
class custom_tie
{
      std::tuple<Ts&...> stored;
public:
      custom_tie(Ts&... args): stored(args...) {};
      int operator=(const std::tuple<Ts...> &t)
      {
          stored = t;
          return 0;
      }
};

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





