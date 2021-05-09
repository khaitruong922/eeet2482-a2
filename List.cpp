#include <iostream>
#include "List.h"
using namespace std;


template<typename T>
List<T>::~List() {
	// Assign a temporary node with initial value of head node
	auto current = head;
	// Remove all nodes from heap memory
	while (current) {
		auto temp = current;
		current = current->next;
		delete temp;
	}
}
// O(1)
// O(1)
template<typename T>
void List<T>::add(const T& value) {
	Node<T>* tmp = new Node<T>();
	tmp->data = value;
	tmp->next = head;
	head = tmp;
	size++;
}
// O(1)
template<typename T>
void List<T>::deleteHead() {
	// Store the pointer to current head so that it can be deleted later
	Node<T>* tmp = head;
	// Assign head of list to head's next node
	head = head->next;
	// Delete old head
	delete tmp;
	size--; // Decrease size of list
}
// O(n)
template<typename T>
void List<T>::deleteNode(int index) {
	// Check if the deleted node is head
	if (index == 0) {
		deleteHead();
		return;
	}
	// Check if the index is out of range
	if (index >= size) return;
	Node<T>* before_deleted = head;
	for (int i = 0;i < index - 1;i++) {
		before_deleted = before_deleted->next;
	}
	// Get the node after the deleted node
	Node<T>* after_deleted = before_deleted->next->next;
	// Delete the node that is going to be deleted
	delete before_deleted->next;
	// Assign the next node of the node before the deleted node to be the node after the deleted note
	before_deleted->next = after_deleted;
	size--; // Decrease size of list
}

// O(n)
template<typename T>
T List<T>::get(int index) {
	if (index >= size) {
		cerr << "Invalid index access to list" << endl;
		return nullptr;
	}
	Node<T>* current = head;
	for (int i = 0;i < index;i++) {
		current = current->next;
	}
	return current->data;
}
