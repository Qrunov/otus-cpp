#include <iostream>
#include <type_traits>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <tuple>
#include <list>
#include <utility>


using namespace std;

template<typename T,typename = void>
struct IP
{
    static void print_ip(T ip)
    {
	cout << "base print_ip";
    }
};




template<typename T>
struct IP<T, typename enable_if<is_integral<T>::value>::type>
{
    static void print_ip(T ip){
	uint8_t* arr = reinterpret_cast<uint8_t *>(&ip);
	size_t size = sizeof(T);
	for (int i = size - 1;i > 0;i--)
	    cout << static_cast<int>(arr[i]) << ".";
	cout << static_cast<uint>(arr[0]) << endl;

    }
};


template<typename T>
struct IP<T, 
    decltype(void(declval<ostream>() << *declval<T>().cbegin() << *declval<T>().cend()))>
{
    
    static void print_ip(T ip)
    {
	
	if (ip.cbegin() != ip.cend()) 
	{
	    cout << *ip.cbegin();
	    for_each(ip.cbegin()++, ip.cend(), [](auto &val){cout << "." << val;});
	}
	cout <<  endl;
    }

};


//1
template <typename... Ts>
struct AllSameType
{
	static constexpr bool value = true;
};

template<typename T, typename U,typename... Ts>
struct AllSameType<T, U, Ts...>
{
	static constexpr bool value = is_same<T, U>::value && AllSameType<T, Ts...>::value;
};

template<typename... Ts>
constexpr bool all_same_type(tuple<Ts...>	&)
{
	return  AllSameType<Ts...>::value;
}
//1

//2
// pretty-print a tuple
template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch,Tr>&,
                      const Tuple&,
                      std::index_sequence<Is...>)
{
}


template<class Ch, class Tr, class Tuple, std::size_t Is,std::size_t... Rest>
void print_tuple_impl(std::basic_ostream<Ch,Tr>& os,
                      const Tuple& t,
                      std::index_sequence<Is, Rest...>)
{
    os << (Is == 0? "" : ".") << std::get<Is>(t);
    print_tuple_impl(os, t, std::index_sequence<Rest...>{});    
}
 
template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os,
                 const std::tuple<Args...>& t)
{
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << endl;
}
//
//2

template<typename... T>
struct is_tuple: false_type {};


template<typename... T>
struct is_tuple<tuple<T...>>: true_type {};

template<typename T>
    struct IP<T, typename enable_if<is_tuple<T>::value>::type>
{
    static	void	print_ip(T ip)
    {
	if (all_same_type(ip))
	    cout << ip << endl;
	else
	    cout << "tuple contains no same elements"<< endl;
    }
};



template<typename T>
void print_ip(T ip)
{
    IP<T>::print_ip(ip);;
}




template<>
void print_ip<string>(string ip)
{
    cout << ip << endl;
}




//template <typename... Ts>
//void	print_ip(tuple<Ts...> &t)
//{
//    static_assert(all_same_type(t),"tuple contains no same types");
//    cout << t;
//}






int main()
{

//    vector<int>	bb{1,44,66,2};
//    IP<uint64_t>::print_ip(23232389787786876);
//    IP<vector<int>>::print_ip(bb);


//    decltype(declval<vector<int>>().cbegin(), declval<vector<int>>().cend()) a;
//    cout << typeid(a).name() << endl;

//    tuple<int,int>	a;
//    static_assert(is_same<tuple_element<0, a>, tuple_element<1,a>>::value);


//    int i,j;
//    static_assert( is_same<decltype(i),decltype(j)>(),"tuple contains no same elements");

    //static_assert(all_same_type(a),"not all the same");

//    print_ip(a);
//    print_ip("12376487326837683276483674"s);
//    print_ip(bb);
//    print_ip(8376482736);
    print_ip( int8_t{-1} ); // 255 
    print_ip( int16_t{0} ); // 0.0 
    print_ip( int32_t{2130706433} ); // 127.0.0.1 
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41 
    print_ip( std::string{"Hello, World!"} ); // Hello, World! 
    print_ip( std::vector<int>{100, 200, 300, 400} ); // 100.200.300.400 
    print_ip( std::list<short>{400, 300, 200, 100} ); // 400.300.200.100 
//    tuple<int,int,int,int> a{123, 456, 789, 0};
    print_ip( std::make_tuple(123, 456, 789, 0) ); // 123.456.789.0
    //print_ip( std::make_pair(1,2));
//    print_ip( a ); // 123.456.789.0
    return 0;
} 