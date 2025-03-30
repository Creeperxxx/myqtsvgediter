#include "FlexibleList.h"
#include <string>
#include <fstream>
#include <iostream>

bool DefaultCompare(const FlexibleList::Iterator& a, const FlexibleList::Iterator& b)
{
	return a > b;
}
bool doubleCompare(const double& a, const double& b)
{
	return a < b;
}

bool intCompare(const int& a, const int& b)
{
	if (FLEXIBLELISTSORTCOMPASC == true)
	{
		return a < b;
	}
	else
	{
		return a > b;
	}
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

//int main()
//{
//	FlexibleList l;
//	for (int i = 0; i < 10; ++i)
//	{
//		l.pushBack(i);
//	}
//	FlexibleList::Iterator it = l.begin();
//	while (!l.empty())
//	{
//		it = l.erase(it); // 假设有合适的 erase 方法
//	}
//	return 0;
//}



int main()
{
	FlexibleList l;
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	double e = 5.5;
	double f = 6.6;
	double g = 7.7;
	l.pushBack(a);
	l.pushBack(b);
	l.pushBack(c);
	l.pushBack(d);
	l.pushBack(e);
	l.pushBack(f);
	l.pushBack(g);
	std::cout << "原来为：";
	for (auto it = l.begin(); it != l.end(); ++it)
	{
		if (it.getValueTypeIndex() == typeid(int))
		{
			std::cout << it.getValue<int>() << " ";
		}
		else if (it.getValueTypeIndex() == typeid(double))
		{
			std::cout << it.getValue<double>() << " ";
		}
		
	}
	std::cout << std::endl;
	l.FlexibleListSort(intCompare);
	std::cout << "后来为：";
	for (auto it = l.begin(); it != l.end(); ++it)
	{
		if (it.getValueTypeIndex() == typeid(int))
		{
			std::cout << it.getValue<int>() << " ";
		}
		else if (it.getValueTypeIndex() == typeid(double))
		{
			std::cout << it.getValue<double>() << " ";
		}
	}
	std::cout << std::endl;
	system("pause");
	return 0;
}
	//std::ofstream file("record.txt", std::ios::app);
	//file << "比较顺序：" << (FLEXIBLELISTSORTCOMPASC ? "升序true" : "降序false") << " " << "参数类型正确的排在" << (FLEXIBLELISTSORTCOMPPARAMTYPEBEFORE ? "前面true" : "后面false")<< " " << "两个非正常参数类型数据：" << (FLEXIBLELISTSORTCOMPNONPARAMTYPEORDER ? "顺序true" : "非顺序false") << "\n";
	//file << "原来为：";
	//for (auto it = l.begin(); it != l.end(); it++)
	//{
	//	if (it.getValueTypeIndex() == typeid(int))
	//	{
	//		file << it.getValue<int>() << " ";
	//	}
	//	else if (it.getValueTypeIndex() == typeid(double))
	//	{
	//		file << it.getValue<double>() << " ";
	//	}
	//}
	//file << "\n";
	//l.FlexibleListSort(intCompare);
	//file << "排后为：";
	//for (auto it = l.begin(); it != l.end(); it++)
	//{
	//	if (it.getValueTypeIndex() == typeid(int))
	//	{
	//		file << it.getValue<int>() << " ";
	//	}
	//	else if (it.getValueTypeIndex() == typeid(double))
	//	{
	//		file << it.getValue<double>() << " ";
	//	}
	//}
	//file << "\n";
	//file << "\n";
	//return 0;
	// a > b : 6.6 5.5 4 3 2 1
	// a < b : 6.6 5.5 1 2 3 4
	// a < b : 1 2 3 4 5.5 6.6
	// a > b : 4 3 2 1 5.5 6.6




	//l.pushBack(person());
	//l.pushBack(person());
	//l.pushBack(person());
	//for (FlexibleList::Iterator it = l.begin(); it != l.end(); it++)
	//{
	//	it.getValue<person>().print();
	//}
	//l.FlexibleListSort(personCompare); // sort函数还是可以扩展的，这里只支持list中所有元素类型相同，其实可以指定排序某种元素，其他元素节点都移动到node后面
	//for (FlexibleList::Iterator it = l.begin(); it != l.end(); it++)
	//{
	//	it.getValue<person>().print();
	//}
	//return 0;


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
//}