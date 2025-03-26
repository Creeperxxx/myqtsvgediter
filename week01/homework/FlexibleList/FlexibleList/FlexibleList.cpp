#include "FlexibleList.h"

FlexibleList::FlexibleList()  noexcept : m_size(0) 
{
	m_head = new Node();
	m_tail = m_head;
	m_head->m_prve = nullptr;
	m_head->m_next = nullptr;
}
FlexibleList::FlexibleList(const FlexibleList& other)  noexcept : m_size(0)
{
	m_head = new Node();
	m_tail = m_head;
	m_head->m_prve = nullptr;
	m_head->m_next = nullptr;

	deepCopy(other);
	//if (0 == other.m_size)
	//{
	//	return;
	//}

	//Node* targetNode = other.m_head;
	//m_head = new Node(*targetNode);
	//m_tail = m_head;
	//m_size = 1;

	//while (targetNode->m_next != nullptr)
	//{
	//	targetNode = targetNode->m_next;
	//	m_tail->m_next = new Node(*targetNode);
	//	m_tail->m_next->m_prve = m_tail;
	//	m_tail = m_tail->m_next;
	//	m_size++;
	//}



	//FlexibleList::Node* targetNode = other.m_head;
	//FlexibleList::Node* newNode = nullptr;
	//int targetSize = other.m_size;
	//FlexibleList::Node* preNewNode = nullptr;
	//if (targetSize == 0)
	//{
	//	m_head = nullptr;
	//	m_tail = nullptr;
	//	m_size = 0;
	//	return;
	//}
	//newNode = new FlexibleList::Node(targetNode);
	//targetNode = targetNode->m_next;
	//preNewNode = newNode;
	//while (targetSize--)
	//{
	//	newNode = new FlexibleList::Node(targetNode);
	//	preNewNode->m_next = newNode;
	//	newNode->m_prve = preNewNode;
	//	preNewNode = newNode;
	//	targetNode = targetNode->m_next;
	//}
}
FlexibleList::FlexibleList(FlexibleList&& other) noexcept : m_size(other.m_size)
{
	m_head = new Node();
	m_tail = m_head;
	m_head->m_prve = nullptr;
	m_head->m_next = nullptr;
	m_head->m_next = other.m_head->m_next;
	m_tail = other.m_tail;
	other.m_head->m_next = nullptr;
	other.m_tail = m_head;
	other.m_size = 0;
}

FlexibleList& FlexibleList::operator=(const FlexibleList& other) noexcept 
{
	if (this == &other)
	{
		return *this;
	}

	destory();
	deepCopy(other);
	return *this;
	//if (0 == other.m_size)
	//{
	//	return;
	//}

	//Node* targetNode = other.m_head;
	//m_head = new Node(*targetNode);
	//m_tail = m_head;
	//m_size = 1;

	//while (targetNode->m_next != nullptr)
	//{
	//	targetNode = targetNode->m_next;
	//	m_tail->m_next = new Node(*targetNode);
	//	m_tail->m_next->m_prve = m_tail;
	//	m_tail = m_tail->m_next;
	//	m_size++;
	//}
}

void FlexibleList::destory()
{
	Node* curNode = m_head->m_next;
	while (curNode != nullptr)
	{
		Node* tempNode = curNode;
		curNode = curNode->m_next;
		delete tempNode;
	}
	m_head->m_next = nullptr;
	m_tail = m_head;
	m_size = 0;
}

void FlexibleList::deepCopy(const FlexibleList& other)
{
	if (0 == other.m_size)
	{
		return;
	}

	Node* targetNode = other.m_head->m_next;
	m_head->m_next = new Node(*targetNode);
	m_tail = m_head->m_next;
	m_size = 1;

	while (targetNode->m_next != nullptr)
	{
		targetNode = targetNode->m_next;
		m_tail->m_next = new Node(*targetNode);
		m_tail->m_next->m_prve = m_tail;
		m_tail = m_tail->m_next;
		m_size++;
	}
}

FlexibleList& FlexibleList::operator=(FlexibleList&& other) noexcept 
{
	destory();
	m_head = other.m_head;
	m_tail = other.m_tail;
	m_size = other.m_size;
	other.m_head = nullptr;
	other.m_tail = nullptr;
	other.m_size = 0;
	return *this;
}

FlexibleList::~FlexibleList() noexcept 
{
	destory();
}

FlexibleList::Node::Node()  noexcept : m_data(nullptr), m_prve(nullptr), m_next(nullptr) {};

FlexibleList::Node::Node(Node&& other) noexcept : m_data(std::move(other.m_data)), m_prve(nullptr), m_next(nullptr) {}

FlexibleList::Node& FlexibleList::Node::operator=(const Node& other) noexcept 
{
	m_data = other.m_data == nullptr ? nullptr : other.m_data->clone();
	m_prve = nullptr;
	m_next = nullptr;
	return *this;
}

FlexibleList::Node& FlexibleList::Node::operator=(Node&& other) noexcept 
{
	m_data = std::move(other.m_data);
	m_prve = nullptr;
	m_next = nullptr;
	return *this;
}

