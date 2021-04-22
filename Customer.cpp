#include <iostream>
#include <string>
#include <fstream>
#include "Item.cpp"
#include "List.cpp"
using namespace std;

#ifndef CUSTOMER
#define CUSTOMER
class Customer
{
private:
	string id;
	string name;
	string address;
	string phoneNumber;
	string type;
	List<string>* itemIds;
public:
	Customer(const string& id, const string& name, const string& address, const string& phoneNumber, const string& type) {
		this->id = id;
		this->name = name;
		this->address = address;
		this->phoneNumber = phoneNumber;
		this->type = type;
		itemIds = new List<string>();
	}
	~Customer() {
		cout << "Destroy account " << name << endl;
		delete itemIds;
	}
	// Getters
	string getId()const { return id; }
	string getName()const { return name; }
	string getAddress()const { return address; }
	string getPhoneNumber()const { return phoneNumber; }
	string getType() const { return type; }
	List<string>* getItemIds() { return itemIds; }
	// Setters
	void setId(const string& id) { this->id = id; }
	void setName(const string& name) { this->name = name; }
	void setAddress(const string& address) { this->address = address; }
	void setPhoneNumber(const string& phoneNumber) { this->phoneNumber = phoneNumber; }
	// Methods
	string toString() {
		// string items_str;
		// auto head = itemIds->getHead();
		// while (head){
		//   items_str += head->data + "\n";
		//   head = head->next;
		// }
		return id + "," + name + "," + address + "," + phoneNumber + "," + to_string(itemIds->getSize()) + "," + type + "\n";
	}
};
class GuestCustomer : public Customer
{
public:
	GuestCustomer(const string& id, const string& name, const string& address, const string& phoneNumber) : Customer(id, name, address, phoneNumber, "Guest") {

	}
};
class RegularCustomer : public Customer
{
public:
	RegularCustomer(const string& id, const string& name, const string& address, const string& phoneNumber) : Customer(id, name, address, phoneNumber, "Regular") {

	}
};
class VIPCustomer : public Customer
{
private:
	int point;
public:
public:
	VIPCustomer(const string& id, const string& name, const string& address, const string& phoneNumber) : Customer(id, name, address, phoneNumber, "VIP") {
		this->point = 0;
	}
	int getPoint() const { return point; }
};

#endif