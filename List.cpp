#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#ifndef LINKED_LIST
#define LINKED_LIST

template<typename T>
struct Node {
	T data;
	Node* next;
	Node() {
		next = nullptr;
	}
	~Node() {
		cout << "Destroy node" << data << endl;
	}
};

template<typename T>
class List {
private:
	int size;
	Node<T>* head;
	Node<T>* tail;
public:
	List() {
		size = 0;
		head = nullptr;
		tail = nullptr;
	}
	~List() {
		cout << "Destroy list" << endl;
		// Assign a temporary node with initial value of head node
		auto tmp = head;
		// Remove all nodes from heap memory
		while (tmp) {
			auto temp = tmp;
			tmp = tmp->next;
			delete temp;
		}
	}
	// O(1)
	int getSize()const { return size; }
	int length() const { return size; }
	Node<T>* getHead()const { return head; }

	// O(1)
	void add(const T& value) {
		Node<T>* tmp = new Node<T>;
		tmp->data = value;
		tmp->next = nullptr;
		// If list is empty
		if (head == nullptr) {
			head = tmp;
			tail = tmp;
		}
		else {
			tail->next = tmp;
			tail = tmp;
		}
		size++;
	}
	// O(1)
	void deleteHead() {
		// Store the pointer to current head so that it can be deleted later
		Node<T>* tmp = head;
		// Assign head of list to head's next node
		head = head->next;
		// Delete old head
		delete tmp;
		size--; // Decrease size of list
	}
	// O(n)
	void deleteTail() {
		// If list is empty
		if (head == nullptr) return;
		// If list has 1 element
		if (head->next == nullptr) {
			// Delete head and tail, set them to null pointer as the list is empty now
			delete head;
			delete tail;
			head = nullptr;
			tail = nullptr;
			size = 0;
			return;
		}
		Node<T>* second_last = head;
		// Traverse the list until the second last
		while (second_last->next->next != nullptr) {
			second_last = second_last->next;
		}
		// Delete the old tail 
		delete tail;
		// Assign the second last element to tail
		tail = second_last;
		// Set its next node to be null pointer
		second_last->next = nullptr;
		size--; // Decrease size of list
	}

	// O(n)
	void deleteNode(int index) {
		// Check if the deleted node is head
		if (index == 0) {
			deleteHead();
			return;
		}
		// Check if the deleted node is tail
		if (index == size - 1) {
			deleteTail();
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
	T get(int index) {
		if (index == 0) return head->data;
		if (index == size - 1) return tail->data;
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
};

#endif