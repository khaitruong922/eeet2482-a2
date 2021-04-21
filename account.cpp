#include <iostream>
#include <string>
#include <fstream>
#include "Item.cpp"
#include "List.cpp"
using namespace std;

#ifndef ACCOUNT
#define ACCOUNT
class Account
{
private:
	string id;
	string name;
	string address;
	string phoneNumber;
	List<Item*>* items;
public:
	//Destructror
	Account() {
		items = new List<Item*>();
	}
	Account(const string& id, const string& name, const string& address, const string& phoneNumber) {
		this->id = id;
		this->name = name;
		this->address = address;
		this->phoneNumber = phoneNumber;
	}
	~Account() {
		cout << "Destroy account " << name << endl;
		delete items;
	}
	// Getters
	string getId()const { return id; }
	string getName()const { return name; }
	string getAddress()const { return address; }
	string getPhoneNumber()const { return phoneNumber; }
	List<Item*>* getItems() { return items; }
	// Setters
	void setId(const string& id) { this->id = id; }
	void setName(const string& name) { this->name = name; }
	void setAddress(const string& address) { this->address = address; }
	void setPhoneNumber(const string& phoneNumber) { this->phoneNumber = phoneNumber; }
	// Methods
};
class GuestAccount : public Account
{
};
class RegularAccount : public Account
{

};
class VIPAccount : public Account
{

};

#endif