#include "FlexibleList.h"
#include <string>
#include <iostream>

bool DefaultCompare(const FlexibleList::Iterator& a, const FlexibleList::Iterator& b)
{
	return a > b;
}
bool myCompare(const double& a, const double& b)
{
	return a < b;
}

class person
{
public:
	person()
	{
		static int age = 1;
		m_age = ++age;
	}
public:
	bool operator==(const person& other) const // 这里注意要定义为const，不然编译器会提示不可用，因为person对象是const，通过该对象只能使用const修饰的成员函数
	{
		return m_age == other.m_age;
	}
	bool operator<(const person& other) const 
	{
		return m_age < other.m_age;
	}
	void print() const
	{
		std::cout << "my age is :  " << m_age << std::endl;
	}
	int getAge() const
	{
		return m_age;
	}
private:
	int m_age;
};

bool personCompare(const person& a, const person& b)
{
	return a.getAge() > b.getAge();
}

int main()
{
	FlexibleList l;
	l.pushBack(person());
	l.pushBack(person());
	l.pushBack(person());
	for (FlexibleList::Iterator it = l.begin(); it != l.end(); it++)
	{
		it.getValue<person>().print();
	}
	l.FlexibleListSort(personCompare); // sort函数还是可以扩展的，这里只支持list中所有元素类型相同，其实可以指定排序某种元素，其他元素节点都移动到node后面
	for (FlexibleList::Iterator it = l.begin(); it != l.end(); it++)
	{
		it.getValue<person>().print();
	}
	return 0;
	//FlexibleList l{ 1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0 };
	//for (FlexibleList::Iterator it = l.begin(); it != l.end(); it++)
	//{
	//	std::cout << it.getValue<double>() << std::endl;
	//}

	////l.FlexibleListSort(DefaultCompare);
	////l.FlexibleListSort(myCompare);
	//l.FlexibleListSort([](const double& a, const double& b) { return a < b; });
	//for (FlexibleList::Iterator it = l.begin(); it != l.end(); it++)
	//{
	//	std::cout << it.getValue<double>() << std::endl;
	//}
	//return 0;
	//int a = 1;
	//int b = 2;
	//int c = 3;
	////std::string c("hello");
	////char d = 'a';
	////char arr[5] = "helo";
	//l.pushBack(a);
	//l.pushBack(b);
	//l.pushBack(c);

	//FlexibleList::Iterator it = l.begin();
	//std::cout << *(it.getValuePoint<int>()) << std::endl;
	//it++;	
	//std::cout << *(it.getValuePoint<int>()) << std::endl;
	//it++;
	//std::cout << *(it.getValuePoint<int>()) << std::endl;

	//l.FlexibleListSort(DefaultCompare);

	//it = l.begin();
	//std::cout << *(it.getValuePoint<int>()) << std::endl;
	//it++;
	//std::cout << *(it.getValuePoint<int>()) << std::endl;
	//it++;
	//std::cout << *(it.getValuePoint<int>()) << std::endl;
	//return 0;
}