
#pragma once

#include <iostream>
#include "Item.h"
#include "List.h"
using namespace std;

class Customer
{
private:
	string id;
	string name;
	string address;
	string phoneNumber;
	string type;
	int numberOfReturns;
	List<string>* rentalIds;
protected:
	int points;
public:
	Customer(const string& id, const string& name, const string& address, const string& phoneNumber, const string type);
	~Customer() { delete rentalIds; }
	// Getters
	string getId()const { return id; }
	string getName()const { return name; }
	string getAddress()const { return address; }
	int getNumberOfReturns() const { return numberOfReturns; }
	int getPoints() const { return points; }
	string getPhoneNumber()const { return phoneNumber; }
	string getType() const { return type; }
	int getNumberOfRentals() const { return rentalIds->getSize(); }
	List<string>* getRentalIds() const { return rentalIds; }
	// To string
	string toString() const;
	string toConsoleString() const;
	string getItemIdsString() const;
	string toFileContent() const;
	// Setters
	void setId(const string& id) { this->id = id; }
	void setName(const string& name) { this->name = name; }
	void setAddress(const string& address) { this->address = address; }
	void setPhoneNumber(const string& phoneNumber) { this->phoneNumber = phoneNumber; }
	// Methods
	void addPoints(int points) { this->points += points; }
	void consumePoints(int points) { this->points -= points; }
	void rentItem(const string& itemId);
	bool returnItem(const string& itemId);
	virtual bool isPromotable() const { return numberOfReturns >= 3; }
	virtual bool canRentForFree() const { return false; }
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
public:
	VIPCustomer(const string& id, const string& name, const string& address, const string& phoneNumber) : Customer(id, name, address, phoneNumber, "VIP") {
	}
	bool isPromotable() const { return false; }
	bool canRentForFree() const { return points >= 100; }
};