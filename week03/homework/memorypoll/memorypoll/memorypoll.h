#pragma once
#include <algorithm>
#include <stdexcept>
#include <iostream>

class KBlockNode
{
public:
	KBlockNode(void* start, size_t size);
	//void* getStart();
	void* getEnd();
	KBlockNode* getNext();
	KBlockNode* getPrev();
	void setNext(KBlockNode* node);
	void setPrev(KBlockNode* node);
	void* getStart();
	size_t getSize();
	
private:
	void* m_start;
	size_t m_size;
	KBlockNode* m_nextnode;
	KBlockNode* m_prevnode;


};

class KMemoryBlock
{
public:
	KMemoryBlock(size_t blocksize);
	void setNextBlock(KMemoryBlock* block);
	KMemoryBlock* getNextBlock();
	size_t findLargestFreeBlock(void*& start);
	void* alloc(size_t size);
	bool free(void* ptr);
	bool isAddressInRange(void* ptr);
	bool deleteNode(KBlockNode* node);
	size_t getTotalUsedSize();
	//size_t calcuTotalFreeSize();
private:
	void* getEnd();
	void* m_start;
	size_t m_size;
	//size_t m_usedsize;
	KMemoryBlock* m_nextblock;
	KBlockNode* m_startnode;
};


class KMemoryPoll
{
public:
	KMemoryPoll(size_t pollsize, size_t blocksize);
	~KMemoryPoll();
	void* alloc(size_t size);
	bool Free(void* ptr);
	void printPollDetails();


private:
	void init();
	void destory();

	size_t m_pollsize;
	size_t m_blocksize;
	//size_t m_usedsize;
	KMemoryBlock* m_startblock;
};