FlexibleList::Node::Node(const Node& other)  noexcept : m_data(other.m_data == nullptr ? nullptr : other.m_data->clone()), m_next(nullptr), m_prve(nullptr) {}

FlexibleList::Iterator::Iterator()  noexcept : m_node(nullptr) {}
FlexibleList::Iterator::Iterator(FlexibleList::Node* node)  noexcept : m_node(node) {}
FlexibleList::Iterator::Iterator(const Iterator& other)  noexcept : m_node(other.m_node) {}
FlexibleList::Iterator::Iterator(Iterator&& other) noexcept : m_node(other.m_node)
{
	other.m_node = nullptr;
}
FlexibleList::Iterator& FlexibleList::Iterator::operator=(const FlexibleList::Iterator& other) noexcept 
{
	if (this != &other)
	{
		m_node = other.m_node;
	}
	return *this;
}
FlexibleList::Iterator& FlexibleList::Iterator::operator=(FlexibleList::Iterator&& other) noexcept
{
	if (this != &other)
	{
		m_node = other.m_node;
		other.m_node = nullptr;
	}
	return *this;
}
FlexibleList::Iterator::~Iterator()  noexcept {}

FlexibleList::Node& FlexibleList::Iterator::operator*() const noexcept
{
	if (m_node == nullptr)
	{
		static FlexibleList::Node node; //这里不想用异常处理。用下面方法代替：如果该迭代器为空，则返回一个空节点
		return node;
	}
	else
	{
		return *m_node;
	}
}

FlexibleList::Node* FlexibleList::Iterator::operator->() const noexcept
{
	return m_node; // 这里很想像*重载那样判断以下合法性，但还是交给使用者判断吧。
}
		
FlexibleList::Iterator& FlexibleList::Iterator::operator++() noexcept // 前置++
{
	m_node = m_node->m_next;
	return *this;
}
FlexibleList::Iterator FlexibleList::Iterator::operator++(int) noexcept
{
	FlexibleList::Iterator it = *this;
	//m_node = m_node->m_next; //这一步可以被之前实现的前置++代替，妙
	++(*this);
	return it;
}
FlexibleList::Iterator& FlexibleList::Iterator::operator--() noexcept
{
	m_node = m_node->m_prve;
	return *this;
}
FlexibleList::Iterator FlexibleList::Iterator::operator--(int) noexcept
{
	FlexibleList::Iterator it = *this;
	//m_node = m_node->m_prve; //与++同理
	--(*this);
	return it;
}

