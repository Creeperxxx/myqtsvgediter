#include <iostream>
#include "mystring.h"
#define _CRT_SECURE_NO_WARNINGS 1
int main()
{
	KMyString k("hello");
	std::cout << k.c_str() << std::endl;
	return 0;
}