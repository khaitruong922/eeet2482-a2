#pragma once
#include <iostream>
#include "Item.h"
#include "Customer.h"
#include "List.h"
using namespace std;

template<typename T>
void splitList(Node<T>* source, Node<T>** front, Node<T>** back);
Node<Item*>* mergedByIdItems(Node<Item*>* lst1, Node<Item*>* lst2);
Node<Item*>* mergedByTitleItems(Node<Item*>* lst1, Node<Item*>* lst2);
Node<Customer*>* mergedByIdCustomers(Node<Customer*>* lst1, Node<Customer*>* lst2);
Node<Customer*>* mergedByNameCustomers(Node<Customer*>* lst1, Node<Customer*>* lst2);
void sortItemsById(Node<Item*>** headRef);
void sortItemsByTitle(Node<Item*>** headRef);
void sortCustomersById(Node<Customer*>** headRef);
void sortCustomersByName(Node<Customer*>** headRef);