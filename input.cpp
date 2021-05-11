#include <iostream>
#include <string>
#include "input.h"
#include "validator.h"
using namespace std;

// This file contains all the functions which handle getting the input from the user until the input is valid

string getItemIdInput() {
    string input;
    while (true) {
        // Get the input
        cout << "Item ID: ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Break from the loop if the input is a valid item id
        if (isValidItemId(input)) break;
        // Display error message and require input again
        cout << "Please match the following format: Ixxx-yyyy (xxx: code, yyyy: year published)" << endl;
    }
    return input;
};

string getItemTitleInput()
{
    string input;
    while (true) {
        // Get the input
        cout << "Title: ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Require input again if it is empty
        if (isEmpty(input)) {
            cout << "Please enter the title!" << endl;
            continue;
        }
        // Require input again if it has comma
        if (hasComma(input)) {
            cout << "Comma is not allowed!" << endl;
            continue;
        }
        // Input is valid, break out from the loop
        break;
    }
    return input;
};

string getItemLoanTypeInput()
{
    // Display loan types to select
    cout << "Select loan type" << endl;
    cout << "1. 2-day" << endl;
    cout << "2. 1-week" << endl;
    // Get the option and return the loan type
    int option = getOptionInput(1, 2);
    if (option == 1) return "2-day";
    if (option == 2) return "1-week";
    // This should not be returned
    return "";
}

int getItemNumberOfCopiesInput()
{
    string input;
    while (true)
    {
        // Get the input
        cout << "Number of copies: ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Break out from the loop if the input is a non negative integer
        if (isNonNegativeInteger(input)) break;
        // Display error message and require input again
        cout << "Number of copies must be a non-negative integer!" << endl;
    }
    // Return the input converted to integer
    return stoi(input);
};

double getItemRentalFeeInput()
{
    string input;
    while (true)
    {
        // Get the input
        cout << "Rental fee($): ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Break out from the loop if the input is a non negative number
        if (isNonNegativeNumber(input)) break;
        // Display error message and require input again
        cout << "Rental fee must be a non-negative number!" << endl;
    }
    // Return the input converted to double
    return stod(input);
};

string getItemGenreInput()
{
    // Display genres to select
    cout << "Select genre" << endl;
    cout << "1. Action" << endl;
    cout << "2. Horror" << endl;
    cout << "3. Drama" << endl;
    cout << "4. Comedy" << endl;
    // Get the option and return the genre
    int option = getOptionInput(1, 4);
    if (option == 1) return "Action";
    if (option == 2) return "Horror";
    if (option == 3) return "Drama";
    if (option == 4) return "Comedy";
    // This should not be returned
    return "";
};

string getCustomerIdInput() {
    string input;
    while (true) {
        // Get the input
        cout << "Customer ID: ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Break out of the loop if the input is valid customer id
        if (isValidCustomerId(input)) break;
        // Display error message and require input again
        cout << "Please match the following format: Cxxx (xxx: 3-digit code)" << endl;
    }
    return input;
}

string getCustomerNameInput() {
    string input;
    while (true) {
        // Get the input
        cout << "Name: ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Require input again if the input is empty
        if (isEmpty(input)) {
            cout << "Please enter a name!" << endl;
            continue;
        }
        // Require input again if the input has characters other than alphabetic letters and space
        if (!isAlpha(input)) {
            cout << "Name can only contain alphabetic letters and space!" << endl;
            continue;
        }
        // Input is valid, break out from the loop
        break;
    }
    return input;
}

string getCustomerAddressInput() {
    string input;
    while (true) {
        // Get the input
        cout << "Address: ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Require input again if it is empty
        if (isEmpty(input)) {
            cout << "Please enter the address!" << endl;
            continue;
        }
        // Require input again if it has any comma
        if (hasComma(input)) {
            cout << "Comma is not allowed!" << endl;
            continue;
        }
        // Input is valid, break out of the loop
        break;
    }
    return input;
}

string getCustomerPhoneNumberInput() {
    string input;
    while (true) {
        // Get the input
        cout << "Phone number: ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Require input again if it is empty
        if (isEmpty(input)) {
            cout << "Please enter a phone number!" << endl;
            continue;
        }
        // Require input again if it has other characters other than digits
        if (!isNumeric(input)) {
            cout << "Phone number can only contain numbers!" << endl;
            continue;
        }
        break;
    }
    return input;
}

bool getYesNoInput() {
    string input;
    while (true) {
        // Display yes/no option
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        // Get the option and return a boolean (yes: true, no: false)
        int option = getOptionInput(1, 2);
        if (option == 1) return true;
        if (option == 2) return false;
        // This should no be returned
        return false;
    }
}

// Get the input between 2 integers
int getOptionInput(int min, int max) {
    string input;
    while (true) {
        // Get the input
        cout << "Your option: ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Require input again if the input is not a non negative integer
        if (!isNonNegativeInteger(input)) {
            cout << "Invalid input!" << endl;
            continue;
        }
        // Convert input to integer
        int option = stoi(input);
        // If the input is in range [min,max], break out of the loop
        if (option >= min && option <= max) break;
        // Display error message and require input again
        cout << "Please select an option in range [" << min << "-" << max << "]" << endl;
    }
    // Return the input converted to integer
    return stoi(input);
}

// Overloaded version of the above function, which can accept a special string which will return the value of max+1
int getOptionInput(int min, int max, const string& exitString) {
    string input;
    while (true) {
        // Get the input
        cout << "Your option: ";
        getline(cin, input);
        // Trim the input
        input = trim(input);
        // Return max+1 the input is the special string
        if (input == exitString) return max + 1;
        // Require input again if the input is not a non negative integer
        if (!isNonNegativeInteger(input)) {
            cout << "Invalid input!" << endl;
            continue;
        }
        // Convert input to integer
        int option = stoi(input);
        // If the input is in range [min,max], break out of the loop
        if (option >= min && option <= max) return stoi(input);
        // Display error message and require input again
        cout << "Please select an option in range [" << min << "-" << max << "]" << endl;
    }
    // Return the input converted to integer
    return stoi(input);
}

// This function is used for pausing the menu and wait for user Enter to proceed
void waitForEnter() {
    string s;
    cout << "Press Enter to continue...";
    // Wait for a line, which will be invoked after pressing Enter
    getline(cin, s);
}

// This function is used for trimming the leading and trailing whitespace
string trim(const string& s)
{
    // If the string contains only whitespace, return the empty string
    if (s.find_first_not_of(' ') == string::npos) return "";
    size_t first = s.find_first_not_of(' ');
    if (string::npos == first)
    {
        return s;
    }
    size_t last = s.find_last_not_of(' ');
    return s.substr(first, (last - first + 1));
}