bool FlexibleList::Iterator::operator==(const Iterator& other) const noexcept
{
	if ((*m_node == *other.m_node) && (m_node->m_prve == other.m_node->m_prve) && (m_node->m_next == other.m_node->m_next)) // 只需要比较值相等就够了吗？
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool FlexibleList::Iterator::operator!=(const Iterator& other) const noexcept
{
	if (*this == other)
	{
		return false;
	}
	else
	{
		return true;
	}
}
FlexibleList::Iterator& FlexibleList::Iterator::operator+=(int n) noexcept
{
	if (0 == n)
		return *this;
	bool forward = (n > 0);
	int step = std::abs(n);
	while (step--)
	{
		if (true == isValid())
		{
			if (true == forward)
				++(*this);
			else
				--(*this);
		}
		else
		{
			return *this;
		}
	}
	return *this;
	//if (n == 0)
	//{
	//	return *this;
	//}
	//else if (n > 0)
	//{
	//	while (n--)
	//	{
	//		if (true == isValid())
	//		{
	//			++(*this);
	//		}
	//		else
	//		{
	//			//如果不能前进n个单位，如何处理
	//			return *this;
	//		}
	//	}
	//	return *this;
	//}
	//else
	//{
	//	n = std::abs(n);
	//	while (n--)
	//	{
	//		if (true == isValid())
	//		{
	//			--(*this);
	//		}
	//		else
	//		{
	//			return *this;
	//		}
	//	}
	//	return *this;
	//}
}
FlexibleList::Iterator& FlexibleList::Iterator::operator-=(int n) noexcept
{
	if (0 == n)
		return *this;
	bool forward = (n < 0);
	int step = std::abs(n);
	while (step--)
	{
		if (true == isValid())
		{
			if (true == forward)
				++(*this);
			else
				--(*this);
		}
		else
		{
			return *this;
		}
	}
	return *this;
}
FlexibleList::Iterator FlexibleList::Iterator::operator-(int n) noexcept
{
	if (0 == n)
		return *this;
	bool forward = (n < 0);
	int step = std::abs(n);
	FlexibleList::Iterator it = *this;
	while (step--)
	{
		if (true == isValid())
		{
			if (true == forward)
				++it;
			else
				--it;
		}
		else
		{
			return it;
		}
	}
	return it;
}
FlexibleList::Iterator FlexibleList::Iterator::operator+(int n) noexcept
{
	if (0 == n)
		return *this;
	bool forward = (n > 0);
	int step = std::abs(n);
	FlexibleList::Iterator it = *this;
	while (step--)
	{
		if (true == isValid())
		{
			if (true == forward)
				++it;
			else
				--it;
		}
		else
		{
			return it;
		}
	}
	return it;
}
int FlexibleList::Iterator::operator-(const Iterator& other) const noexcept
{
	//int n = 0;
	//FlexibleList::Iterator it = *this;
	//while (it != other || it.isValid() == true)
	//{
	//	++it;
	//	--n;
	//}
	//if (it == other)
	//{
	//	return n;
	//}

	//it = *this;
	//n = 0;
	//while (it != other || it.isValid() == true)
	//{
	//	--it;
	//	n++;
	//}
	//if (it == other)
	//{
	//	return n;
	//}
	//return -1;
}


bool FlexibleList::Iterator::isValid() const noexcept
{
	if (m_node == nullptr)
		return false;
	else
		return true;
}

size_t FlexibleList::size() const noexcept
{
	return m_size;
}
bool FlexibleList::empty() const noexcept
{
	if (0 == m_size && m_headIt == m_tailIt)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
T& FlexibleList::frontElement() noexcept
{
	return *m_headIt;
}
template <typename T>
T& FlexibleList::backElement() noexcept
{
	return *m_tailIt;
}
template <typename T>
const T& FlexibleList::frontElementConst() const noexcept
{
	return *m_headIt;
}
template <typename T>
const T& FlexibleList::backElementConst() const noexcept
{
	return *m_tailIt;
}
void FlexibleList::clear() noexcept
{
	destory();
}
FlexibleList::Iterator FlexibleList::erase(FlexibleList::Iterator it) noexcept
{
	if (it == m_headIt)
	{
		//头节点不允许删除
		return it + 1;
	}
	if (it == m_tailIt)
	{
		FlexibleList::Iterator tailPre = m_tailIt.getPrev();
		m_tailIt.destory();
		tailPre.setNext(nullptr);
		m_tailIt = tailPre;
		m_size--;
		return;
	}
	it.getPrev().setNext(it.getNext());
	it.getNext().setPrev(it.getPrev());
	it.destory();
	m_size--;
}

void FlexibleList::Iterator::destory() noexcept
{
	if (m_node != nullptr)
	{
		delete m_node;
	}
}
template <typename T>
void FlexibleList::insertFrontIt(FlexibleList::Iterator it, const T& val) noexcept
{
	if (it == m_headIt)
	{
		return;
	}
	Node* newNode = new node(val);
	Iterator newIt = Iterator(newNode);
	it.getPrev().setNext(newIt);
	newIt.setPrev(it.getPrev());
	newIt.setNext(it);
	it.setPrev(newIt);
	m_size++;
}
template <typename T>
T FlexibleList::popBack() 
{
	if (empty() == true)
	{
		return T();
	}
	T tempVal = static_cast<const T&>((static_cast<DataElement<T>*>(m_tailIt.m_node->m_data.get()))->getValue());
	Iterator tailPre = m_tailIt.getPrev();
	m_tailIt.destory();
	tailPre.setNext(nullptr);
	m_tailIt = tailPre;
	m_size--;

}
FlexibleList::Iterator FlexibleList::begin() const noexcept
{
	return m_headIt;
}
FlexibleList::Iterator FlexibleList::end() const noexcept
{
	return m_tailIt;
}
const FlexibleList::Iterator& FlexibleList::cbegin() const noexcept
{
	return m_headIt;
}

const FlexibleList::Iterator& FlexibleList::cend() const noexcept
{
	return m_tailIt;
}

bool FlexibleList::operator==(const FlexibleList& other) const noexcept
{
	
}
	//bool operator!=(const FlexibleList& other) const noexcept;
	//void swap(FlexibleList& other) noexcept;

FlexibleList::Iterator FlexibleList::Iterator::getPrev() const noexcept
{
	return FlexibleList::Iterator(m_node->m_prve);
}

FlexibleList::Iterator FlexibleList::Iterator::getNext() const noexcept
{
	return FlexibleList::Iterator(m_node->m_next);
}

void FlexibleList::Iterator::setNext(const Iterator& next) noexcept
{
	m_node->m_next = next.m_node;
}

void FlexibleList::Iterator::setPrev(const Iterator& prev) noexcept
{
	m_node->m_prve = prev.m_node;
}

void FlexibleList::Iterator::setNext(Node* node) noexcept
{
	m_node->m_next = node;
}

void FlexibleList::Iterator::setPrev(Node* node) noexcept
{
	m_node->m_prve = node;
}

bool FlexibleList::Node::operator==(const Node& other) const noexcept
{
	if (!m_data || !other.m_data)
		return !m_data && !other.m_data;
	return m_data->isEqual(*other.m_data); //是否要比较前后关系是否也相等？
}

bool FlexibleList::Node::operator!=(const Node& other) const noexcept
{
	if (other == *this)
	{
		return false;
	}
	else
	{
		return true;
	}
}

