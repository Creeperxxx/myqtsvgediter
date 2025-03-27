#pragma once
#include <memory>
#include <cstdlib>
#include <initializer_list>
#include <stdexcept>
#include <functional>

// todo : noexcept 是否有函数缺少该关键字

//3.27 : FlexibleList重载==运算符开始写

class IBaseElement
{
public:
	virtual ~IBaseElement() noexcept = default;
	virtual std::unique_ptr<IBaseElement> clone() const noexcept = 0;
	virtual bool isEqual(const IBaseElement& other) const = 0;
	virtual bool less(const IBaseElement& other) const = 0;
};

//为每个类或结构体都配备了无参 左右参 左右拷贝 运算符=左右重载 析构 
//可以为每个函数都添加noexcept关键字，然后如果会抛出异常则去掉
template <typename T>
class DataElement : public IBaseElement
{
public:
	DataElement();
	//explicit要加，防止隐式类型转换，我觉得自定义类的隐式类型转换其实就是调用隐式的调用有参构造函数吧，例如person a = 5等价于person a = person(5);
	explicit DataElement(const T& val);
	explicit DataElement(T&& val);
	//还得加模板参数包构造，毕竟T还可以是接收构造参数的自定义类
	template <typename... Args>
	explicit DataElement(Args&&... args) noexcept;

	//explicit DataElement(T& val) noexcept;
	DataElement(const DataElement& other);
	DataElement(DataElement&& other);
	DataElement& operator=(const DataElement& other);
	DataElement& operator=(DataElement&& other);
	std::unique_ptr<IBaseElement> clone() const noexcept override;
	~DataElement() noexcept;

	const T* getValuePoint() const;
	const T& getValue() const;
	bool isEqual(const IBaseElement& other) const override;
	bool less(const IBaseElement& other) const override;
private:
	//T m_value; // todo : 这里可以使用std::unique_ptr<T> m_value
	std::unique_ptr<T> m_value;
};


class FlexibleList
{
private:
	class Node;
public:
	class Iterator;
	FlexibleList() noexcept;
	FlexibleList(const FlexibleList& other) noexcept;
	FlexibleList(FlexibleList&& other) noexcept;
	FlexibleList& operator=(const FlexibleList& other) noexcept;
	FlexibleList& operator=(FlexibleList&& other) noexcept;
	template <typename T>
	FlexibleList(std::initializer_list<T> init) noexcept;
	~FlexibleList() noexcept;

	template <typename T>
	void pushBack(const T& val);
	template <typename T>
	void pushBack(T&& val);
	size_t size() const noexcept;
	bool empty() const noexcept;

	//template <typename T>
	//T& frontElement() noexcept; // 这里不好弄啊，T不知道有没有默认构造函数，所以T以指针存储而不是变量，但是这又导致无法返回引用，毕竟T*可以为nullptr，只能返回指针然后由用户解引用了
	//template <typename T>
	//T& backElement() noexcept;
	//template <typename T>
	//const T& frontElementConst() const noexcept;
	//template <typename T>
	//const T& backElementConst() const noexcept;
	template <typename T>
	const T* frontElement() noexcept;
	template <typename T>
	const T* backElement() noexcept;
	FlexibleList::Iterator erase(FlexibleList::Iterator it) noexcept;
	template <typename T>
	void insertFrontIt(FlexibleList::Iterator it, const T& val) noexcept;
	template <typename T>
	void popBack();
	Iterator begin() const noexcept;
	Iterator back() const noexcept;
	Iterator end() const noexcept;
	const Iterator cbegin() const noexcept;
	const Iterator cback() const noexcept;

	bool operator==(const FlexibleList& other) const noexcept;
	bool operator!=(const FlexibleList& other) const noexcept;
	void swap(FlexibleList& other) noexcept;
	void swapIterator(Iterator& a, Iterator& b);

	template <typename T, typename... Args>
	Iterator emplaceBack(Args&&... args);

	void setBeginNode(Node* node);
	void setBackNode(Node* node);

	void setHeadPoint(Node* node);
	void setEndPoint(Node* node);

	template <typename Compare>
	void FlexibleListSort(Compare comp);

	class Iterator
	{
	public:
		Iterator() noexcept;
		Iterator(const FlexibleList* container);
		//Iterator(const FlexibleList::Node* node);//不能加const修饰，不然会变成常迭代器
		explicit Iterator(FlexibleList::Node* node, const FlexibleList* container) noexcept;
		Iterator(const Iterator& other) noexcept;
		Iterator(Iterator&& other) noexcept;
		Iterator& operator=(const Iterator& other) noexcept;
		Iterator& operator=(Iterator&& other) noexcept;
		~Iterator() noexcept;

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
		//int operator-(const Iterator& other) const noexcept;  //计算迭代器之间距离麻烦

