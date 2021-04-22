#include <iostream>
#include <string>
#include "input.h"
#include "validator.h"
using namespace std;

string getItemIdInput(){
  string input;
  while(true){
    cout << "Item ID: ";
    getline(cin,input);
    if (isValidItemId(input)) return input;
    cout << "Please match the following format: Ixxx-yyyy (xxx: code, yyyy: year published)" << endl;
  }
  return input;
};

string getItemTitleInput()
{
  string input;
  while(true){
    cout << "Title: ";
    getline(cin,input);
    if(isNotEmpty(input)) return input;
    cout <<"Please enter the title!" << endl;
  }
  return input;
};

string getItemLoanTypeInput()
{
  string input;
  while(true){
    cout << "Loan type: ";
    getline(cin,input);
    if(isNotEmpty(input)) return input;
    cout << "Please enter the loan type!" << endl;
  }
  return input;
}

int getItemNumberOfCopiesInput()
{
  string input;
  while(true)
  {
    cout << "Number of copies: " ;
    getline(cin,input);
    if(isNonNegativeInteger(input)) return stoi(input);
    cout << "Number of copies must be a non-negative integer!" << endl;
  }
  return stoi(input);
};

double getItemRentalFeeInput()
{
  string input;
  while(true)
  {
    cout << "Rental fee: " ;
    getline(cin,input);
    if(isNonNegativeInteger(input)) return stod(input);
    cout << "Rental fee must be a non-negative number!" << endl;
  }
  return stod(input); 
};

string getItemGenreInput()
{
  string input;
  while(true)
  {
    cout << "Genre: " ;
    getline(cin,input);
    if(isNotEmpty(input)) return input;
    cout << "Please enter a genre!" << endl;
  }
  return input; 
};

string getCustomerIdInput(){
  string input;
  while(true){
    cout << "Customer ID: ";
    getline(cin,input);
    if (isValidCustomerId(input)) return input;
    cout << "Please match the following format: Cxxx (xxx: 3-digit code)" << endl;
  }
  return input;
}

string getCustomerNameInput(){
  string input;
  while(true){
    cout << "Name: ";
    getline(cin,input);
    if (isNotEmpty(input)) return input;
    cout << "Please enter a name!" << endl;
  }
  return input;
}

string getCustomerAddressInput(){
  string input;
  while(true){
    cout << "Address: ";
    getline(cin,input);
    if (isNotEmpty(input)) return input;
    cout << "Please enter an address" << endl;
  }
  return input;
}

string getCustomerPhoneNumberInput(){
  string input;
  while(true){
    cout << "Phone number: ";
    getline(cin,input);
    if (isNotEmpty(input)) return input;
    cout << "Please enter a phone number" << endl;
  }
  return input;
}

int getOptionInput(int min, int max){
    string input;
    while (true){
      cout << "Your option: ";
      getline(cin,input);
      if (!isNonNegativeNumber(input)){
        cout << "Invalid input!" << endl;
        continue;
      }
      int option = stoi(input);
      if (option >= min && option <= max) return stoi(input);
      cout << "Please select an option in range [" << min << "-" << max << "]" << endl; 
    }
    return stoi(input);
}