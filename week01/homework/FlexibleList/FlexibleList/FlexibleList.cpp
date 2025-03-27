#include "FlexibleList.h"

FlexibleList::FlexibleList()  noexcept : m_size(0)
{
	m_head = new Node();
	m_tail = m_head;
	m_head->setPrev(nullptr);
	m_head->setNext(nullptr);
}
FlexibleList::FlexibleList(const FlexibleList& other)  noexcept : m_size(0)
{
	m_head = new Node();
	m_tail = m_head;
	m_head->setPrev(nullptr);
	m_head->setNext(nullptr);

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
	m_head->setPrev(nullptr);
	m_head->setNext(nullptr);
	m_head->setNext(other.m_head->getNext());
	m_tail = other.m_tail;
	other.m_head->setNext(nullptr);
	other.m_tail = m_head;
	other.m_size = 0;
}

FlexibleList& FlexibleList::operator=(const FlexibleList& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	clear();
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

void FlexibleList::clear()
{
	Node* curNode = m_head->getNext();
	while (curNode != nullptr)
	{
		Node* tempNode = curNode;
		curNode = curNode->getNext();
		delete tempNode;
	}
	m_head->setNext(nullptr);
	m_tail = m_head;
	m_size = 0;
}

void FlexibleList::deepCopy(const FlexibleList& other)
{
	if (0 == other.m_size)
	{
		return;
	}

	Node* targetNode = other.m_head->getNext();
	m_head->setNext(new Node(*targetNode));
	m_tail = m_head->getNext();
	m_size = 1;

	while (targetNode->getNext() != nullptr)
	{
		targetNode = targetNode->getNext();
		m_tail->setNext(new Node(*targetNode));
		m_tail->getNext()->setPrev(m_tail);
		m_tail = m_tail->getNext();
		m_size++;
	}
}

FlexibleList& FlexibleList::operator=(FlexibleList&& other) noexcept
{
	clear();
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

FlexibleList::Iterator::Iterator(const FlexibleList* container) : m_node(nullptr), m_container(container) {}
FlexibleList::Iterator::Iterator()  noexcept : m_node(nullptr), m_container(nullptr) {}
FlexibleList::Iterator::Iterator(FlexibleList::Node* node, const FlexibleList* container)  noexcept : m_node(node), m_container(container) {}
FlexibleList::Iterator::Iterator(const Iterator& other)  noexcept : m_node(other.m_node), m_container(other.m_container) {}
FlexibleList::Iterator::Iterator(Iterator&& other) noexcept : m_node(other.m_node), m_container(other.m_container)
{
	other.m_node = nullptr;
	other.m_container = nullptr;
}
FlexibleList::Iterator& FlexibleList::Iterator::operator=(const FlexibleList::Iterator& other) noexcept
{
	if (this != &other)
	{
		m_node = other.m_node;
		m_container = other.m_container;
	}
	return *this;
}
FlexibleList::Iterator& FlexibleList::Iterator::operator=(FlexibleList::Iterator&& other) noexcept
{
	if (this != &other)
	{
		m_node = other.m_node;
		m_container = other.m_container;
		other.m_node = nullptr;
		other.m_container;
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
	m_node = m_node->getNext();
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
	m_node = m_node->getPrev();
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
	if (isSameContainer(other) == false)
	{
		return false;
	}
	if ((*m_node == *other.m_node) && (m_node->getPrev() == other.m_node->getPrev()) && (m_node->getNext() == other.m_node->getNext())) // 只需要比较值相等就够了吗？
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
//int FlexibleList::Iterator::operator-(const Iterator& other) const noexcept
//{
//	
//	int n = 0;
//	FlexibleList::Iterator it = *this;
//	while (it != other || it.isValid() == true)
//	{
//		++it;
//		--n;
//	}
//	if (it == other)
//	{
//		return n;
//	}
//
//	it = *this;
//	n = 0;
//	while (it != other || it.isValid() == true)
//	{
//		--it;
//		n++;
//	}
//	if (it == other)
//	{
//		return n;
//	}
//	return -1;
//}


bool FlexibleList::Iterator::isValid() const noexcept
{
	if (m_node == nullptr || m_container == nullptr)
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
	if (0 == m_size && m_head == m_tail)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
const T* FlexibleList::frontElement() noexcept
{
	return begin().getValuePoint();
}

template <typename T>
const T* FlexibleList::backElement() noexcept
{
	return back().getValuePoint();
}
//template <typename T>
//const T* FlexibleList::frontElement() noexcept
//{
	//Iterator it = Iterator(m_head->m_next);
	//it.getValuePoint()

//}
//template <typename T>
//T& FlexibleList::backElement() noexcept
//{
	//return *m_tailIt;
//}
//template <typename T>
//const T& FlexibleList::frontElementConst() const noexcept
//{
	//return *m_headIt;
//}
//template <typename T>
//const T& FlexibleList::backElementConst() const noexcept
//{
	//return *m_tailIt;
//}
void FlexibleList::destory() noexcept
{
	clear();
	delete m_head;
	m_head = nullptr;
	m_tail = m_head;
}
FlexibleList::Iterator FlexibleList::erase(FlexibleList::Iterator it) noexcept
{
	if (it == begin())
	{
		//头节点不允许删除
		return it + 1;
	}
	Iterator tailIt = back();
	if (it == tailIt)
	{
		FlexibleList::Iterator tailPre = tailIt.getPrev();
		tailIt.destory();
		tailPre.setNext(nullptr);
		tailIt = tailPre;
		m_size--;
		return tailIt;
	}
	Iterator itNext = it.getNext();
	it.getPrev().setNext(itNext);
	itNext.setPrev(it.getPrev());
	it.destory();
	m_size--;
	return itNext;
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
	if (it == begin())
	{
		return;
	}
	Node* newNode = new Node(val);
	Iterator newIt = Iterator(newNode);
	it.getPrev().setNext(newIt);
	newIt.setPrev(it.getPrev());
	newIt.setNext(it);
	it.setPrev(newIt);
	m_size++;
}
template <typename T>
//T FlexibleList::popBack() 
void FlexibleList::popBack()
{
	if (empty() == true)
	{
		return;
	}
	Iterator tailIt = back();
	Iterator tailPre = tailIt.getPrev();
	tailIt.destory();
	tailPre.setNext(nullptr);
	m_tail = tailPre.m_node;
	m_size--;
}
FlexibleList::Iterator FlexibleList::begin() const noexcept
{
	return Iterator(m_head->getNext(), this);
}
FlexibleList::Iterator FlexibleList::back() const noexcept
{
	return Iterator(m_tail, this);
}
const FlexibleList::Iterator FlexibleList::cbegin() const noexcept
{
	return Iterator(m_head->getNext(), this);
}

const FlexibleList::Iterator FlexibleList::cback() const noexcept
{
	return Iterator(m_tail, this);
}

bool FlexibleList::operator==(const FlexibleList& other) const noexcept
{
	Iterator otherIt = other.begin();
	Iterator thisIt = begin();

	while (otherIt.isValid() && thisIt.isValid())
	{
		if (*otherIt != *thisIt)
		{
			return false;
		}
		else
		{
			otherIt++;
			thisIt++;
		}
	}
	if (!otherIt.isValid() || !thisIt.isValid())
		return !otherIt.isValid() && !thisIt.isValid();
	return false; //如果两个都有效那么怎么从循环中出来？
}
bool FlexibleList::operator!=(const FlexibleList& other) const noexcept
{
	if (other == *this)
		return false;
	else
		return true;
}
void FlexibleList::swap(FlexibleList& other) noexcept
{
	std::swap(m_head, other.m_head);
	std::swap(m_tail, other.m_tail);
	std::swap(m_size, other.m_size);
	//Node* temp = m_head;
	//m_head = other.m_head;
	//other.m_head = temp;

	//temp = m_tail;
	//m_tail = other.m_tail;
	//other.m_tail = temp;

	//m_size += other.m_size;
	//other.m_size = m_size - other.m_size;
	//m_size = m_size - other.m_size;
}


FlexibleList::Iterator FlexibleList::Iterator::getPrev() const noexcept
{
	return FlexibleList::Iterator(m_node->getPrev(), m_container);
}

FlexibleList::Iterator FlexibleList::Iterator::getNext() const noexcept
{
	return FlexibleList::Iterator(m_node->getPrev(), m_container);
}

void FlexibleList::Iterator::setNext(const Iterator& next) noexcept
{
	if (isSameContainer(next) == false)
		return;
	m_node->setNext(next.m_node);
}

void FlexibleList::Iterator::setPrev(const Iterator& prev) noexcept
{
	if (isSameContainer(prev) == false)
		return;
	m_node->setPrev(prev.m_node);
}

void FlexibleList::Iterator::setNext(Node* node) noexcept
{
	m_node->setNext(node);
}

void FlexibleList::Iterator::setPrev(Node* node) noexcept
{
	m_node->setPrev(node);
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

const FlexibleList::Node* FlexibleList::Node::getConstNext() const
{
	return m_next;
}

const FlexibleList::Node* FlexibleList::Node::getConstPrev() const
{
	return m_prve;
}

FlexibleList::Node* FlexibleList::Node::getPrev()
{
	return m_prve;
}

FlexibleList::Node* FlexibleList::Node::getNext()
{
	return m_next;
}
void FlexibleList::Node::setNext(Node* n)
{
	m_next = n;
}

void FlexibleList::Node::setPrev(Node* n)
{
	m_prve = n;
}

void FlexibleList::swapNode(Node* a, Node* b)
{
	a->swap(b);
}

void FlexibleList::Node::swap(Node* n)
{
	std::swap(m_data, n->m_data);
	std::swap(m_prve, n->m_prve);
	std::swap(m_next, n->m_next);
}

FlexibleList::Iterator FlexibleList::end() const noexcept
{
	return FlexibleList::Iterator(nullptr);
}

void FlexibleList::insertNodeEnd(Node* n)
{
	m_tail->setNext(n);
	n->setPrev(m_tail);
	m_tail = n;
	m_size++;
}
bool FlexibleList::Node::operator<(const Node& other) const noexcept
{
	return (*m_data).less(*(other.m_data));
}
bool FlexibleList::Node::operator>(const Node& other) const noexcept
{
	return !(*m_data).less(*(other.m_data));
}
bool FlexibleList::Node::operator<=(const Node& other) const noexcept
{
	return *this < other || *this == other;
}
bool FlexibleList::Node::operator>=(const Node& other) const noexcept
{
	return *this > other || *this == other;
}

void FlexibleList::swapIterator(FlexibleList::Iterator& a, FlexibleList::Iterator& b)
{
	std::swap(a, b);
}

void FlexibleList::setBeginNode(Node* node)
{
	Iterator it = begin();
	while (it.isValid() == true)
	{
		it = erase(it); 
	}
	m_head->setNext(node);
	node->setPrev(m_head);
	m_size = 0;
	it = begin();
	while (it.isValid() == true)
	{
		m_size++;
	}
}

void FlexibleList::setBackNode(Node* node)
{
	insertNodeEnd(node);
}

void FlexibleList::setHeadPoint(Node* node)
{
	m_head = node;
}

void FlexibleList::setEndPoint(Node* node)
{
	m_tail = node;
}

bool FlexibleList::Iterator::operator<(const Iterator& other) const
{
	if (false == isSameContainer(other))
	{
		throw std::runtime_error("两个迭代器并不来自同一容器对象");
		return false;
	}
	else
	{
		return *m_node < *other;
	}
}

bool FlexibleList::Iterator::operator<=(const Iterator& other) const
{
	return *this < other || *this == other;
}
//上面继续

bool FlexibleList::Iterator::operator>(const Iterator& other) const
{
	return !(*this < other) && (*this != other);
}

bool FlexibleList::Iterator::operator>=(const Iterator& other) const
{
	return !(*this < other);
}
const FlexibleList* FlexibleList::Iterator::getContainerPoint() const
{
	return m_container;
}

bool FlexibleList::Iterator::isSameContainer(const Iterator& other) const
{
	return m_container == other.getContainerPoint();
}

FlexibleList::Node* FlexibleList::Iterator::getNodePoint() const
{
	return m_node;
}
