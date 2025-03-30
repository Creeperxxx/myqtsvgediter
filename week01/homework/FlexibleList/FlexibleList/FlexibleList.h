#pragma once
#include <memory>
#include <cstdlib>
#include <initializer_list>
#include <stdexcept>
#include <functional>
#include <type_traits>
#include <utility>
#include <typeindex>

constexpr bool FLEXIBLELISTSORTCOMPPARAMTYPEBEFORE = true;  //这个是决定非比较类型的数据是放在容器前面还是后面
constexpr bool FLEXIBLELISTSORTCOMPNONPARAMTYPEORDER = true; //这个是决定两个数据都是非比较类型时如何排序的，是按原顺序还是非原顺序
constexpr bool FLEXIBLELISTSORTCOMPASC = false; //这个只是方便我看排列顺序的，降序还是逆序


//#include <tuple>

// todo : noexcept 是否有函数缺少该关键字

//3.27 : FlexibleList重载==运算符开始写

class IBaseElement
{
public:
	virtual ~IBaseElement() noexcept = default;
	virtual std::unique_ptr<IBaseElement> clone() const noexcept = 0;
	virtual bool isEqual(const IBaseElement& other) const = 0;
	virtual bool less(const IBaseElement& other) const = 0;
	virtual const std::type_index& getTypeIndex() const = 0;

};

//为每个类或结构体都配备了无参 左右参 左右拷贝 运算符=左右重载 析构 
//可以为每个函数都添加noexcept关键字，然后如果会抛出异常则去掉
template <typename T>
class DataElement : public IBaseElement
{
public:
	//DataElement(); //这个模板类就不能提供默认构造的
	//explicit要加，防止隐式类型转换，我觉得自定义类的隐式类型转换其实就是调用隐式的调用有参构造函数吧，例如person a = 5等价于person a = person(5);
	//explicit DataElement(const T& val); //这个没必要提供了
	//explicit DataElement(T&& val); //这个不是万能引用，因为T的类型不需要推导了已经明确了
	template <typename U>
	explicit DataElement(U&& value);
	//还得加模板参数包构造，毕竟T还可以是接收构造参数的自定义类
	template <typename... Args>
	explicit DataElement(Args&&... args);

	//explicit DataElement(T& val) noexcept;
	DataElement(const DataElement& other);
	DataElement(DataElement&& other);
	DataElement& operator=(const DataElement& other);
	DataElement& operator=(DataElement&& other);
	std::unique_ptr<IBaseElement> clone() const noexcept override;
	~DataElement() noexcept;

	//const T* getValuePoint() const;
	const T& getValue() const;
	T& getValueReference();
	bool isEqual(const IBaseElement& other) const override;
	bool less(const IBaseElement& other) const override;
	const std::type_index& getTypeIndex() const override;

	//记住观察node的构造函

	//void updateValue(T&& value);
	template <typename U>
	void updateValue(U&& value);
private:
	//T m_value; // todo : 这里可以使用std::unique_ptr<T> m_value
	//std::unique_ptr<T> m_value;  //当初这里用unique_ptr，主要是因为返回value时value可能为空，所以就像用指针代替变量，但后来一想，
							     //当创建一个DataElement对象时,是因为要创建一个node节点，而创建一个node节点是因为有值要push进容器里，
								 //换句话说，只要dataElement对象存储，那么值是定然存在的根本不需要考虑value为空，所以可以直接存储值对象

	T m_tempvalue; //之前对于这里一直有个担心，就是在node中构造dataElement时，T的类型是通过node传入的参数推导的，如果参数带const，那么T推导时岂不是
				   //也带const，这样就不能updatevalue，而且两个dataElement因为const而导致类型不同也难受，后来发现decay_t就能解决这个问题
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

	//template <typename T>
	//void pushBack(const T& val);//有万能引用就不需要这个了
	template <typename T>
	void pushBack(T&& val);
	size_t size() const noexcept;
	bool empty() const noexcept;

