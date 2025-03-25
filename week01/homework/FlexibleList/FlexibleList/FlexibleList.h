#pragma once
#include <memory>
#include <initializer_list>

class IBaseElement
{
public:
	virtual ~IBaseElement() = default;
	virtual std::unique_ptr<IBaseElement> clone() const = 0;
};

//为每个类或结构体都配备了无参 左右参 左右拷贝 运算符=左右重载 析构 
template <typename T>
class DataElement : public IBaseElement
{
public:
	DataElement();
	explicit DataElement(const T& val);
	explicit DataElement(T&& val);
	DataElement(const DataElement& other);
	DataElement(DataElement&& other);
	DataElement& operator=(const DataElement& other);
	DataElement& operator=(DataElement&& other);
	std::unique_ptr<IBaseElement> clone() const override;
	~DataElement();
	const T& getValue() const;
private:
	T m_value; // todo : 这里使用std::unique_ptr<T> m_value，更好还是更坏？
};


class FlexibleList
{
public:
	FlexibleList();
	FlexibleList(const FlexibleList& other);
	FlexibleList(FlexibleList&& other);
	FlexibleList& operator=(const FlexibleList& other);
	FlexibleList& operator=(FlexibleList&& other);
	template <typename T>
	FlexibleList(std::initializer_list<T> init);
	~FlexibleList();
	template <typename T>
	void pushBack(const T& val);

private:
	void destory();
	void deepCopy(const FlexibleList& other);
	struct Node
	{
		std::unique_ptr<IBaseElement> m_data;
		Node* m_next; // todo : 也许可以用一个shared_ptr指向pre，一个weak_ptr指向nxt
		Node* m_prve;
		Node();
		template <typename T>
		explicit Node(const T& val);
		template <typename T>
		explicit Node(T&& val);

		Node(const Node& other); // 拷贝构造函数不能被模板化，切记！
		Node(Node&& other);
		Node& operator=(const Node& other);
		Node& operator=(Node&& other);
		
	};
	Node* m_head;
	Node* m_tail;
	size_t m_size;
};

template <typename T>
void FlexibleList::pushBack(const T& val)
{
	FlexibleList::Node* node = new Node(val);
	m_tail->m_next = node;
	node->m_prve = m_tail;
	m_size++;
	m_tail = node;
}

template <typename T>
FlexibleList::Node::Node(const T& val)
{
	//m_data = std::make_unique<DataElement<std::decay_t<T>>>(std::forward<T>(val)); //decay_t意为类型退化，比如传入const int& val，T就代表const int&，但我们想让T为int,所以使用decay_t将前者退化为后者
	m_data = std::make_unique<DataElement<T>>(std::forward<T>(val));
	m_next = nullptr;
	m_prve = nullptr;
}

FlexibleList::Node::Node(const Node& other) : m_data(other.m_data == nullptr ? nullptr: other.m_data->clone()), m_next(nullptr), m_prve(nullptr) {}

template <typename T>
const T& DataElement<T>::getValue() const
{
	return m_value;
}

template <typename T>
std::unique_ptr<IBaseElement> DataElement<T>::clone() const
{
	return std::make_unique<DataElement>(*this);
}

template <typename T>
FlexibleList::FlexibleList(std::initializer_list<T> init)
{
	for (const T& val : init)
	{
		pushBack(val);
	}
}

template <typename T>
FlexibleList::Node::Node(T&& val)
{
	//m_data = std::make_unique<DataElement<std::decay_t<T>>>(std::forward<T>(val));
	m_data = std::make_unique<DataElement<T>>(std::forward<T>(val));
	m_next = nullptr;
	m_prve = nullptr;
}
template <typename T>
DataElement<T>& DataElement<T>::operator=(const DataElement<T>& other)
{
	if (&other != this)
		m_value = other.m_value;
	return *this;
}

template <typename T>
DataElement<T>& DataElement<T>::operator=(DataElement<T>&& other)
{
	if (&other != this)
		m_value = std::move(other.m_value);
	return *this;
}

template <typename T>
DataElement<T>::DataElement() {}

template <typename T>
DataElement<T>::DataElement(const T& val) : m_value(val) {}

template <typename T>
DataElement<T>::DataElement(T&& val) : m_value(std::move(val)) {}

template<typename T>
DataElement<T>::DataElement(const DataElement& other) : m_value(other.m_value) {}

template <typename T>
DataElement<T>::DataElement(DataElement<T>&& other) : m_value(std::move(other.m_value)) {}

template <typename T>
DataElement<T>::~DataElement() {};


