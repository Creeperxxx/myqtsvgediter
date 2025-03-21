#pragma once
constexpr int KMYSTRING_CAPACITY = 100;
#include <cstring>
#include <algorithm>
#include <iostream>

class KMyString
{
	char* m_str;
	size_t m_size;
public:		
	KMyString();
	KMyString(const char* str);
	KMyString(const KMyString& other);
	KMyString& operator=(const KMyString& other);
	~KMyString();

	KMyString& operator+(const KMyString& other);
	bool operator==(const KMyString& other);
	char operator[](int i);
	//std::ostream& operator<<(std::ostream& os, const KMyString& other);

	size_t size() const;
	char* c_str() const;

	void init();
};