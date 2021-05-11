#pragma once

#include <iostream>
#include "validator.h"
#include "List.cpp"

using namespace std;

class Item
{
private:
	string id;
	string title;
	string rentalType;
	string loanType;
	int totalNumberOfCopies;
	int currentNumberOfCopies;
	double rentalFee;
	string genre;

public:
	Item(const string& id, const string& title, const string& rentalType, const string& loanType, int totalNumberOfCopies, double rentalFee, const string& genre);
	// Getters
	string getId()const { return id; }
	string getTitle()const { return title; }
	string getRentalType()const { return rentalType; }
	string getLoanType()const { return loanType; }
	int getTotalNumberOfCopies()const { return totalNumberOfCopies; }
	int getCurrentNumberOfCopies() const { return currentNumberOfCopies; }
	double getRentalFee()const { return rentalFee; }
	string getGenre()const { return genre; }
	bool isAvailable()const { return currentNumberOfCopies > 0; }
	// Setters
	void setId(const string& id) { this->id = id; }
	void setTitle(const string& title) { this->title = title; }
	void setRentalType(const string& rentalType) { this->rentalType = rentalType; }
	void setLoanType(const string& loanType) { this->loanType = loanType; }
	void setRentalFee(double rentalFee) { this->rentalFee = rentalFee; }
	void setGenre(const string& genre) { this->genre = genre; }
	// Methods
	void addToStock(int numberOfCopies) {
		this->totalNumberOfCopies += numberOfCopies;
		this->currentNumberOfCopies += numberOfCopies;
	}
	void rented() { currentNumberOfCopies--; }
	void returned() { currentNumberOfCopies++; }

	string getRentalFeeString() const;
	string toString() const;
	string toConsoleString() const;
	string toFileContent() const;
};

class Record : public Item
{
public:
	Record(const string& id, const string& title, const string& loanType, int totalNumberOfCopies, double rentalFee, const string& genre) :Item(id, title, "Record", loanType, totalNumberOfCopies, rentalFee, genre) {

	}
};

class DVD : public Item
{
public:
	DVD(const string& id, const string& title, const string& loanType, int totalNumberOfCopies, double rentalFee, const string& genre) :Item(id, title, "DVD", loanType, totalNumberOfCopies, rentalFee, genre) {

	}
};

class Game : public Item
{
public:
	Game(const string& id, const string& title, const string& loanType, int totalNumberOfCopies, double rentalFee) :Item(id, title, "Game", loanType, totalNumberOfCopies, rentalFee, "") {

	}
};
