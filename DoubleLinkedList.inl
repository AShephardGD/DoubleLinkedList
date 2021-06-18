#include "DoubleLinkedList.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList() {
    _head = nullptr;
    _size = 0;
}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList& other) {
    _size = other._size;
    if (!other._head) {
        _head = nullptr;
        return;
    }
    Node* curOther = other._head;
    Node* cur = new Node(curOther->_data);
    Node* preCur = nullptr;
    _head = cur;
    while (curOther->_next) {
        curOther = curOther->_next;
        cur->_next = new Node(curOther->_data);
        cur->_prev = preCur;
        preCur = cur;
        cur = cur->_next;
    }
    cur->_prev = preCur;
}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(DoubleLinkedList&& other) noexcept {
    std::swap(_head, other._head);
    std::swap(_size, other._size);
}



template <typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(const DoubleLinkedList& other) {
    delete _head;
    _size = other._size;
    if (!other._head) {
        _head = nullptr;
        return *this;
    }
    Node* curOther = other._head;
    Node* cur = new Node(curOther->_data);
    Node* preCur = nullptr;
    _head = cur;
    while (curOther->_next) {
        curOther = curOther->_next;
        cur->_next = new Node(curOther->_data);
        cur->_prev = preCur;
        preCur = cur;
        cur = cur->_next;
    }
    cur->_prev = preCur;
    return *this;
}

template <typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(DoubleLinkedList&& other) noexcept {
    std::swap(_head, other._head);
    std::swap(_size, other._size);
    delete other._head;
    other._head = nullptr;
    other._size = 0;
    return *this;
}



template <typename T>
DoubleLinkedList<T>::~DoubleLinkedList() {
    delete _head;
}



template <typename T>
T& DoubleLinkedList<T>::operator[](const size_t pos) {
    if (pos >= _size) {
        throw std::out_of_range("Out of DoubleLinkedList's range");
    }
    Node* res = _head;
    for (size_t i = 0; i < pos; ++i) {
        res = res->_next;
    }
    return res->_data;
}

template <typename T>
const T& DoubleLinkedList<T>::operator[](const size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("Out of DoubleLinkedList's range");
    }
    Node* res = _head;
    for (size_t i = 0; i < pos; ++i) {
        res = res->_next;
    }
    return res->_data;
}

template <typename T>
size_t DoubleLinkedList<T>::size() const {
    return _size;
}

template <typename T>
typename DoubleLinkedList<T>::ListIterator DoubleLinkedList<T>::getNode(const size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("Out of DoubleLinkedList's range");
    }
    ListIterator it = begin();
    for (size_t i = 0; i < pos; ++i) {
        ++it;
    }
    return it;
}



template <typename T>
bool DoubleLinkedList<T>::find(const T& needle) const {
    if (!_head) {
        return false;
    }
    Node* cur = _head;
    while (cur->_next) {
        if (cur->_data == needle) {
            return true;
        }
        cur = cur->_next;
    }
    return false;
}

template <typename T>
bool DoubleLinkedList<T>::isEmpty() const {
    return !_head;
}



template <typename T>
void DoubleLinkedList<T>::pushBack(const T& data) {
    insert(data, size());
}

template <typename T>
void DoubleLinkedList<T>::pushFront(const T& data) {
    insert(data, 0);
}

template <typename T>
void DoubleLinkedList<T>::insert(const T& data, size_t pos) {
    ++_size;
    if (!_head) {
        _head = new Node(data);
        return;
    }
    Node* cur = _head;
    Node* saved;
    if (!pos) {
        saved = _head;
        _head = new Node(data);
        _head->_next = saved;
        saved->_prev = _head;
        return;
    }
    --pos;
    while (pos-- && cur->_next) {
        cur = cur->_next;
    }
    saved = cur->_next;
    cur->_next = new Node(data);
    cur->_next->_next = saved;
    cur->_next->_prev = cur;
    if (saved) {
        saved->_prev = cur->_next;
    }
}

template <typename T>
void DoubleLinkedList<T>::insertAfter(const T& data, DoubleLinkedList::ListIterator it) {
    insert(data, it.getNodeIndex() + 1);
}



template <typename T>
void DoubleLinkedList<T>::clear() {
    delete _head;
    _head = nullptr;
    _size = 0;
}

template <typename T>
void DoubleLinkedList<T>::popBack() {
    remove(size() - 1);
}

template <typename T>
void DoubleLinkedList<T>::popFront() {
    remove(0);
}

template <typename T>
void DoubleLinkedList<T>::remove(size_t pos) {
    if (pos >= size()) {
        throw std::out_of_range("Out of DoubleLinkedList's range");
    }
    --_size;
    Node* saved = _head->_next;
    Node* cur = _head;
    if (!pos) {
        _head = saved;
        cur->_next = nullptr;
        delete cur;
        if (_head) {
            _head->_prev = nullptr;
        }
        return;
    }
    --pos;
    while (pos--) {
        cur = cur->_next;
    }
    saved = cur->_next->_next;
    cur->_next->_next = nullptr;
    delete cur->_next;
    cur->_next = saved;
    if (cur->_next) {
        cur->_next->_prev = cur;
    }
}

template <typename T>
void DoubleLinkedList<T>::removeAfter(DoubleLinkedList::ListIterator it) {
    remove(it.getNodeIndex() + 1);
}

template <typename T>
void DoubleLinkedList<T>::remove(DoubleLinkedList::ListIterator it) {
    remove(it.getNodeIndex());
}



