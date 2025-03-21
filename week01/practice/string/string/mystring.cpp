#include "mystring.h"
KMyString::KMyString()
{
	init();
}
KMyString::KMyString(const char* str)
{
	init();
	strcpy_s(m_str, KMYSTRING_CAPACITY, str);


	/*int index = 0;
	while (str[index] != '\0' && index <= KMYSTRING_CAPACITY)
	{
		m_str[index] = str[index];
		index++;
	}*/

	//m_str[index] = str[index];
	//m_size = index + 1;
}
KMyString::KMyString(const KMyString& other)
{
	init();
	strcpy_s(m_str, KMYSTRING_CAPACITY, other.c_str());
	m_size = other.size();

	//int index = 0;
	/*while (other[index] != '\0' && index < KMYSTRING_CAPACITY)
	{
		m_str[index] = other[index];
		index++;
	}
	if (index >= KMYSTRING_CAPACITY)
	{
		m_str[index - 1] = '\0';
		m_size = index;
	}
	else
	{
		m_str[index] = '\0';
		m_size = index + 1;
	}*/

}
KMyString& KMyString::operator=(const KMyString& other)
{
	strcpy_s(m_str, KMYSTRING_CAPACITY, other.c_str());
	m_size = other.size();


	/*int index = 0;
	while (other[index] != '\0' && index < KMYSTRING_CAPACITY)
	{
		m_str[index] = other[index];
		index++;
	}
	if (index > KMYSTRING_CAPACITY)
	{
		m_str[index - 1] = '\0';
		m_size = index;
	}
	else
	{
		m_str[index] = '\0';
		m_size = index + 1;
	}
	return *this;*/
}
KMyString::~KMyString()
{
	delete m_str;
}

KMyString& KMyString::operator+(const KMyString& other)
{
	int size = 0;
	if (m_size + other.size() > KMYSTRING_CAPACITY)
	{
		size = KMYSTRING_CAPACITY - m_size;
	}
	else
	{
		size = other.size();
	}
	strncat(m_str, other.c_str(), size);
}
bool KMyString::operator==(const KMyString& other)
{
	if (strcmp(m_str, other.c_str()) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
char KMyString::operator[](int i)
{
	if (i > static_cast<int>(m_size - 1))
	{
		return '0';
	}
	else
	{
		return m_str[i];
	}
}

size_t KMyString::size() const
{
	return m_size;	
}
char* KMyString::c_str() const
{
	return m_str;
}

void KMyString::init()
{
	m_str = new char[KMYSTRING_CAPACITY];
	m_size = 0;
}

//std::ostream& KMyString::operator<<(std::ostream& os, const KMyString& other)
//{
//    os << other.c_str();
//	return os;
//}