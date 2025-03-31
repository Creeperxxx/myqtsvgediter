#pragma once
#include <fstream>
#include <string>
#include <cstdio>

class FileRAII
{
public:
	FileRAII(std::string filename)
	{
		if (filename.size() == 0)
		{
			throw std::runtime_error("路径为0");
		}
		m_filePoint = std::fopen(filename.c_str(), "w");
		if (nullptr == m_filePoint)
		{
			throw std::runtime_error("打开失败");
		}
	}
	FILE* getFilePoint() const
	{
		return m_filePoint;
	}
	~FileRAII()
	{
		std::fclose(m_filePoint);
	}
private:
	FILE* m_filePoint;

};

template <typename T>
class MySharedPtr
{
public:
	MySharedPtr(T* ptr) : m_ptr(ptr)
	{
		if (nullptr == m_ptr)
		{
			m_count = new int(0);
		}
		else
		{
			m_count = new int(1);
		}
	}
	MySharedPtr(const MySharedPtr& other) : m_ptr(other.m_ptr), m_count(other.m_count)
	{
		if (nullptr != m_ptr)
		{
			++(*m_count);
		}
	}
	MySharedPtr& operator=(const MySharedPtr& other)
	{
		if (this != &other)
		{
			release();
			m_ptr = other.m_ptr;
			m_count = other.m_count;
			if (m_ptr != nullptr && m_count != nullptr)
			{
				++(*m_count);
			}
		}
		return *this;

	}
	T& operator*()
	{
		if (nullptr != m_ptr)
		{
			return *m_ptr;
		}
		else
		{
			throw std::runtime_error("异常");
		}
	}
	T* operator->()
	{
		return m_ptr;
	}
	~MySharedPtr()
	{
		release();
	}

private:
	void release()
	{
		if (nullptr != m_count)
		{
			--(*m_count);
			if (0 == (*m_count))
			{
				delete m_ptr;
				delete m_count;
			}
		}
		m_ptr = nullptr;
		m_count = nullptr;
	}
	T* m_ptr;
	int* m_count;
};