		bool operator<(const Iterator& other) const ;
		bool operator<=(const Iterator& other) const ;
		bool operator>(const Iterator& other) const ;
		bool operator>=(const Iterator& other) const ;


		bool isValid() const noexcept;
		void destory() noexcept;

		Iterator getPrev() const noexcept;
		Iterator getNext() const noexcept;
		void setPrev(const Iterator& prev) noexcept;
		void setNext(const Iterator& next) noexcept;
		void setNext(FlexibleList::Node* node) noexcept;
		void setPrev(FlexibleList::Node* node) noexcept;

		template <typename T>
		const T* getValuePoint() const;

		const FlexibleList* getContainerPoint() const;
		bool isSameContainer(const Iterator& other) const;

		Node* getNodePoint() const;

	private:
		FlexibleList::Node* m_node;
		const FlexibleList* m_container; //得确认两个迭代器来自统一容器

	};


private:
	void clear();
	void deepCopy(const FlexibleList& other);
	void swapNode(Node* a, Node* b);
	void insertNodeEnd(Node* n);
	void destory() noexcept;

	class Node //节点node就相当于是对底层数据的一层封装吧，FlexibleList不应该直接和数据T交互，而是通过node和T进行交互，也就是FlexibleList->node->T,FlexibleList不应该能够接触T
	{
	public:
		Node() noexcept;
		template <typename T>
		explicit Node(const T& val) noexcept;
		template <typename T>
		explicit Node(T&& val) noexcept;
		template <typename T, typename... Args>
		explicit Node(Args&&... args);

		Node(const Node& other) noexcept; // 拷贝构造函数不能被模板化，切记！
		Node(Node&& other) noexcept;
		Node& operator=(const Node& other) noexcept;
		Node& operator=(Node&& other) noexcept;

		bool operator==(const Node& other) const noexcept;
		bool operator!=(const Node& other) const noexcept;

		bool operator<(const Node& other) const noexcept;
		bool operator>(const Node& other) const noexcept;
		bool operator<=(const Node& other) const noexcept;
		bool operator>=(const Node& other) const noexcept;

		template <typename T>
		//const T& getValue() const noexcept;
		const T* getValuePoint() const;

		const Node* getConstNext() const;
		const Node* getConstPrev() const;
		Node* getNext();
		Node* getPrev();

		void setNext(Node* n);
		void setPrev(Node* n);

		void swap(Node* n);

	private:
		std::unique_ptr<IBaseElement> m_data;
		Node* m_next; // todo : 也许可以用一个shared_ptr指向pre，一个weak_ptr指向nxt
		Node* m_prve;
	};
	Node* m_head;
	Node* m_tail;
	size_t m_size;
};