	//template <typename T>
	//T& frontElement() noexcept; // 这里不好弄啊，T不知道有没有默认构造函数，所以T以指针存储而不是变量，但是这又导致无法返回引用，毕竟T*可以为nullptr，只能返回指针然后由用户解引用了
	//template <typename T>       // 如果要返回T&的话就得使用异常，但编码规范又不让用异常
	//T& backElement() noexcept;
	//template <typename T>
	//const T& frontElementConst() const noexcept;
	//template <typename T>
	//const T& backElementConst() const noexcept;
	template <typename T>
	//const T* frontElement() noexcept;
	const T& frontData();
	template <typename T>
	const T& backData();
	//const T* backElement() noexcept;
	FlexibleList::Iterator erase(FlexibleList::Iterator& it);
	template <typename T>
	//void insertFrontIt(FlexibleList::Iterator it, const T& val) noexcept;
	Iterator insertFrontIt(FlexibleList::Iterator &it, T&& val);
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

	bool isSameTypeElement(const Iterator& a, const Iterator& b) const;
	bool isMyIterator(const Iterator& it) const noexcept;

	//迭代器
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

		//FlexibleList::Node& operator*() const noexcept;
		template <typename T>
		T& operator*();
		//FlexibleList::Node* operator->() const noexcept;
		template <typename T>
		T* operator->();
		//const T& operator->();

		Iterator& operator++() ;
		Iterator operator++(int) ;
		Iterator& operator--() ;
		Iterator operator--(int) ;

		bool operator==(const Iterator& other) const noexcept;
		bool operator!=(const Iterator& other) const noexcept;

		Iterator& operator+=(int n) noexcept;
		Iterator& operator-=(int n) noexcept;
		Iterator operator-(int n) noexcept;
		Iterator operator+(int n) noexcept;
		//int operator-(const Iterator& other) const noexcept;  //计算迭代器之间距离麻烦

		bool operator<(const Iterator& other) const;
		bool operator<=(const Iterator& other) const;
		bool operator>(const Iterator& other) const;
		bool operator>=(const Iterator& other) const;


		bool isValid() const noexcept;
		void deleteElement() noexcept;

		Iterator getPrev() const noexcept;
		Iterator getNext() const noexcept;
		void setPrev(const Iterator& prev) noexcept;
		void setNext(const Iterator& next) noexcept;
		void setNext(FlexibleList::Node* node)noexcept;
		void setPrev(FlexibleList::Node* node)noexcept;

		//template <typename T>
		//const T* getValuePoint() const;

		const FlexibleList* getContainerPoint() const;
		bool isSameContainer(const Iterator& other) const;

		Node* getNodePoint() const;
		template <typename T>
		const T& getValue()const;

		const std::type_index& getValueTypeIndex() const;
		
		template <typename T>
		bool updateData(T&& value);

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




	//Node
	class Node //节点node就相当于是对底层数据的一层封装吧，FlexibleList不应该直接和数据T交互，而是通过node和T进行交互，也就是FlexibleList->node->T,FlexibleList不应该能够接触T
	{
	public:
		Node() noexcept;
		//template <typename T>
		//explicit Node(const T& val) noexcept; //这个没有必要
		template <typename T>
		explicit Node(T&& val) noexcept; //用这个万能引用就好了
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

		//template <typename T>
		//const T& getValue() const noexcept;
		//const T* getValuePoint() const;
		template <typename T>
		const T& getValue() const;
		template <typename T>
		T& getValueReference();

		const Node* getConstNext() const;
		const Node* getConstPrev() const;
		Node* getNext();
		Node* getPrev();

		void setNext(Node* n);
		void setPrev(Node* n);

		void swap(Node* n);

		bool isSameNodeTotally(const Node* node) const;
		bool isSameNodeTotally(const Node& other) const;
		bool isSameDataType(const Node& other) const;
		const std::type_index& getDataTypeIndex() const;

		template <typename T>
		bool updateData(T&& value);

	private:
		std::unique_ptr<IBaseElement> m_data;
		Node* m_next; // todo : 也许可以用一个shared_ptr指向pre，一个weak_ptr指向nxt
		Node* m_prve;
	};


	Node* m_head;
	Node* m_tail;
	size_t m_size;
};

//template <typename T>
//void FlexibleList::pushBack(const T& val)
//{
//	FlexibleList::Node* node = new Node(val);
//	insertNodeEnd(node);
//}

//这个构造函数没有必要的，可以使用万能引用那个构造函数代替
//template <typename T>
//FlexibleList::Node::Node(const T& val) noexcept
//{
//	//m_data = std::make_unique<DataElement<std::decay_t<T>>>(std::forward<const T>(val));
//	m_data = std::make_unique<DataElement<std::decay_t<T>>>(val);
//	//m_data = std::make_unique<DataElement<std::decay_t<T>>>(std::forward<T>(val)); //decay_t意为类型退化，例如传入数组，那么T就代表数组的类型，需要退化到单个元素的指针类型。
//	//m_data = std::make_unique<DataElement<T>>(std::forward<T>(val));
//	m_next = nullptr;
//	m_prve = nullptr;
//}


