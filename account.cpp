#include <iostream>
#include <string>
#include <fstream>
#include "item.cpp"
using namespace std;

class Account
{
private:
	string id;
	string name;
	string address;
	string phoneNumber;
	Item item[100];
public:
	// Getters
	string getId() { return id; }
	string getName() { return name; }
	string getAddress() { return address; }
	string getPhoneNumber() { return phoneNumber; }
	// Setters
	void setId(string id) { this->id = id; }
	void setName(string name) { this->name = name; }
	void setAddress(string address) { this->address = address; }
	void setPhoneNumber(string phoneNumber) { this->phoneNumber = phoneNumber; }
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