//#include "memorypoll.h"
//#include <iostream>
//
//int main()
//{
//	KMemoryPoll poll(10485760, 2097152);
//	void* ptr = poll.alloc(sizeof(int));
//	if (ptr == nullptr)
//	{
//		std::cout << "ptr is nullptr" << std::endl;
//		return 0;
//	}
//	//poll.printPollDetails();
//	new(ptr) int(2);
//	int* p = static_cast<int*>(ptr);
//	std::cout << "p : " << *p << std::endl;
//	
//	*p = 3;
//	std::cout << "p : " << *p << std::endl;
//
//	if (poll.Free(p))
//	{
//		std::cout << "free : true" << std::endl;
//	}
//	else
//	{
//		std::cout << "free : false" << std::endl;
//	}
//	return 0;
//}