//template <typename T>
////const T& DataElement<T>::getValue() const 
//const T* DataElement<T>::getValuePoint() const
//{
//	try
//	{
//		return m_value.get();
//	}
//	catch (const std::exception&)
//	{
//		return nullptr;
//	}
//}

template <typename T>
std::unique_ptr<IBaseElement> DataElement<T>::clone() const noexcept
{
	return std::make_unique<DataElement>(*this);
}

template <typename T>
FlexibleList::FlexibleList(std::initializer_list<T> init) noexcept : m_head(new Node()), m_size(0)
{
	m_tail = m_head;
	for (auto&& val : init)
	{
		pushBack(std::move(val));
	}
}

template <typename T>
FlexibleList::Node::Node(T&& val) noexcept
{
	//这里dataElement内的m_value的参数类型推导不是靠传入的参数val决定的，而是靠尖括号内std::decay_t<T>决定的，所以即使传入const类型的值，m_value
	//也不会被推导为带const属性，因为decay_t意为类型退化，如果传入const &类型的值，T会被推导为这种类型，如果直接以T类型构造dataElement，那么
	//m_value也会带上const，我们希望m_value是T的原始类型，例如传入参数是const int& ,T被推倒为const int&，我们希望m_value是去掉const和引用的int，
	//那么就使用decay_g
	m_data = std::make_unique<DataElement<std::decay_t<T>>>(std::forward<T>(val));//使用了万能引用T&&，需要用的完美转发
	//m_data = std::make_unique<DataElement<T>>(std::forward<T>(val));
	m_next = nullptr;
	m_prve = nullptr;
}
template <typename T>
DataElement<T>& DataElement<T>::operator=(const DataElement<T>& other)
{
	if (&other != this)
		//m_value = std::make_unique<T>(*(other.m_value));
		m_tempvalue = other.m_tempvalue;
	return *this;
}

template <typename T>
DataElement<T>& DataElement<T>::operator=(DataElement<T>&& other)
{
	if (&other != this)
		//m_value = std::move(other.m_value);
		m_tempvalue = std::move(other.m_tempvalue);
	return *this;
}

//template <typename T>
//DataElement<T>::DataElement()
//{
//	//try
//	//{
//		//m_value = std::make_unique<T>(); // 没有默认构造就返回nullptr吧
//	//}
//	//catch (const std::exception& e)
//	//{
//		//m_value = nullptr;
//	//}
//}

//template <typename T>
//DataElement<T>::DataElement(const T& val) : m_value(std::make_unique<T>(val)) {}

template <typename T>
//DataElement<T>::DataElement(const DataElement& other) : m_value(std::make_unique<T>(*(other.m_value))) {}
DataElement<T>::DataElement(const DataElement& other) : m_tempvalue(other.m_tempvalue) {}

//template <typename T>
//DataElement<T>::DataElement(T&& val) : m_value(std::make_unique<T>(std::forward<T>(val))) {}
//DataElement<T>::DataElement(T&& val) : m_tempvalue(std::forward<T>(val)) {}

template <typename T>
//DataElement<T>::DataElement(DataElement<T>&& other) : m_value(std::move(other.m_value)) {}
DataElement<T>::DataElement(DataElement&& other) : m_tempvalue(std::move(other.m_tempvalue)) {}

template <typename T>
DataElement<T>::~DataElement() noexcept {}

//template <typename T>
//DataElement<T>::DataElement(T& val) :m_value(val) {}


template<typename T>
bool DataElement<T>::isEqual(const IBaseElement& other) const
{
	if (getTypeIndex() != other.getTypeIndex())
	{
		return false;
	}
	else
	{
		//const DataElement<T>* otherPoint = dynamic_cast<const DataElement<T>*>(other);
		return getValue() == (dynamic_cast<const DataElement<T>&>(other)).getValue();
	}
	//try
	//{
	//	const DataElement<T>& transfromOther = dynamic_cast<const DataElement<T>&>(other);
	//	//return getValue<T>() == transfromOther.getValue<T>();
	//	return getValue() == transfromOther.getValue();
	//	//const T* otherValuePoint = transfromOther.getValuePoint();
	//	//const T* myValuePoint = getValuePoint();
	//	//if (!otherValuePoint || !myValuePoint)
	//	//	return !otherValuePoint && !myValuePoint;
	//	//return *otherValuePoint == *myValuePoint;
	//}
	//catch (const std::bad_cast&)
	//{
	//	return false;
	//}
}
template <typename T>
template <typename... Args>
DataElement<T>::DataElement(Args&&... args) : m_tempvalue(std::forward<Args>(args)...) {}
//DataElement<T>::DataElement(Args&&... args) noexcept : m_value(std::make_unique<T>(std::forward<Args>(args)...)) {};

