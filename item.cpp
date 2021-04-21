#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include "validator.h"
#include "List.cpp"
using namespace std;

#ifndef ITEM
#define ITEM

class Item
{
private:
	string id;
	string title;
	string rentalType;
	string loanType;
	int numberOfCopies;
	double rentalFee;
	string genre;
	bool available;

public:
	//Destructor
	~Item() {
		cout << "Destroy item " << title << endl;
	}
	// Constructor
	Item() {

	}
	Item(const string& id, const string& title, const string& rentalType, const string& loanType, int numberOfCopies, double rentalFee, string genre) {
		this->id = id;
		this->title = title;
		this->rentalType = rentalType;
		this->loanType = loanType;
		this->numberOfCopies = numberOfCopies;
		this->rentalFee = rentalFee;
		this->genre = genre;
		this->available = true;
	}
	// Getters
	string getId()const { return id; }
	string getTitle()const { return title; }
	string getRentalType()const { return rentalType; }
	string getLoanType()const { return loanType; }
	int getNumberOfCopies()const { return numberOfCopies; }
	double getRentalFee()const { return rentalFee; }
	string getGenre()const { return genre; }
	bool isAvailable()const { return available; }
	// Setters

	void setId(const string& id) { this->id = id; }
	void setTitle(const string& title) { this->title = title; }
	void setRentalType(const string& rentalType) { this->rentalType = rentalType; }
	void setLoanType(const string& loanType) { this->loanType = loanType; }
	void setNumberOfCopies(int numberOfCopies) { this->numberOfCopies = numberOfCopies; }
	void setRentalFee(double rentalFee) { this->rentalFee = rentalFee; }
	void setGenre(string genre) { this->genre = genre; }
	void setAvailable(bool available) { this->available = available; }

	// Methods
	string toString() {
		return id + "," + title + "," + rentalType + "," + loanType + "," + to_string(numberOfCopies) + "," + to_string(rentalFee) + "," + genre;
	};



};
class Record : public Item
{
public:
	Record(const string& id, const string& title, const string& loanType, int numberOfCopies, double rentalFee, string genre) :Item(id, title, "Record", loanType, numberOfCopies, rentalFee, genre) {

	}
};

class DVD : public Item
{
public:
	DVD(const string& id, const string& title, const string& loanType, int numberOfCopies, double rentalFee, string genre) :Item(id, title, "DVD", loanType, numberOfCopies, rentalFee, genre) {

	}
};

class Game : public Item
{
public:
	Game(const string& id, const string& title, const string& loanType, int numberOfCopies, double rentalFee) :Item(id, title, "Game", loanType, numberOfCopies, rentalFee, "") {

	}
};

#endif