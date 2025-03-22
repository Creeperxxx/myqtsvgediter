#include <iostream>

template <typename T>
class KNode {
private:
    KNode* m_pre;
    KNode* m_next;
    T m_data;
public:
    KNode(T data) : m_data(data), m_pre(nullptr), m_next(nullptr) {}
    KNode() : m_pre(nullptr), m_next(nullptr) {}
    void setData(T data) { m_data = data; }
    void setPreNode(KNode* pre) { m_pre = pre; }
    void setNextNode(KNode* next) { m_next = next; }
    KNode* getPreNode() 
    {
        return m_pre;
    }
    KNode* getNextNode() { return m_next; }
    T getData()
    {
        return m_data;
    }
};


template <typename T>
class KList {
private:
    KNode<T>* m_head;
    KNode<T>* m_end;
    int m_size;
public:
    KList() : m_size(0) {
        m_head = new KNode<T>();
        m_end = m_head;
        m_head->setNextNode(m_head);
        m_head->setPreNode(m_head);
    }

    ~KList() {
        KNode<T>* current = m_head->getNextNode();
        while (current != m_head) {
            KNode<T>* toDelete = current;
            current = current->getNextNode();
            delete toDelete;
        }
        delete m_head;
    }

    void append(T data) {
        auto nodePtr = new KNode<T>(data);
        nodePtr->setPreNode(m_end);
        nodePtr->setNextNode(m_head);
        m_end->setNextNode(nodePtr);
        m_head->setPreNode(nodePtr);
        m_end = nodePtr;
        ++m_size;
    }

    KNode<T>* remove(KNode<T>* node) {
        if (node == m_head || m_size == 0) return nullptr;
        node->getPreNode()->setNextNode(node->getNextNode());
        node->getNextNode()->setPreNode(node->getPreNode());
        if (node == m_end) m_end = m_end->getPreNode();
        --m_size;
        return node;
    }

    KNode<T>* findAndRemoveByIndex(int index) {
        KNode<T>* current = m_head->getNextNode();
        for (int i = 0; i < index && current != m_head; ++i) {
            current = current->getNextNode();
        }
        return remove(current);
    }
    int getSize()
    {
        return m_size;
    }
    KNode<T>* getHead()
    {
        return m_head;
    }
};

void monkeyQuestion(int n, int m) {
    KList<int> list;
    for (int i = 1; i <= n; ++i) {
        list.append(i);
    }

    int currentIndex = 0;
    while (list.getSize() > 1) {
        currentIndex = (currentIndex + m - 1) % list.getSize();
        auto removed = list.findAndRemoveByIndex(currentIndex);
        std::cout << removed->getData()<< ", ";
    }
    /*std::cout << list.getHead()->getNextNode()->getData()<< std::endl;*/
}