template <typename T, typename... Args>
FlexibleList::Node::Node(Args&&... args) : m_data(std::make_unique<DataElement<T>>(std::forward<Args>(args)...)), m_next(nullptr), m_prve(nullptr) {};

template <typename T, typename... Args>
FlexibleList::Iterator FlexibleList::emplaceBack(Args&&... args)
{
	Node* node = new Node(std::forward<Args>(args)...);
	insertNodeEnd(node);
	//node->m_prve = m_tail;
	//m_tail->m_next = node;
	//m_tail = node;
	//m_size++;
	return Iterator(node, this);
}

//template <typename T>
//const T* FlexibleList::Node::getValuePoint() const
//{
//	if (nullptr == m_data)
//	{
//		return nullptr;
//	}
//	const DataElement<T>* p = dynamic_cast<const DataElement<T>*>(m_data.get());
//	if (nullptr == p)
//	{
//		return nullptr;
//	}
//	else
//	{
//		return p->getValuePoint();
//	}
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
//}

//template <typename T>
//const T* FlexibleList::Iterator::getValuePoint() const
//{
//	if (true == isValid())
//	{
//		return (*m_node).getValuePoint<T>();
//	}
//	else
//	{
//		return nullptr;
//	}
//}

template <typename T>
const T& DataElement<T>::getValue() const
{
	return m_tempvalue;
	//try
	//{
		//return *m_value;
	//}
	//catch (const std::exception&)
	//{
		//throw std::runtime_error("DataElement中value为空");
	//}
}

template <typename T>
bool DataElement<T>::less(const IBaseElement& other) const
{
	if (getTypeIndex() != other.getTypeIndex())
	{
		return FLEXIBLELISTSORTCOMPNONPARAMTYPEORDER;
	}
	else
	{
		return m_tempvalue < (dynamic_cast<const DataElement<T>&>(other)).getValue();
	}

	//try
	//{
		//const DataElement<T>& p = dynamic_cast<const DataElement<T>&>(other);
		//return *m_value < *(p.m_value);
	//}
	//catch (const std::exception&)
	//{
		//throw std::runtime_error("转换失败,比较两个DataElement时请确认内部存储的元素类型相同");
	//}
}

template <typename T>
void FlexibleList::pushBack(T&& val)
{
	;
	insertNodeEnd(new Node(std::forward<T>(val)));
}

//template <typename Compare>
//void FlexibleList::FlexibleListSort(Compare comp)
//{
//	if (size() <= 1) return;
//
//	std::function<FlexibleList::Node*(FlexibleList::Node*, FlexibleList::Node*)> merge = [comp,this](FlexibleList::Node* a, FlexibleList::Node* b) -> FlexibleList::Node*
//	{
//		FlexibleList::Node dummy;
//		FlexibleList::Node* tail = &dummy;
//
//		while (a && b) 
//		{
//			if (comp(Iterator(a, this),Iterator(b, this))) 
//			{
//				tail->setNext(a);
//				a->setPrev(tail);
//				tail = a;
//				a = a->getNext();
//			}
//			else 
//			{
//				tail->setNext(b);
//				b->setPrev(tail);
//				tail = b;
//				b = b->getNext();
//			}
//		}
//		tail->setNext(a ? a: b);
//		return dummy.getNext();
//	};
//
//	size_t block_size = 1;
//	while (true) 
//	{
//		FlexibleList::Node* current = begin().getNodePoint();
//		FlexibleList::Node* next_block = current;
//		FlexibleList::Node dummy_head;
//		FlexibleList::Node* tail = &dummy_head;
//		size_t merged_count = 0;
//
//		while (current) 
//		{
//			FlexibleList::Node* left = current;
//			for (size_t i = 1; i < block_size && current->getNext(); ++i)
//				current = current->getNext();
//
//			FlexibleList::Node* right = current->getNext();
//			current->setNext(nullptr);
//			if (right)
//				right->setPrev(nullptr);
//			current = right;
//
//			for (size_t i = 1; i < block_size && current; ++i)
//				current = current->getNext();
//
//			FlexibleList::Node* next = current ? current->getNext() : nullptr;
//			if (current) current->setNext(nullptr);
//
//			FlexibleList::Node* merged = merge(left, right);
//			tail->setNext(merged);
//			merged->setPrev(tail);
//			while (tail->getNext())
//				tail = tail->getNext();
//
//			current = next;
//			++merged_count;
//		}
//
//		if (merged_count <= 1) 
//			break;
//		block_size *= 2;
//		m_head->setNext(dummy_head.getNext());
//		dummy_head.getNext()->setPrev(m_head);
//		setEndPoint(tail);
//	}
//}

