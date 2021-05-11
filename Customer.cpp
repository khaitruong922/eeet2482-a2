#include <iostream>
#include <sstream>
#include "Customer.h"
using namespace std;

Customer::Customer(const string& id, const string& name, const string& address, const string& phoneNumber, const string& type) {
	this->id = id;
	this->name = name;
	this->address = address;
	this->phoneNumber = phoneNumber;
	this->type = type;
	this->numberOfReturns = 0;
	this->points = 100;
	rentalIds = new List<string>();
}

string Customer::toString() const {
	stringstream ss;
	ss << id << "," << name << "," << address << "," << phoneNumber << "," << getNumberOfRentals() << "," << type << endl;
	return ss.str();
}

string Customer::getItemIdsString() const {
	stringstream ss;
	auto current = rentalIds->getHead();
	while (current) {
		ss << current->data << endl;
		current = current->next;
	}
	return ss.str();
}

string Customer::toConsoleString() const {
	stringstream ss;
	ss << "Customer: " << toString();
	ss << "Number of rentals: " << getRentalIds()->getSize() << endl;
	ss << getItemIdsString();
	return ss.str();
}

string Customer::toFileContent() const {
	stringstream ss;
	ss << toString() << getItemIdsString();
	return ss.str();
}

void Customer::rentItem(const string& itemId) {
	rentalIds->add(itemId);
}

bool Customer::returnItem(const string& itemId) {
	int i = 0;
	auto node = rentalIds->getHead();
	while (node) {
		// If the item can be found
		if (node->data == itemId) {
			rentalIds->deleteNode(i);
			numberOfReturns++;
			addPoints(10);
			return true;
		}
		node = node->next;
		i++;
	}
	// If the item cannot be found
	return false;
}