template <typename T>
void FlexibleList::pushBack(const T& val)
{
	FlexibleList::Node* node = new Node(val);
	insertNodeEnd(node);
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
//const T& DataElement<T>::getValue() const 
const T* DataElement<T>::getValuePoint() const
{
	try
	{
		return m_value.get();
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
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
DataElement<T>& DataElement<T>::operator=(const DataElement<T>& other)
{
	if (&other != this)
		m_value = std::make_unique<T>(*(other.m_value));
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
DataElement<T>::DataElement()
{
	try
	{
		m_value = std::make_unique<T>(); // 没有默认构造就返回nullptr吧
	}
	catch (const std::exception& e)
	{
		m_value = nullptr;
	}
}

template <typename T>
DataElement<T>::DataElement(const T& val) : m_value(std::make_unique<T>(val)) {}

template <typename T>
DataElement<T>::DataElement(const DataElement& other) : m_value(std::make_unique<T>(*(other.m_value))) {}

template <typename T>
DataElement<T>::DataElement(T&& val) : m_value(std::make_unique<T>(std::forward<T>(val))) {}

template <typename T>
DataElement<T>::DataElement(DataElement<T>&& other) : m_value(std::move(other.m_value)) {}

template <typename T>
DataElement<T>::~DataElement() noexcept {}

//template <typename T>
//DataElement<T>::DataElement(T& val) :m_value(val) {}


template<typename T>
bool DataElement<T>::isEqual(const IBaseElement& other) const
{
	try
	{
		const DataElement<T>& transfromOther = dynamic_cast<const DataElement<T>&>(other);
		const T* otherValuePoint = transfromOther.getValuePoint();
		const T* myValuePoint = getValuePoint();
		if (!otherValuePoint || !myValuePoint)
			return !otherValuePoint && !myValuePoint;
		return *otherValuePoint == *myValuePoint;
	}
	catch (const std::bad_cast&)
	{
		return false;
	}
}
template <typename T>
template <typename... Args>
DataElement<T>::DataElement(Args&&... args) noexcept : m_value(std::make_unique<T>(std::forward<Args>(args)...)) {};

template <typename T, typename... Args>
FlexibleList::Node::Node(Args&&... args) : m_data(std::make_unique<DataElement<T>>(std::forward<Args>(args)...)), m_next(nullptr), m_prve(nullptr) {};

template <typename T, typename... Args>
FlexibleList::Iterator FlexibleList::emplaceBack(Args&&... args)
{
	Node* node = new Node(std::forward<Args>(args)...);
	node->m_prve = m_tail;
	m_tail->m_next = node;
	m_tail = node;
	m_size++;
	return Iterator(node, this);
}

template <typename T>
const T* FlexibleList::Node::getValuePoint() const
{
	if (nullptr == m_data)
	{
		return nullptr;
	}
	const DataElement<T>* p = dynamic_cast<const DataElement<T>*>(m_data.get());
	if (nullptr == p)
	{
		return nullptr;
	}
	else
	{
		return p->getValuePoint();
	}
	//try
	//{
	//	if (m_data == nullptr)
	//	{
	//
	//		return nullptr;
	//	}
	//	const T* p = (dynamic_cast<const DataElement<T>*>(m_data.get()))->getValuePoint();
	//	return p;
	//}
	//catch (const std::exception&)
	//{
	//	return nullptr;
	//}
}

template <typename T>
const T* FlexibleList::Iterator::getValuePoint() const
{
	if (true == isValid())
	{
		return (*m_node).getValuePoint<T>();
	}
	else
	{
		return nullptr;
	}
}

template <typename T>
const T& DataElement<T>::getValue() const
{
	try
	{
		return *m_value;
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("DataElement中value为空");
	}
}

template <typename T>
bool DataElement<T>::less(const IBaseElement& other) const
{
	try
	{
		const DataElement<T>& p = dynamic_cast<const DataElement<T>&>(other);
		return *m_value < *(p.m_value);
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("转换失败");
	}
}

template <typename T>
void FlexibleList::pushBack(T&& val)
{
	Node* newNode = new Node(std::forward<T>(val));
	insertNodeEnd(newNode);
}

template <typename Compare>
void FlexibleList::FlexibleListSort(Compare comp)
{
	if (size() <= 1) return;

	std::function<FlexibleList::Node*(FlexibleList::Node*, FlexibleList::Node*)> merge = [comp,this](FlexibleList::Node* a, FlexibleList::Node* b) -> FlexibleList::Node*
	{
		FlexibleList::Node dummy;
		FlexibleList::Node* tail = &dummy;

		while (a && b) 
		{
			if (comp(Iterator(a, this),Iterator(b, this))) 
			{
				tail->setNext(a);
				a->setPrev(tail);
				tail = a;
				a = a->getNext();
			}
			else 
			{
				tail->setNext(b);
				b->setPrev(tail);
				tail = b;
				b = b->getNext();
			}
		}
		tail->setNext(a ? a: b);
		return dummy.getNext();
	};

	size_t block_size = 1;
	while (true) 
	{
		FlexibleList::Node* current = begin().getNodePoint();
		FlexibleList::Node* next_block = current;
		FlexibleList::Node dummy_head;
		FlexibleList::Node* tail = &dummy_head;
		size_t merged_count = 0;

		while (current) 
		{
			FlexibleList::Node* left = current;
			for (size_t i = 1; i < block_size && current->getNext(); ++i)
				current = current->getNext();

			FlexibleList::Node* right = current->getNext();
			current->setNext(nullptr);
			if (right)
				right->setPrev(nullptr);
			current = right;

			for (size_t i = 1; i < block_size && current; ++i)
				current = current->getNext();

			FlexibleList::Node* next = current ? current->getNext() : nullptr;
			if (current) current->setNext(nullptr);

			FlexibleList::Node* merged = merge(left, right);
			tail->setNext(merged);
			merged->setPrev(tail);
			while (tail->getNext())
				tail = tail->getNext();

			current = next;
			++merged_count;
		}

		if (merged_count <= 1) 
			break;
		block_size *= 2;
		setHeadPoint(dummy_head.getNext());
		dummy_head.getNext()->setPrev(nullptr);
		setEndPoint(tail);
	}
}