//以下是为了获取模板函数的参数类型

// 辅助结构体用于从任意可调用对象中提取第一个参数类型
template<typename Callable, typename = void>
struct ExtractFirstParamType;

// 特化版本：针对普通函数指针或函数类型
template<typename ReturnType, typename Arg, typename... Args>
struct ExtractFirstParamType<ReturnType(*)(Arg, Args...), void> {
	using RawParamType = std::decay_t<Arg>;
};

// 特化版本：针对成员函数指针
template<typename ClassType, typename ReturnType, typename Arg, typename... Args>
struct ExtractFirstParamType<ReturnType(ClassType::*)(Arg, Args...), void> {
	using RawParamType = std::decay_t<Arg>;
};

// 特化版本：针对所有其他可调用对象（如lambda）
template<typename Callable>
struct ExtractFirstParamType<Callable, std::void_t<decltype(&Callable::operator())>> {
private:
	using CallableOperator = decltype(&Callable::operator());

	template<typename T>
	struct ExtractArg;

	template<typename Ret, typename Class, typename Arg, typename... Args>
	struct ExtractArg<Ret(Class::*)(Arg, Args...) const> {
		using type = Arg;
	};

	template<typename Ret, typename Class, typename Arg, typename... Args>
	struct ExtractArg<Ret(Class::*)(Arg, Args...)> {
		using type = Arg;
	};

public:
	using RawParamType = std::decay_t<typename ExtractArg<CallableOperator>::type>;
};


