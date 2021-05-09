#pragma once
#include <iostream>
using namespace std;
template<typename T>
struct Node {
	T data;
	Node* next;
};

template<typename T>
class List {
private:
	int size;
	Node<T>* head;
public:
	List() {
		size = 0;
		head = nullptr;
	}
	~List();

	// O(1)
	int getSize()const { return size; }
	int length() const { return size; }
	Node<T>* getHead() { return head; }
	Node<T>** getHeadRef() { return &head; }
	// O(1)
	void add(const T& value);
	// O(1)
	void deleteHead();
	// O(n)
	void deleteNode(int index);
	// O(n)
	T get(int index);
};