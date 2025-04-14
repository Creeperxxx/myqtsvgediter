#include "memorypoll.h"

KMemoryPoll::KMemoryPoll(size_t pollsize, size_t blocksize)
	: m_pollsize(pollsize)
	, m_blocksize(blocksize)
	//, m_usedsize(0)
	, m_startblock(nullptr)
{
	init();
}

KMemoryPoll::~KMemoryPoll()
{
	destory();
}

void KMemoryPoll::destory()
{
	KMemoryBlock* block = m_startblock;
	while (block != nullptr)
	{
		KMemoryBlock* nextblock = block->getNextBlock();
		delete block;
		block = nextblock;
	}
}

bool KMemoryBlock::isAddressInRange(void* ptr)
{
	if (ptr == nullptr)
	{
		return false;
	}
	if (ptr >= m_start && ptr <= static_cast<char*>(m_start) + m_size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KMemoryPoll::Free(void* ptr)
{
	KMemoryBlock* curBlock = m_startblock;
	bool isInRange = false;
	while (curBlock)
	{
		if (curBlock->isAddressInRange(ptr))
		{
			isInRange = true;
			break;
		}
		curBlock = curBlock->getNextBlock();
	}
	if (isInRange)
	{
		return curBlock->free(ptr);
	}
	else
	{
		return false;
	}

}

bool KMemoryBlock::free(void* ptr)
{
	if (ptr == nullptr)
	{
		return false;
	}
	if (ptr < m_start || ptr >= static_cast<char*>(m_start) + m_size)
	{
		return false;
	}
	KBlockNode* node = m_startnode;
	while (node)
	{
		if (node->getStart() == ptr)
		{
			return deleteNode(node);
		}
		node = node->getNext();
	}
	return false;
}

bool KMemoryBlock::deleteNode(KBlockNode* node)
{
	if (node == nullptr)
	{
		return false;
	}
	KBlockNode* prenode = node->getPrev();
	KBlockNode* nxtnode = node->getNext();
	if (prenode != nullptr && nxtnode != nullptr)
	{
		prenode->setNext(nxtnode);
		nxtnode->setPrev(prenode);
	}
	else if (prenode == nullptr && nxtnode != nullptr)
	{
		m_startnode = nxtnode;
		nxtnode->setPrev(nullptr);
	}
	else if (prenode != nullptr && nxtnode == nullptr)
	{
		prenode->setNext(nullptr);
	}
	else
	{
		m_startnode = nullptr;
	}
	delete node;
	return true;
}



void KMemoryPoll::init()
{
	if (m_pollsize > 10485760)
	{
		throw std::runtime_error("error");
	}
	size_t pollsize = m_pollsize;
	if (pollsize <= 0 || m_blocksize <= 0)
	{
		throw std::runtime_error("error");
	}
	while (pollsize > 0)
	{
		KMemoryBlock* block = nullptr;
		if (pollsize >= m_blocksize)
		{
			block = new KMemoryBlock(m_blocksize);
			pollsize -= m_blocksize;
		}
		else
		{
			block = new KMemoryBlock(pollsize);
			pollsize = 0;
		}
		if (block == nullptr)
		{
			throw std::runtime_error("error");
		}

		if (m_startblock == nullptr)
		{
			m_startblock = block;
		}
		else
		{
			KMemoryBlock* lastblock = m_startblock;
			while (lastblock->getNextBlock() != nullptr)
			{
				lastblock = lastblock->getNextBlock();
			}
			lastblock->setNextBlock(block);
		}

	}

}

KMemoryBlock* KMemoryBlock::getNextBlock()
{
	return m_nextblock;
}

KMemoryBlock::KMemoryBlock(size_t blocksize)
	: m_start(nullptr)
	, m_size(blocksize)
	//, m_usedsize(0)
	, m_nextblock(nullptr)
	, m_startnode(nullptr)
{
	m_start = malloc(blocksize);
	if (m_start == nullptr)
	{
		throw std::runtime_error("error");
	}
}

void KMemoryBlock::setNextBlock(KMemoryBlock* block)
{
	m_nextblock = block;

}

void* KMemoryPoll::alloc(size_t size)
{
	if (size > m_pollsize)
	{
		throw std::runtime_error("error");
	}
	KMemoryBlock* block = m_startblock;
	while (true)
	{
		void* tempptr = nullptr;
		if (block->findLargestFreeBlock(tempptr) >= size)
		{
			//从这个块中分配内存
			return block->alloc(size);
		}
		else
		{
			block = block->getNextBlock();
			if (block == nullptr)
			{
				return nullptr;
			}
		}
	}
}

size_t KBlockNode::getSize()
{
	return m_size;
}

size_t KMemoryBlock::getTotalUsedSize()
{
	KBlockNode* node = m_startnode;
	size_t sum = 0;
	while (node)
	{
		sum += node->getSize();
		node = node->getNext();
	}
	return sum;
}

void KMemoryPoll::printPollDetails()
{
	std::cout << "总大小为：" << m_pollsize << std::endl;
	std::cout << "块大小为：" << m_blocksize << std::endl;
	int i = 0;
	KMemoryBlock* curblock = m_startblock;
	while (curblock)
	{
		std::cout << "块" << i++ << "详情为:" << std::endl;
		std::cout << "已经用了：" << curblock->getTotalUsedSize() << std::endl;
		std::cout << "还剩：" << m_blocksize - curblock->getTotalUsedSize() << std::endl;
		curblock = curblock->getNextBlock();
	}
}


void* KMemoryBlock::alloc(size_t size)
{
	// 从两个节点之间找到适合的内存块
	KBlockNode* node = m_startnode;
	void* startptr = nullptr;
	size_t suitableSize = 0;
	if (node == nullptr)
	{
		startptr = m_start;
	}
	else
	{
		while (node->getNext() != nullptr)
		{
			KBlockNode* nextnode = node->getNext();
			if (node->getEnd() < nextnode->getStart())
			{
				size_t freesize = static_cast<char*>(nextnode->getStart()) - static_cast<char*>(node->getEnd()) - 1;
				if (freesize >= size)
				{
					if (freesize < suitableSize || suitableSize == 0)
					{
						suitableSize = freesize;
						startptr = static_cast<char*>(node->getEnd()) + 1;
					}
				}
			}
			else
			{
				throw std::runtime_error("error");
			}
			node = nextnode;
		}
		if (node->getEnd() <= getEnd())
		{
			size_t freesize = static_cast<char*>(getEnd()) - static_cast<char*>(node->getEnd());
			if (freesize >= size)
			{
				if (freesize < suitableSize || suitableSize == 0)
				{
					suitableSize = freesize;
					startptr = static_cast<char*>(node->getEnd()) + 1;
				}
			}
		}

		if (startptr == nullptr || suitableSize == 0)
		{
			return nullptr;
		}
	}


	// 创建节点
	KBlockNode* newnode = new KBlockNode(startptr, size);
	if (newnode == nullptr)
	{
		throw std::runtime_error("error");
	}

	if (m_startnode == nullptr)
	{
		m_startnode = newnode;
		return startptr;
	}

	KBlockNode* curnode = m_startnode;
	while (!curnode)
	{
		if (curnode->getStart() > startptr)
		{
			newnode->setNext(curnode);
			curnode->setPrev(newnode);
			if (curnode->getPrev())
			{
				newnode->setPrev(curnode->getPrev());
				curnode->getPrev()->setNext(newnode);
			}
			break;
		}
		curnode = curnode->getNext();
	}

	return startptr;
}

void KBlockNode::setNext(KBlockNode* node)
{
	m_nextnode = node;
}

void KBlockNode::setPrev(KBlockNode* node)
{
	m_prevnode = node;
}

KBlockNode::KBlockNode(void* start, size_t size)
	: m_start(start)
	, m_size(size)
	, m_nextnode(nullptr)
	, m_prevnode(nullptr)
{
}
//
//size_t KMemoryBlock::calcuTotalFreeSize()
//{
//	size_t totalSize = 0;
//	KBlockNode* node = m_startnode;
//	while (node->getNext() != nullptr)
//	{
//		KBlockNode* nextnode = node->getNext();
//		if (node->getEnd() < nextnode->getStart())
//		{
//			size_t freesize = static_cast<char*>(nextnode->getStart()) - static_cast<char*>(node->getEnd()) - 1;
//			totalSize += freesize;
//		}
//		else
//		{
//			throw std::runtime_error("error");
//		}
//	}
//	return totalSize;
//}

void* KBlockNode::getStart()
{
	return m_start;
}

void* KBlockNode::getEnd()
{
	return static_cast<void*>(static_cast<char*>(m_start) + m_size - 1);
}

KBlockNode* KBlockNode::getNext()
{
	return m_nextnode;
}

KBlockNode* KBlockNode::getPrev()
{
	return m_prevnode;
}

//void* KBlockNode::getStart()
//{
//	return m_start;
//}

size_t KMemoryBlock::findLargestFreeBlock(void*& start)
{
	KBlockNode* node = m_startnode;
	void* startptr = nullptr;
	size_t largestSize = 0;

	if (node == nullptr)
	{
		start = m_start;
		return m_size;
	}

	while (node->getNext() != nullptr)
	{
		KBlockNode* nextnode = node->getNext();
		if (node->getEnd() < nextnode->getStart())
		{
			size_t freesize = static_cast<char*>(nextnode->getStart()) - static_cast<char*>(node->getEnd()) - 1;
			if (freesize > largestSize)
			{
				largestSize = freesize;
				startptr = static_cast<char*>(node->getEnd()) + 1;
			}
		}
		else
		{
			throw std::runtime_error("error");
		}
		node = nextnode;
	}
	if (node->getEnd() <= getEnd())
	{
		size_t freesize = static_cast<char*>(getEnd()) - static_cast<char*>(node->getEnd());
		if (freesize > largestSize)
		{
			largestSize = freesize;
			startptr = static_cast<char*>(node->getEnd()) + 1;
		}
	}
	if (largestSize > 0)
	{
		start = startptr;
	}
	else
	{
		start = nullptr;
	}
	return largestSize;
}

void* KMemoryBlock::getEnd()
{
	return static_cast<void*>(static_cast<char*>(m_start) + m_size);
}
