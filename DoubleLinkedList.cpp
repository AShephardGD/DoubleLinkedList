#include "DoubleLinkedList.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

DoubleLinkedList::DoubleLinkedList() {
    _head = nullptr;
    _size = 0;
}

DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList& other) {
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

DoubleLinkedList::DoubleLinkedList(DoubleLinkedList&& other) noexcept {
    std::swap(_head, other._head);
    std::swap(_size, other._size);
}



DoubleLinkedList& DoubleLinkedList::operator=(const DoubleLinkedList& other) {
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

DoubleLinkedList& DoubleLinkedList::operator=(DoubleLinkedList&& other) noexcept {
    std::swap(_head, other._head);
    std::swap(_size, other._size);
    delete other._head;
    other._head = nullptr;
    other._size = 0;
    return *this;
}



DoubleLinkedList::~DoubleLinkedList() {
    delete _head;
}



ValueType& DoubleLinkedList::operator[](const size_t pos) {
    if (pos >= _size) {
        throw std::out_of_range("Out of DoubleLinkedList's range");
    }
    Node* res = _head;
    for (size_t i = 0; i < pos; ++i) {
        res = res->_next;
    }
    return res->_data;
}

const ValueType& DoubleLinkedList::operator[](const size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("Out of DoubleLinkedList's range");
    }
    Node* res = _head;
    for (size_t i = 0; i < pos; ++i) {
        res = res->_next;
    }
    return res->_data;
}

size_t DoubleLinkedList::size() const {
    return _size;
}

DoubleLinkedList::ListIterator DoubleLinkedList::getNode(const size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("Out of DoubleLinkedList's range");
    }
    ListIterator it = begin();
    for (size_t i = 0; i < pos; ++i) {
        ++it;
    }
    return it;
}



bool DoubleLinkedList::find(const ValueType& needle) const {
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

bool DoubleLinkedList::isEmpty() const {
    return !_head;
}



void DoubleLinkedList::pushBack(const ValueType& data) {
    insert(data, size());
}

void DoubleLinkedList::pushFront(const ValueType& data) {
    insert(data, 0);
}

void DoubleLinkedList::insert(const ValueType& data, size_t pos) {
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

void DoubleLinkedList::insertAfter(const ValueType& data, DoubleLinkedList::ListIterator it) {
    insert(data, it.getNodeIndex() + 1);
}



void DoubleLinkedList::clear() {
    delete _head;
    _head = nullptr;
    _size = 0;
}

void DoubleLinkedList::popBack() {
    remove(size() - 1);
}

void DoubleLinkedList::popFront() {
    remove(0);
}

void DoubleLinkedList::remove(size_t pos) {
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

void DoubleLinkedList::removeAfter(DoubleLinkedList::ListIterator it) {
    remove(it.getNodeIndex() + 1);
}

void DoubleLinkedList::remove(DoubleLinkedList::ListIterator it) {
    remove(it.getNodeIndex());
}



void DoubleLinkedList::print(std::ostream& stream) const {
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



std::ostream& operator<<(std::ostream& stream, const DoubleLinkedList& list) {
    list.print(stream);
    return stream;
}



void DoubleLinkedList::forEach(ValueType& (*fn)(ValueType&)) {
    if (!_head) {
        return;
    }
    Node* cur = _head;
    do {
        cur->_data = fn(cur->_data);
        cur = cur->_next;
    } while (cur);
}

DoubleLinkedList DoubleLinkedList::map(ValueType& (*fn)(ValueType&)) const {
    DoubleLinkedList list = *this;
    list.forEach(fn);
    return list;
}

void DoubleLinkedList::filter(bool (*fn)(ValueType&)) {
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



void DoubleLinkedList::reverse() {
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

void DoubleLinkedList::reverse(size_t start, size_t end) {
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

DoubleLinkedList DoubleLinkedList::getReverseList() const {
    DoubleLinkedList list = *this;
    list.reverse();
    return list;
}

DoubleLinkedList DoubleLinkedList::getReverseList(size_t start, size_t end) const {
    DoubleLinkedList list = *this;
    list.reverse(start, end);
    return list;
}



DoubleLinkedList::ListIterator DoubleLinkedList::begin() const {
    return DoubleLinkedList::ListIterator(_head, 0);
}

DoubleLinkedList::ListIterator DoubleLinkedList::end() const {
    return DoubleLinkedList::ListIterator(nullptr, size());
}



DoubleLinkedList::Node::Node(const ValueType& data)  {
    _data = data;
    _next = nullptr;
    _prev = nullptr;
}

DoubleLinkedList::Node::~Node() {
    delete _next;
}


DoubleLinkedList::ListIterator::ListIterator(Node* ptr, size_t index) : _ptr(ptr), _index(index) {}

ValueType& DoubleLinkedList::ListIterator::operator*() {
    return _ptr->_data;
}

ValueType* DoubleLinkedList::ListIterator::operator->() {
    return &(_ptr->_data);
}

DoubleLinkedList::ListIterator& DoubleLinkedList::ListIterator::operator++() {
    _ptr = _ptr->_next;
    ++_index;
    return *(this);
}

DoubleLinkedList::ListIterator DoubleLinkedList::ListIterator::operator++(int notUsed) {
    ListIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool DoubleLinkedList::ListIterator::operator!=(DoubleLinkedList::ListIterator& other) {
    return _ptr != other._ptr;
}

bool DoubleLinkedList::ListIterator::operator==(DoubleLinkedList::ListIterator& other) {
    return _ptr == other._ptr;
}

std::ptrdiff_t DoubleLinkedList::ListIterator::operator-(DoubleLinkedList::ListIterator& other) {
    return _ptr - other._ptr;
}

size_t DoubleLinkedList::ListIterator::getNodeIndex() const {
    return _index;
}
