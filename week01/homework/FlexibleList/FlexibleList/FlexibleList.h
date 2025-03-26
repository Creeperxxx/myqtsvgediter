#pragma once
#include <memory>
#include <cstdlib>
#include <initializer_list>

// todo : noexcept 是否有函数缺少该关键字

//3.27 : FlexibleList重载==运算符开始写

class IBaseElement
{
public:
	virtual ~IBaseElement() noexcept = default;
	virtual std::unique_ptr<IBaseElement> clone() const noexcept = 0;
	virtual bool isEqual(const IBaseElement& other) const noexcept = 0;
};

//为每个类或结构体都配备了无参 左右参 左右拷贝 运算符=左右重载 析构 
//可以为每个函数都添加noexcept关键字，然后如果会抛出异常则去掉
template <typename T>
class DataElement : public IBaseElement
{
public:
	DataElement() noexcept ;
	//explicit要加，防止隐式类型转换，我觉得自定义类的隐式类型转换其实就是调用隐式的调用有参构造函数吧，例如person a = 5等价于person a = person(5);
	explicit DataElement(const T& val) noexcept ;
	explicit DataElement(T&& val) noexcept ;
	//explicit DataElement(T& val) noexcept;
	DataElement(const DataElement& other) noexcept ;
	DataElement(DataElement&& other) noexcept ;
	DataElement& operator=(const DataElement& other) noexcept ;
	DataElement& operator=(DataElement&& other) noexcept ;
	std::unique_ptr<IBaseElement> clone() const noexcept override;
	~DataElement() noexcept ;
	const T& getValue() const noexcept ;
	bool isEqual(const IBaseElement& other) const noexcept override;
private:
	T m_value; // todo : 这里使用std::unique_ptr<T> m_value，更好还是更坏？
};


class FlexibleList
{
private:
	struct Node;
public:
	class Iterator;
	FlexibleList() noexcept ;
	FlexibleList(const FlexibleList& other) noexcept ;
	FlexibleList(FlexibleList&& other) noexcept ;
	FlexibleList& operator=(const FlexibleList& other) noexcept ;
	FlexibleList& operator=(FlexibleList&& other) noexcept ;
	template <typename T>
	FlexibleList(std::initializer_list<T> init) noexcept ;
	~FlexibleList() noexcept ;

	template <typename T>
	void pushBack(const T& val);
	size_t size() const noexcept;
	bool empty() const noexcept;

	template <typename T>
	T& frontElement() noexcept;
	template <typename T>
	T& backElement() noexcept;
	template <typename T>
	const T& frontElementConst() const noexcept;
	template <typename T>
	const T& backElementConst() const noexcept;
	void clear() noexcept;
	FlexibleList::Iterator erase(FlexibleList::Iterator it) noexcept;
	template <typename T>
	void insertFrontIt(FlexibleList::Iterator it, const T& val) noexcept;
	template <typename T>
	T popBack() ;
	Iterator begin() const noexcept;
	Iterator end() const noexcept;
	const Iterator& cbegin() const noexcept;
	const Iterator& cend() const noexcept;

	bool operator==(const FlexibleList& other) const noexcept;
	bool operator!=(const FlexibleList& other) const noexcept;
	void swap(FlexibleList& other) noexcept;


	Iterator emplaceBack();


	class Iterator
	{
	public:
		Iterator() noexcept ;
		//Iterator(const FlexibleList::Node* node);//不能加const修饰，不然会变成常迭代器
		explicit Iterator(FlexibleList::Node* node) noexcept ;
		Iterator(const Iterator& other) noexcept ;
		Iterator(Iterator&& other) noexcept ;
		Iterator& operator=(const Iterator& other) noexcept ;
		Iterator& operator=(Iterator&& other) noexcept;
		~Iterator() noexcept ;

		FlexibleList::Node& operator*() const noexcept;
		FlexibleList::Node* operator->() const noexcept;
		
		Iterator& operator++() noexcept;
		Iterator operator++(int) noexcept;
		Iterator& operator--() noexcept;
		Iterator operator--(int) noexcept;

		bool operator==(const Iterator& other) const noexcept;
		bool operator!=(const Iterator& other) const noexcept;

		Iterator& operator+=(int n) noexcept;
		Iterator& operator-=(int n) noexcept;
		Iterator operator-(int n) noexcept;
		Iterator operator+(int n) noexcept;
		int operator-(const Iterator& other) const noexcept;

		bool isValid() const noexcept;
		void destory() noexcept;

		Iterator getPrev() const noexcept;
		Iterator getNext() const noexcept;
		void setPrev(const Iterator& prev) noexcept;
		void setNext(const Iterator& next) noexcept;
		void setNext(Node* node) noexcept;
		void setPrev(Node* node) noexcept;

