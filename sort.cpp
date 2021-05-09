#include <iostream>
#include "List.h"
#include <string>
#include "sort.h"
using namespace std;

// Splitting two into halves.
// If the size of the list is odd, then extra element goes in the first list.
template <typename T>
void splitList(Node<T>* source, Node<T>** front, Node<T>** back)
{
	Node<T>* ptr1;
	Node<T>* ptr2;
	ptr2 = source;
	ptr1 = source->next;

	// ptr1 is incrmented twice and ptr2 is icremented once.
	while (ptr1 != nullptr) {
		ptr1 = ptr1->next;
		if (ptr1 != nullptr) {
			ptr2 = ptr2->next;
			ptr1 = ptr1->next;
		}
	}

	// ptr2 is at the midpoint.
	*front = source;
	*back = ptr2->next;
	ptr2->next = nullptr;
}

Node<Item*>* mergedByIdItems(Node<Item*>* lst1, Node<Item*>* lst2) {
	Node<Item*>* result = nullptr;

	// Base Cases
	if (lst1 == nullptr)
		return lst2;
	else if (lst2 == nullptr)
		return lst1;
	// recursively merging two lists
	if (lst1->data->getId() <= lst2->data->getId()) {
		result = lst1;
		result->next = mergedByIdItems(lst1->next, lst2);
	}
	else {
		result = lst2;
		result->next = mergedByIdItems(lst1, lst2->next);
	}
	return result;
}
Node<Item*>* mergedByTitleItems(Node<Item*>* lst1, Node<Item*>* lst2) {
	Node<Item*>* result = nullptr;

	// Base Cases
	if (lst1 == nullptr)
		return lst2;
	else if (lst2 == nullptr)
		return lst1;

	// recursively merging two lists
	if (lst1->data->getTitle() <= lst2->data->getTitle()) {
		result = lst1;
		result->next = mergedByTitleItems(lst1->next, lst2);
	}
	else {
		result = lst2;
		result->next = mergedByTitleItems(lst1, lst2->next);
	}
	return result;
}
Node<Customer*>* mergedByIdCustomers(Node<Customer*>* lst1, Node<Customer*>* lst2) {
	Node<Customer*>* result = nullptr;
	// Base Cases
	if (lst1 == nullptr)
		return lst2;
	else if (lst2 == nullptr)
		return lst1;

	// recursively merging two lists
	if (lst1->data->getId() <= lst2->data->getId()) {
		result = lst1;
		result->next = mergedByIdCustomers(lst1->next, lst2);
	}
	else {
		result = lst2;
		result->next = mergedByIdCustomers(lst1, lst2->next);
	}
	return result;
}
Node<Customer*>* mergedByNameCustomers(Node<Customer*>* lst1, Node<Customer*>* lst2) {
	Node<Customer*>* result = nullptr;
	// Base Cases
	if (lst1 == nullptr)
		return lst2;
	else if (lst2 == nullptr)
		return lst1;

	// recursively merging two lists
	if (lst1->data->getName() <= lst2->data->getName()) {
		result = lst1;
		result->next = mergedByNameCustomers(lst1->next, lst2);
	}
	else {
		result = lst2;
		result->next = mergedByNameCustomers(lst1, lst2->next);
	}
	return result;
}

void sortItemsById(Node<Item*>** headRef) {
	Node<Item*>* head = *headRef;
	Node<Item*>* ptr1;
	Node<Item*>* ptr2;

	// Base Case
	if ((head == nullptr) || (head->next == nullptr)) {
		return;
	}

	// Splitting list
	splitList<Item*>(head, &ptr1, &ptr2);

	// Recursively sorting two lists.
	sortItemsById(&ptr1);
	sortItemsById(&ptr2);

	// Sorted List.
	*headRef = mergedByIdItems(ptr1, ptr2);
}
// test cai//oke test di
void sortItemsByTitle(Node<Item*>** headRef) {
	Node<Item*>* head = *headRef;
	Node<Item*>* ptr1;
	Node<Item*>* ptr2;

	// Base Case
	if ((head == nullptr) || (head->next == nullptr)) {
		return;
	}

	// Splitting list
	splitList<Item*>(head, &ptr1, &ptr2);

	// Recursively sorting two lists.
	sortItemsByTitle(&ptr1);
	sortItemsByTitle(&ptr2);

	// Sorted List.
	*headRef = mergedByTitleItems(ptr1, ptr2);
}
void sortCustomersById(Node<Customer*>** headRef) {
	Node<Customer*>* head = *headRef;
	Node<Customer*>* ptr1;
	Node<Customer*>* ptr2;
	// Base Case
	if ((head == nullptr) || (head->next == nullptr)) {
		return;
	}
	// Splitting list
	splitList<Customer*>(head, &ptr1, &ptr2);
	// Recursively sorting two lists.
	sortCustomersById(&ptr1);
	sortCustomersById(&ptr2);
	// Sorted List.
	*headRef = mergedByIdCustomers(ptr1, ptr2);
}
void sortCustomersByName(Node<Customer*>** headRef) {
	Node<Customer*>* head = *headRef;
	Node<Customer*>* ptr1;
	Node<Customer*>* ptr2;
	// Base Case
	if ((head == nullptr) || (head->next == nullptr)) {
		return;
	}
	// Splitting list
	splitList<Customer*>(head, &ptr1, &ptr2);
	// Recursively sorting two lists.
	sortCustomersByName(&ptr1);
	sortCustomersByName(&ptr2);
	// Sorted List.
	*headRef = mergedByNameCustomers(ptr1, ptr2);
};