template <typename T>
void DoubleLinkedList<T>::print(std::ostream& stream) const {
    if(!_head) {
        stream << "(nullptr)";
        return;
    }
    Node* cur = _head;
    do {
        stream << cur->_data << " -> ";
        cur = cur->_next;
    } while(cur);
    stream << "(nullptr)";
}



template <typename Type>
std::ostream& operator<<(std::ostream& stream, const DoubleLinkedList<Type>& list) {
    list.print(stream);
    return stream;
}



template <typename T>
void DoubleLinkedList<T>::forEach(T& (*fn)(T&)) {
    if (!_head) {
        return;
    }
    Node* cur = _head;
    do {
        cur->_data = fn(cur->_data);
        cur = cur->_next;
    } while (cur);
}

template <typename T>
DoubleLinkedList<T> DoubleLinkedList<T>::map(T& (*fn)(T&)) const {
    DoubleLinkedList list = *this;
    list.forEach(fn);
    return list;
}

template <typename T>
void DoubleLinkedList<T>::filter(bool (*fn)(T&)) {
    if (!_head) {
        return;
    }
    Node* cur = _head;
    while (!fn(_head->_data)) {
        if (_head->_next) {
            _head = _head->_next;
            _head->_prev = nullptr;
            cur->_next = nullptr;
            delete cur;
            cur = _head;
        }
        else {
            delete _head;
            _head = nullptr;
        }
    }
    while (cur) {
        if (!cur->_next) {
            break;
        }
        if (!fn((cur->_next)->_data)) {
            if ((cur->_next)->_next) {
                Node* temp = (cur->_next)->_next;
                (cur->_next)->_next = nullptr;
                delete cur->_next;
                cur->_next = temp;
                temp->_prev = cur;
            }
            else {
                delete cur->_next;
                cur->_next = nullptr;
            }
        }
        cur = cur->_next;
    }
}



template <typename T>
void DoubleLinkedList<T>::reverse() {
    if (!_head) {
        return;
    }
    Node* cur = _head;
    do {
        std::swap(cur->_prev, cur->_next);
        _head = cur;
        cur = cur->_prev;
    } while (cur);
}

template <typename T>
void DoubleLinkedList<T>::reverse(size_t start, size_t end) {
    if (start >= size()) {
        throw std::out_of_range("Reverse() - Out of DoubleLinkedList's range");
    }
    Node* last = _head;
    for (size_t i = 0; i <= end && last != nullptr; ++i) {
        last = last->_next;
    }
    end -= start;
    Node* cur = _head;
    Node* startNode = nullptr;
    while (start-- && cur->_next) {
        startNode = cur;
        cur = cur->_next;
    }
    Node* firstNode = cur;
    while (cur->_next && end--) {
        std::swap(cur->_prev, cur->_next);
        cur = cur->_prev;
    }
    std::swap(cur->_prev, cur->_next);
    if (startNode) {
        startNode->_next = cur;
    }
    else {
        _head = cur;
    }
    cur->_prev = startNode;
    firstNode->_next = last;
    if (last) {
        last->_prev = firstNode;
    }
}

template <typename T>
DoubleLinkedList<T> DoubleLinkedList<T>::getReverseList() const {
    DoubleLinkedList list = *this;
    list.reverse();
    return list;
}

template <typename T>
DoubleLinkedList<T> DoubleLinkedList<T>::getReverseList(size_t start, size_t end) const {
    DoubleLinkedList list = *this;
    list.reverse(start, end);
    return list;
}



template <typename T>
typename DoubleLinkedList<T>::ListIterator DoubleLinkedList<T>::begin() const {
    return DoubleLinkedList::ListIterator(_head, 0);
}

template <typename T>
typename DoubleLinkedList<T>::ListIterator DoubleLinkedList<T>::end() const {
    return DoubleLinkedList::ListIterator(nullptr, size());
}



template <typename T>
DoubleLinkedList<T>::Node::Node(const T& data)  {
    _data = data;
    _next = nullptr;
    _prev = nullptr;
}

template <typename T>
DoubleLinkedList<T>::Node::~Node() {
    delete _next;
}



template <typename T>
DoubleLinkedList<T>::ListIterator::ListIterator(Node* ptr, size_t index) : _ptr(ptr), _index(index) {}

template <typename T>
T& DoubleLinkedList<T>::ListIterator::operator*() {
    return _ptr->_data;
}

template <typename T>
T* DoubleLinkedList<T>::ListIterator::operator->() {
    return &(_ptr->_data);
}

template <typename T>
typename DoubleLinkedList<T>::ListIterator& DoubleLinkedList<T>::ListIterator::operator++() {
    _ptr = _ptr->_next;
    ++_index;
    return *(this);
}

template <typename T>
typename DoubleLinkedList<T>::ListIterator DoubleLinkedList<T>::ListIterator::operator++(int notUsed) {
    ListIterator tmp = *this;
    ++(*this);
    return tmp;
}

template <typename T>
bool DoubleLinkedList<T>::ListIterator::operator!=(DoubleLinkedList::ListIterator& other) {
    return _ptr != other._ptr;
}

template <typename T>
bool DoubleLinkedList<T>::ListIterator::operator==(DoubleLinkedList::ListIterator& other) {
    return _ptr == other._ptr;
}

template <typename T>
std::ptrdiff_t DoubleLinkedList<T>::ListIterator::operator-(DoubleLinkedList::ListIterator& other) {
    return _ptr - other._ptr;
}

template <typename T>
size_t DoubleLinkedList<T>::ListIterator::getNodeIndex() const {
    return _index;
}