	private:
		FlexibleList::Node* m_node;
	};



private:
	void destory();
	void deepCopy(const FlexibleList& other);
	struct Node
	{
		std::unique_ptr<IBaseElement> m_data;
		Node* m_next; // todo : 也许可以用一个shared_ptr指向pre，一个weak_ptr指向nxt
		Node* m_prve;
		Node() noexcept ;
		template <typename T>
		explicit Node(const T& val) noexcept ;
		template <typename T>
		explicit Node(T&& val) noexcept ;

		Node(const Node& other) noexcept ; // 拷贝构造函数不能被模板化，切记！
		Node(Node&& other) noexcept ;
		Node& operator=(const Node& other) noexcept ;
		Node& operator=(Node&& other) noexcept ;

		bool operator==(const Node& other) const noexcept;
		bool operator!=(const Node& other) const noexcept;
		
	};
	Node* m_head;
	Node* m_tail;
	size_t m_size;
	Iterator m_headIt;
	Iterator m_tailIt;
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
FlexibleList::Node::Node(const T& val) noexcept 
{
	// 我去，这里std::forward<T>(val)是错的，应该是std::forward<const T>(val)，查了好久
	// 二改，在学习了一上午万能引用和完美转发后，终于搞明白了。这里应该是std::forward<T>(val)，val有三种类型，const T&,T&,T&&,DataElement需要提供以上三种构造。  后面发现不对，const T&的构造包含了T&，前者可以传入普通左值和常量左值，右者只能传入普通左值。
	// 三改，彻底搞明白了。首先，完美转发是配合万能引用T&&一起使用的，这里本身用的不合适，其次，无论传入val的是左值引用，还是常量左值引用，还是右值引用，val的类型都与其
	//无关，因为val的类型是由const T&决定的，它是个常量左值引用，该引用可以绑定任何参数（左值，常量左值，右值），其中T被推导为原始类型（也就是去掉const和引用）。
	// 所以对于这个函数而言，第一，不需要完美转发，第二，就算使用完美转发，也应该是std::forward<const T>(val)，保障其val的const属性。
	//m_data = std::make_unique<DataElement<std::decay_t<T>>>(std::forward<const T>(val));//这种也可以，不过就像之前说的这里不是万能引用没必要用完美转发，虽说用了可以和那个构造函数保持一致看着舒服
	m_data = std::make_unique<DataElement<std::decay_t<T>>>(val);
	//m_data = std::make_unique<DataElement<std::decay_t<T>>>(std::forward<T>(val)); //decay_t意为类型退化，例如传入数组，那么T就代表数组的类型，需要退化到单个元素的指针类型。
	//m_data = std::make_unique<DataElement<T>>(std::forward<T>(val));
	m_next = nullptr;
	m_prve = nullptr;
}


template <typename T>
const T& DataElement<T>::getValue() const noexcept 
{
	return m_value;
}

template <typename T>
std::unique_ptr<IBaseElement> DataElement<T>::clone() const noexcept 
{
	return std::make_unique<DataElement>(*this);
}

template <typename T>
FlexibleList::FlexibleList(std::initializer_list<T> init) noexcept 
{
	for (const T& val : init)
	{
		pushBack(val);
	}
}

template <typename T>
FlexibleList::Node::Node(T&& val) noexcept 
{
	m_data = std::make_unique<DataElement<std::decay_t<T>>>(std::forward<T>(val));//使用了万能引用T&&，需要用的完美转发，
	//m_data = std::make_unique<DataElement<T>>(std::forward<T>(val));
	m_next = nullptr;
	m_prve = nullptr;
}
template <typename T>
DataElement<T>& DataElement<T>::operator=(const DataElement<T>& other) noexcept 
{
	if (&other != this)
		m_value = other.m_value;
	return *this;
}

template <typename T>
DataElement<T>& DataElement<T>::operator=(DataElement<T>&& other) noexcept 
{
	if (&other != this)
		m_value = std::move(other.m_value);
	return *this;
}

template <typename T>
DataElement<T>::DataElement()  noexcept {}

template <typename T>
DataElement<T>::DataElement(const T& val)  noexcept : m_value(val) {}

template <typename T>
DataElement<T>::DataElement(T&& val) noexcept : m_value(std::move(val)) {}

template<typename T>
DataElement<T>::DataElement(const DataElement& other)  noexcept : m_value(other.m_value) {}

template <typename T>
DataElement<T>::DataElement(DataElement<T>&& other) noexcept : m_value(std::move(other.m_value)) {}

template <typename T>
DataElement<T>::~DataElement() noexcept  {}

//template <typename T>
//DataElement<T>::DataElement(T& val) :m_value(val) {}

template<typename T>
bool DataElement<T>::isEqual(const IBaseElement& other) const noexcept
{
	try
	{
		const DataElement<T>& transfromOther = dynamic_cast<const DataElement<T>&>(other);
		return transfromOther.getValue() == getValue();
	}
	catch (const std::bad_cast&)
	{
		return false;
	}
}
