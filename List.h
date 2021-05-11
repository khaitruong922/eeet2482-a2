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

	int getSize()const { return size; }
	int length() const { return size; }
	Node<T>* getHead() { return head; }
	Node<T>** getHeadRef() { return &head; }
	void add(const T& value);
	void deleteHead();
	void deleteNode(int index);
	T get(int index);
};