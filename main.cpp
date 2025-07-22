#include <memory>
#include <vector>
#include <iostream>
#include <cstddef>
#include <algorithm>
#include <list>
#include <map>
#include <string.h>

using namespace std;

template<typename T, size_t limit = 10 > 
class allocator_t
{
public:
    using value_type = 	T;
    using pointer    =  T*;
    explicit allocator_t() {}
    pointer allocate(size_t	n)
    {
	pointer ret;
	if (n > limit)
	{
	    ret = static_cast<pointer>(malloc(sizeof(T) * n));
	    if (!ret)
		throw bad_alloc();
	    return ret;
	}
	if (!m_pool || m_index + limit > n)
	{
	    m_index = 0;
	    auto new_shunk =  move(make_unique<T[]>(limit));
	    m_pool = new_shunk.get();
	    m_active.push_back(move(new_shunk));
	}

	ret = static_cast<pointer>(m_pool + m_index);
	m_index += n;
	return ret;
    }
    void deallocate(pointer p, size_t n)
    {
//	cout << "asking for deallocate " << n << endl;
	if (n > limit)
	    free(p);
    }
    template <typename U>
    struct rebind {
	using other = allocator_t<U, limit>;
    };
    private:
	T*	m_pool{ nullptr };
	int	m_index{ 0 };
	list<unique_ptr<T[]>>	m_active;

    bool	operator==(const allocator_t &a) const { return this -> m_pool == a.m_pool && this -> m_index != a.m_index; }
    bool	operator!=(const allocator_t &a) const { return this -> m_pool != a.m_pool; }

};





template<typename T, typename Alloc = std::allocator<T>>
class simple_vector
{
public:
//    explicit simple_vector(Alloc& a = Alloc()):m_a(a) {}
    simple_vector() {}
    simple_vector(size_t t) { reserve(t); }
    template<typename IT>
    class Iterator
    {
	IT*	pointer;
	public:
	    Iterator(IT* ptr): pointer(ptr) {}
	    T& operator*() const { return *pointer; }
	    Iterator& operator++() 
	    { 
		pointer++; 
		return *this; 
	    }
	    bool operator!=(const Iterator &other) const { return pointer != other.pointer; }
    };

    using value_type		=	T;
    using reference		= 	T&;
    using const_reference	=	const T&;
    using iterator		= 	Iterator<T>;
    using const_iterator	= 	const Iterator<T>;
    using difference_type	= 	int;
    using size_type		=	size_t;

    iterator begin()	const	{ return iterator(m_array); }
    iterator end()	const	{ return iterator(m_array + m_size); }

    const_iterator cbegin()	const	{ return iterator(m_array); }
    const_iterator cend()	const	{ return iterator(m_array + m_size); }


    T&	operator[](size_t index)	const	{ return *(m_array + index); }

    size_type	size()	const	{ return m_size; }
    size_type	capacity()	const	{ return m_count; }
    void	reserve( size_type new_capacity )
    {
	if (new_capacity > m_count)
	{
		T* new_array =  allocator_traits<Alloc>::allocate(m_a, new_capacity);
		if (m_count)
		{
		    memcpy(new_array, m_array, m_count * sizeof(T));
		    allocator_traits<Alloc>::deallocate(m_a, m_array, m_count);
		}
		m_array = new_array;
		m_count = new_capacity;
	}
    }
    bool	empty()	const    { return (m_size == 0); }

    void	clear() 
    {
    	if (m_count)
	    allocator_traits<Alloc>::deallocate(m_a, m_array, m_count);
	m_size = 0;
    }


    template <typename TT>
    void	push_back( TT&& value )
    {
	if (m_size == m_count)
		reserve(m_count ? m_count * 2 : 1);

	m_array[ m_size ] = value;
	m_size++;
    }

    ~simple_vector() 
    {	
	if (m_count)
	    allocator_traits<Alloc>::deallocate(m_a, m_array, m_count);
    }
private:
    size_type	m_count{ 0 };
    size_type	m_size{ 0 };
    T*		m_array{ nullptr };
    Alloc	m_a;
};


int main()
{
    constexpr int 	high	= 10;
    map<int,int>				base_map;
    auto print_map = [](auto &map)
    {
        map[0] = 1;
	map[1] = 1;
	for (int i = 2;i < high;i++)
	    map[i] = map[i - 1] * i;
	for_each(map.cbegin(), map.cend(), [](auto &a){cout << a.first << " " << a.second << " ";});
	cout << endl;
    };
    print_map(base_map);

    map<int,int,less<int>,allocator_t<int>>	alloc_map;

    print_map(alloc_map);


    simple_vector<int>	s;
    auto print_vector = [](auto &vector)
    {
	for (int i = 0; i < high;i++)
	    vector.push_back(i);

	for_each(vector.cbegin(), vector.cend(), [](auto &a){cout << a << " ";});
        cout << endl;
    };
    print_vector(s);

    simple_vector<int, allocator_t<int>>	alloc_v;
    print_vector(alloc_v);

    return 0;
}