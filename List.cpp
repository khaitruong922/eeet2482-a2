#include <iostream>
#include "List.h"
using namespace std;


template<typename T>
// Destructor: deallocate memory of all nodes
List<T>::~List() {
	// Assign a temporary node with initial value of head node
	auto current = head;
	// Remove all nodes from heap memory
	while (current) {
		// Assign the temporary node to current node
		auto temp = current;
		// Set the current node to next
		current = current->next;
		// Delete temporary node which is the current node
		delete temp;
	}
}

template<typename T>
void List<T>::add(const T& value) {
	// Create new node
	Node<T>* tmp = new Node<T>();
	// Assign data to new node
	tmp->data = value;
	// Set the next node of the new node to head
	tmp->next = head;
	// Set the head to next node
	head = tmp;
	// Incrase the list size
	size++;
}
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
	// Decrease size of list
	size--;
}
