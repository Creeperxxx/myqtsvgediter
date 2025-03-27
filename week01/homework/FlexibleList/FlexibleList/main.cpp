#include "FlexibleList.h"
#include <string>
#include <iostream>

bool DefaultCompare(const FlexibleList::Iterator& a, const FlexibleList::Iterator& b)
{
	return a < b;
}

int main()
{
	FlexibleList l;
	int a = 1;
	int b = 2;
	int c = 3;
	//std::string c("hello");
	//char d = 'a';
	//char arr[5] = "helo";
	l.pushBack(a);
	l.pushBack(b);
	l.pushBack(c);

	FlexibleList::Iterator it = l.begin();
	std::cout << *(it.getValuePoint<int>()) << std::endl;
	it++;	
	std::cout << *(it.getValuePoint<int>()) << std::endl;
	it++;
	std::cout << *(it.getValuePoint<int>()) << std::endl;

	l.FlexibleListSort(DefaultCompare);

	it = l.begin();
	std::cout << *(it.getValuePoint<int>()) << std::endl;
	it++;
	std::cout << *(it.getValuePoint<int>()) << std::endl;
	it++;
	std::cout << *(it.getValuePoint<int>()) << std::endl;
	return 0;
}