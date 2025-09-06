#include <cassert>
#include <memory>
#include <vector>
#include <iostream>
#include <cstddef>
#include <algorithm>
#include <map>
#include <string.h>
#include <tuple>
#include <stdexcept>

using namespace std;


template <int N>
class matrix
{
public:
    using key_type   = size_t;
    using value_type = matrix<N - 1>;


    class Iterator
    {

	typename map<matrix::key_type, matrix::value_type>::iterator		m_current_key;
	typename value_type::iterator						m_current_value;
	public:
	    Iterator() {};
	    Iterator(matrix *ptr): m_container(ptr) {};

	    auto operator*() const
	    {
		if (m_end)
		    throw runtime_error("attemping * on matrix::end()");
		return tuple_cat(make_tuple(m_current_key -> first), *m_current_value); 
	    }

	    Iterator& operator++()
	    { 
		if (m_end)
		    return *this;

		if (m_current_value != m_current_key -> second.end())
		    ++m_current_value;

		seek_valid();
		return *this;
	    }

	    bool operator!=(const Iterator &other) const
	    {
		 return !this ->operator==(other);
	    }


	    bool operator==(const Iterator &other) const
	    {
		 return  ((true == m_end) == other.m_end) || (m_current_key == other.m_current_key && m_current_value == other.m_current_value);
	    }

	    friend class matrix;

	private:
	    //shared_ptr<matrix>	m_container;
	    matrix*	m_container;
	    bool m_end{false};

	    void	seek_valid()
	    {
		while (m_current_value == m_current_key -> second.end())
		{
		
			m_current_key++;
		        if (m_current_key != m_container -> m_storage.end())
			{
			    m_current_value = m_current_key -> second.begin();
			    if (m_current_value != m_current_key -> second.end())
			        break;
			}
			else
		        {
			    m_end = true;
			    break;
		        }
		}
	    }

	    Iterator&	begin() 
	    {
		if (!m_container -> size())
		{
		    m_end = true;
		    return *this;
		}

		if (m_container)
		{
		    m_current_key = m_container -> m_storage.begin();
		    m_current_value = m_current_key -> second.begin();
		    m_end = false;
		    seek_valid();
		}
		
		return *this;
	    }

	    Iterator&	end() 
	    {
		if (m_container)
		{
		    m_current_value = m_current_key -> second.end();
		    m_current_key = m_container -> m_storage.end();
		}
		m_end = true;
		return *this;
	    }
    };
    using iterator =  Iterator;
    using const_iterator =  const Iterator;

    friend class	Iterator;

    matrix(int d):m_default(d) {};
    matrix(){};


/*
    using iterator =  Iterator<N>;
//    using const_iterator = map<key_type, value_type>::const_iterator;
*/
    iterator begin()
    { 
	//TODO: shared weak? ptr
	iterator	it(this);
	return 	it.begin();
    }

    iterator end()
    { 
	//TODO: shared weak? ptr
	iterator	it(this);
	return 	it.end(); 
    }

//    const_iterator cbegin() const { return m_storage.cbegin(); }
//    const_iterator cend() const { return m_storage.cend(); }

    size_t size() const
    {
	size_t res = 0;
	for (auto e: m_storage)
	    res += e.second.size();

	return res;
    }

    value_type operator[](size_t index) const
    {
	if (m_storage.find(index) != m_storage.cend())
	    return m_storage.find(index) -> second;
	
	return value_type(m_default);
    }

    value_type& operator[](size_t index)
    {
	if (m_storage.find(index) != m_storage.cend())
	    return m_storage[index];
	
	return m_storage.emplace(pair<key_type,value_type>(index,value_type(m_default))).first -> second;
    }

    void	setDefault(int def)
    {
	m_default = def;
    }


private:
    map<key_type, value_type>	m_storage;
    int	m_default{-1};

//    friend class matrix<N>::Iterator;
};


template<>
class	matrix<0>
{

public:
    using value_type = int;
    using key_type   = size_t;

    using iterator =  map<key_type, value_type>::iterator;
    using const_iterator = map<key_type, value_type>::const_iterator;

    class reference
    {
	public:
	    explicit reference(map<key_type, value_type>	&s, key_type i, value_type def = -1): m_storage(s), m_index(i), m_default(def) {};
	    reference& operator=(value_type x)
	    {
		if (m_storage.find(m_index) != m_storage.cend())
		{
	    	    if (x == m_default)
			//TODO: check and remove empty parent's map
			//TODO: may be need link to parent
			m_storage.erase(m_index);
		    else
			m_storage[m_index] = x;
		}
		else
		{
		    if (x != m_default)
			m_storage.emplace(m_index, x);
		}
		return *this;
	    }
	    operator int() const
	    {
		if (m_storage.find(m_index) != m_storage.cend())
		    return m_storage[m_index];
		return m_default;
	    }
	    
	private:
	    map<key_type, value_type>	&m_storage;
	    key_type			m_index;
	    value_type			m_default{-1};
	friend class matrix<0>;
    };

    matrix(){};
    matrix(int d):m_default(d) {};

    iterator begin() { return m_storage.begin(); }
    iterator end() { return m_storage.end(); }

    const_iterator cbegin() const { return m_storage.cbegin(); }
    const_iterator cend() const { return m_storage.cend(); }

    size_t size() const
    {
	return m_storage.size();
    }

    value_type operator[](key_type index) const
    {
	if (m_storage.find(index) != m_storage.cend())
	    return m_storage.find(index) -> second;
	return m_default;
    }

    reference operator[](key_type index)
    {
	reference x(m_storage, index, m_default);
	return x;
    }

    void	setDefault(value_type def)
    {
	m_default = def;
    }


private:
    map<key_type, value_type>	m_storage;
    value_type			m_default{-1};
};


using Matrix = matrix<1>;


int main()
{
//      matrix<3>	m;
//      m[0][0][6][0] = 5;
//      m[7][7][7][1] = 16;
//      m[0][1][7][1] = 171;
//      m[0][1][1][1] = 111;

//      cout << m[0][0] << " " << m[0][1] << " " << m[10][10] << " " << m.size();

//      cout << m[0][0][6] << " " << m[7][7][7] << " " << m.size();	
//      cout << endl;

//     matrix<2>::Iterator	i = m.begin();
    //m.begin();



//    Matrix mmm;
//    for (auto e: m)
//    {
//	int x,y,z,w,v;
//	tie(x,y,z,w,v) = e;
//	cout << x << " " <<  y << " " << z  << " " << w << " " << v << endl;
//    }
//    auto elem = mmm[100, 100];
//    elem = 10;
//    cout << mmm[100, 100];


/*    Matrix	matrix;
    assert(matrix.size() == 0);
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);

    ((matrix[100][100] = 314) = 0) = 217;
    assert(matrix[100][100] == 217);
    assert(matrix.size() == 1);

    for (auto c:matrix)
    {
	int x,y,v;
	tie(x, y, v) = c;
	cout << x << y << v << endl;
    }*/

    Matrix m;
    m.setDefault(0);

    for (int i = 0;i < 10;i++)
    {
	m[i][i] = i;
	m[i][9 - i] = 9 - i;
    }


    for (int i = 1;i < 9;i++)
    {
	for (int j = 1;j < 9;j++)
	    cout << " " << m[i][j];
	cout << endl;
    }
    cout << m.size() << endl;

    for (auto e:m)
    {
	int a, b, c;
	tie(a, b, c) = e;
	cout << a << b << c << endl;
    }


    return 0;
}
