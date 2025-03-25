#include "FlexibleList.h"

FlexibleList::FlexibleList() : m_size(0) 
{
	m_head = new Node();
	m_tail = m_head;
	m_head->m_prve = nullptr;
	m_head->m_next = nullptr;
}
FlexibleList::FlexibleList(const FlexibleList& other) : m_size(0)
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
FlexibleList::FlexibleList(FlexibleList&& other) : m_size(other.m_size)
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

FlexibleList& FlexibleList::operator=(const FlexibleList& other)
{
	if (this == &other)
	{
		return *this;
	}

	destory();
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

FlexibleList& FlexibleList::operator=(FlexibleList&& other)
{
	destory();
	m_head = other.m_head;
	m_tail = other.m_tail;
	m_size = other.m_size;
	other.m_head = nullptr;
	other.m_tail = nullptr;
	other.m_size = 0;
}

FlexibleList::~FlexibleList()
{
	destory();
}

FlexibleList::Node::Node() : m_data(nullptr), m_prve(nullptr), m_next(nullptr) {};

FlexibleList::Node::Node(Node&& other) : m_data(std::move(other.m_data)), m_prve(nullptr), m_next(nullptr) {}

FlexibleList::Node& FlexibleList::Node::operator=(const Node& other)
{
	m_data = other.m_data == nullptr ? nullptr : other.m_data->clone();
	m_prve = nullptr;
	m_next = nullptr;
}

FlexibleList::Node& FlexibleList::Node::operator=(Node&& other)
{
	m_data = std::move(other.m_data);
	m_prve = nullptr;
	m_next = nullptr;
}