template <typename Compare>
void FlexibleList::FlexibleListSort(Compare comp) {
	using ParamType = typename ExtractFirstParamType<Compare>::RawParamType;
	std::type_index ParamTypeIndex = typeid(ParamType);

	//auto compWithTypeIndex = [&](Node* left, Node* right) -> bool // 这里就定义了ParamType型数据和非ParamType型数据的比较规则
	std::function<bool(Node*, Node*)> compWithTypeIndex = [&](Node* left, Node* right)->bool
	{
		std::type_index lTypeIndex = left->getDataTypeIndex();
		std::type_index rTypeIndex = right->getDataTypeIndex();
		if ((lTypeIndex == ParamTypeIndex) && (rTypeIndex == ParamTypeIndex))
		{
			return comp(left->getValue<ParamType>(), right->getValue<ParamType>());
		}
		else if ((lTypeIndex == ParamTypeIndex) && (rTypeIndex != ParamTypeIndex))
		{
			return FLEXIBLELISTSORTCOMPPARAMTYPEBEFORE; //认为ParamType型数据更小更大？
		}
		else if ((lTypeIndex != ParamTypeIndex) && (rTypeIndex == ParamTypeIndex))
		{
			return !FLEXIBLELISTSORTCOMPPARAMTYPEBEFORE;
		}
		else
		{
			return FLEXIBLELISTSORTCOMPNONPARAMTYPEORDER; //如果都是非ParamType型数据，则保持原样，应该是这样
		}
	};

	//auto getMiddle = [](Node* head) -> Node*
	std::function<Node* (Node*)> getMiddle = [](Node* head) -> Node*
	{
		if (!head || !head->getNext()) return head;

		Node* slow = head;
		Node* fast = head;
		while (fast->getNext() && fast->getNext()->getNext())
		{
			slow = slow->getNext();
			fast = fast->getNext()->getNext();
		}
		return slow;
		//Node* slow = head, * fast = head->getNext();
		//while (fast != nullptr) {
			//fast = fast->getNext();
			//if (fast != nullptr) {
				//slow = slow->getNext();
				//fast = fast->getNext();
			//}
		//}
		//return slow;
	};

	//// 创建迭代器的辅助函数      //这一步没用了，每次都创建迭代器麻烦
	//auto makeIterator = [this](Node* node) -> Iterator {
	//	return Iterator(node, this);
	//};


	//auto merge = [&](Node* left, Node* right) -> Node* {
	std::function<Node* (Node*, Node*)> merge = [&](Node* left, Node* right) -> Node* {
		Node dummy;
		Node* tail = &dummy;

		while (left && right)
		{
			//if (comp(makeIterator(left), makeIterator(right)))
			//这里说明以下，首先comp是一个模板比较器，意为用户传入的自定义比较函数，但该函数一开始设计为传入参数为迭代器，后来发现不对
			//因为自定义比较器的意义是用户自定义比较规则，如果传入迭代器，那么和自定义比较规则不沾边了，毕竟这时比较规一步步向下，最终隐藏于T对<>的重载了，所以传入迭代器只能决定正序还是倒序，不能决定比较规则
			//但是问题又来了，如果comp的参数是数据T，就要推导出T的类型，也就是getValue要传入T的实际类型，所以要从comp的参数中提取出数据类型，并去除const 引用，得到原始类型ParamType,即类型萃取
			//类型萃取属于模板元编程特别复杂级别的，实在研究不明白了我甚至连ai给出的代码都看不懂只能交给ai生成了
			//if (comp(left->getValue<ParamType>(), right->getValue<ParamType>()))
			if (compWithTypeIndex(left, right))
			{
				tail->setNext(left);
				left->setPrev(tail);
				left = left->getNext();
			}
			else
			{
				tail->setNext(right);
				right->setPrev(tail);
				right = right->getNext();
			}
			tail = tail->getNext();
		}
		if (left)
		{
			tail->setNext(left);
			left->setPrev(tail);
			//left->setNext(nullptr); //这里不用置空了，不然后面的节点会丢失
		}
		else
		{
			tail->setNext(right);
			right->setPrev(tail);
			//right->setNext(nullptr);
		}

		//tail->setNext((left != nullptr) ? left : right);
		//dummy.getNext()->setPrev(nullptr);
		//tail = tail->getNext();
		//tail->setNext(nullptr);
		dummy.getNext()->setPrev(nullptr);
		return dummy.getNext();
	};

	// 递归的归并排序函数
	std::function<Node* (Node*)> mergeSort = [&](Node* head) -> Node*
	{
		if (!head || !head->getNext())
			return head;

		Node* middle = getMiddle(head);
		Node* nextOfMiddle = middle->getNext();

		middle->setNext(nullptr);

		Node* left = mergeSort(head);
		Node* right = mergeSort(nextOfMiddle);

		return merge(left, right);

		//Node* sortedListStart = merge(left, right);
		//head->setNext(sortedListStart); 

		//// 找到排序后的新尾节点
		//Node* newTail = head;
		//while (newTail->getNext() != nullptr) {
		//	newTail = newTail->getNext();
		//}
		//return newTail;
	};

	if (m_head->getNext() != nullptr)
	{
		Node* firstNode = mergeSort(m_head->getNext());
		m_head->setNext(firstNode);
		firstNode->setPrev(m_head);
		while (firstNode->getNext())
		{
			firstNode = firstNode->getNext();
		}
		m_tail = firstNode;
	}
}

//template <typename T>
//T& FlexibleList::Iterator::operator*();
//{
//	return getValue();
//}

template <typename T>
const T& FlexibleList::Node::getValue() const
{
	if (m_data == nullptr)
	{
		throw std::runtime_error("尝试从地址解引用，但地址为nullptr");
	}
	else
	{
		if (m_data->getTypeIndex() != typeid(T))
		{
			throw std::runtime_error("尝试获取数据但以错误的数据类型");
		}
		else
		{
			const DataElement<T>* d = dynamic_cast<const DataElement<T>*>(m_data.get());
			if (d == nullptr)
			{
				throw std::runtime_error("转换失败");
			}
			return d->getValue();
		}
		//try
		//{
			//const DataElement<T>& d = dynamic_cast<const DataElement<T>&>(*m_data);
			//return d.getValue();
		//}
		//catch (const std::exception&)
		//{
			//throw std::runtime_error("尝试dynamic_cast获取值，但转换失败");
		//}
	}
}

