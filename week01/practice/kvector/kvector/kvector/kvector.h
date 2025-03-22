#pragma once

template <typename T>
//kvector中的接口需要大家自己来实现
class KVector
{
public:
    KVector(int len = 10) //这里的10为默认长度
    {
        m_cap = 10;
        m_data = new t[len];
        start = m_data;
        pos = 0;
    }
    ~KVector()
    {
        delete[] m_data;
    }
    void push_back(const T& val)
    {
        if (pos > m_cap - 1)
        {
            return;
        }
        m_data[pos] = val;
        pos++;
    }
    T pop_back()
    {
        if (pos == 0)
        {
            return T();
        }
        
        pos--;
        return m_data[pos];
    }
    int size() const
    {
        return pos;
    }
    int getCap() const//返回m_cap的值
    {
        return m_cap;
    }
    T* begin()
    {
        return start;
    }
    T* end()
    {
        return start + pos - 1;
    }

    T operator[] (int n)
    {
        if (n < 0 || n > pos - 1)
        {
            return m_data[0];
        }
        return m_data[n];
    }
    void emplace_back(const T& val)
    {
        if (pos > m_cap - 1)
        {
            return;
        }
        m_data[pos] = val;
        pos++;
    }

protected:
    T* m_data;   //申请数组堆区空间的首地址
    T* start;    //保存数组的起始位置
    int m_cap;         //数组的长度
    int pos;           //下一个数据要插入的位置,
};
