#pragma warning(disable : 4996)
#include <iostream>
#include <sstream>
#include "Item.h"
using namespace std;
Item::Item(const string& id, const string& title, const string& rentalType, const string& loanType, int totalNumberOfCopies, double rentalFee, string genre) {
    this->id = id;
    this->title = title;
    this->rentalType = rentalType;
    this->loanType = loanType;
    this->totalNumberOfCopies = totalNumberOfCopies;
    this->currentNumberOfCopies = totalNumberOfCopies;
    this->rentalFee = rentalFee;
    this->genre = genre;
}

// Return the rental fee with 2 decimal points
string Item::getRentalFeeString() const {
    int str_len = to_string(rentalFee).length();
    char* cptr = new char[str_len];
    sprintf(cptr, "%.2lf", rentalFee);
    string s = cptr;
    delete[] cptr;
    return s;
}

string Item::toString() const
{
    stringstream ss;
    ss << id << "," << title << "," << rentalType << "," << loanType << "," << totalNumberOfCopies << "," << getRentalFeeString();
    if (genre != "") ss << "," << genre;
    ss << endl;
    return ss.str();
};

string Item::toConsoleString() const {
    return toString();
}

string Item::toFileContent() const
{
    return toString();
};