template <typename T>
const T& FlexibleList::Iterator::getValue() const
{
	if (m_node == nullptr)
	{
		throw std::runtime_error("迭代器尝试getvalue，但节点地址为空");
	}
	else
	{
		return m_node->getValue<T>();
	}
}

template <typename T>
const std::type_index& DataElement<T>::getTypeIndex() const
{
	static std::type_index t = typeid(T);
	return t;
}

//template <typename T>
//void DataElement<T>::updateValue(T&& value)
//{
//	m_tempvalue = std::forward<T>(value);
//	return true;
//}

template <typename T>
const T& FlexibleList::frontData()
{
	if (empty())
	{
		throw std::runtime_error("容器为空，不能返回首数据");
	}
	return m_head->getNext()->getValue();
}

template <typename T>
const T& FlexibleList::backData()
{
	if (empty())
	{
		throw std::runtime_error("容器为空，不能返回尾数据");
	}
	return m_tail->getValue();
}

template <typename T>
bool FlexibleList::Node::updateData(T&& value)
{
	//if (m_data->getTypeIndex() == typeid(std::decay_t<T>))
	if(m_data->getTypeIndex() == typeid(T)) //这里好像不用担心T不是原始类型的问题。T&&是万能引用，value可能带const和引用，那么T会被推到为const和引用，
	{									    //那么这种情况得到的typeindex还是和原始类型的typeindex一样吗。但发现，typeid好像会去掉const和引用类型，所以无需decay_t
		DataElement<std::decay_t<T>>* dtElement = dynamic_cast<DataElement<std::decay_t<T>>*>(m_data.get());

		if (dtElement != nullptr)
		{
			dtElement->updateValue(std::forward<T>(value));
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

template <typename T>
template <typename U>
DataElement<T>::DataElement(U&& value) :m_tempvalue(std::forward<U>(value)) {}

template <typename T>
template <typename U>
void DataElement<T>::updateValue(U&& value)
{
	m_tempvalue = std::forward<U>(value);
}

template <typename T>
bool FlexibleList::Iterator::updateData(T&& value)
{
	if (isValid() == true)
	{
		return m_node->updateData(std::forward<T>(value));	
	}
	else
	{
		return false;
	}
}

//template <typename T>
//const T& FlexibleList::Iterator::operator->() const noexcept
//{
//	return *(*this);
//}

template <typename T>
T& DataElement<T>::getValueReference()
{
	return m_tempvalue;
}

template <typename T>
T& FlexibleList::Node::getValueReference()
{
	if (nullptr == m_data)
	{
		throw std::runtime_error("指针为空,无法返回数据");
	}
	if (typeid(T) != m_data->getTypeIndex())
	{
		throw std::runtime_error("数据类型对不上");
	}
	DataElement<T>* p = dynamic_cast<DataElement<T>*>(m_data.get());
	if (nullptr == p)
	{
		throw std::runtime_error("数据类型对不上");
	}
	return p->getValueReference();
}

template <typename T>
T& FlexibleList::Iterator::operator*()
{
	//return m_node->getValueReference();
	if (!isValid())
	{
		throw std::runtime_error("迭代器非法");
	}
	return m_node->getValueReference();
	/*if (m_node != nullptr)
	{
		return m_node->getValueReference();
	}
	else
	{
		throw std::runtime_error("迭代器指向空元素，不允许获取元素引用");
	}*/
}

template <typename T>
T* FlexibleList::Iterator::operator->()
{
	if (!isValid())
	{
		throw std::runtime_error("迭代器非法");
	}
	return &(m_node->getValueReference<T>());
	//return &(m_node->getValueReference);
	/*if (m_node != nullptr)
	{
		return &(m_node->getValueReference());
	}
	else
	{
		throw std::runtime_error("迭代器指向空元素，不允许返回元素引用");
	}*/
}

template <typename T>
FlexibleList::Iterator FlexibleList::insertFrontIt(Iterator& it, T&& val)
{
	if (false == it.isValid())
	{
		return it;
	}
	if (false == isMyIterator(it))
	{
		return it;
	}
	if (it.m_node == m_head)
	{
		return it;
	}
	Node* newNode = new Node(std::forward<T>(val));
	newNode->setPrev(it.m_node->getPrev());
	newNode->setNext(it.m_node);
	newNode->getPrev()->setNext(newNode);
	it.setPrev(newNode);
	return Iterator(newNode, this);
}
