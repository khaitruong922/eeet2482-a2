#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Item
{
private:
    string id;
    string title;
    string rentType;
    string loanType;
    int numberOfCopies;
    double rentalFee;
    string genre;

public:
    // Getters
    string getId() { return id; }
    string getTitle() { return title; }
    string getRentType() { return rentType; }
    string getLoanType() { return loanType; }
    int getNumberOfCopies() { return numberOfCopies; }
    double getRentalFee() { return rentalFee; }
    string getGenre() { return genre; }
    // Setters
    void setId(string id) { this->id = id; }
    void setTitle(string title) { this->title = title; }
    void setRentType(string rentType) { this->rentType = rentType; }
    void setLoanType(string loanType) { this->loanType = loanType; }
    void setNumberOfCopies(int numberOfCopies) { this->numberOfCopies = numberOfCopies; }
    void setRentalFee(double rentalFee) { this->rentalFee = rentalFee; }
    void setGenre(string genre) { this->genre = genre; }



};
class OldMovieRecord : public Item
{

};

class DVD : public Item
{

};

class VideoGame : public Item
{

};