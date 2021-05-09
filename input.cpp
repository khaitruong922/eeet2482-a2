#include <iostream>
#include <string>
#include "input.h"
#include "validator.h"
using namespace std;

string getItemIdInput() {
    string input;
    while (true) {
        cout << "Item ID: ";
        getline(cin, input);
        input = trim(input);
        if (isValidItemId(input)) break;
        cout << "Please match the following format: Ixxx-yyyy (xxx: code, yyyy: year published)" << endl;
    }
    return input;
};

string getItemTitleInput()
{
    string input;
    while (true) {
        cout << "Title: ";
        getline(cin, input);
        input = trim(input);
        if (isEmpty(input)) {
            cout << "Please enter the title!" << endl;
            continue;
        }
        if (hasComma(input)) {
            cout << "Comma is not allowed!" << endl;
            continue;
        }
        break;
    }
    return input;
};

string getItemLoanTypeInput()
{
    cout << "Select loan type" << endl;
    cout << "1. 2-day" << endl;
    cout << "2. 1-week" << endl;
    int option = getOptionInput(1, 2);
    if (option == 1) return "2-day";
    if (option == 2) return "1-week";
    return "";
}

int getItemNumberOfCopiesInput()
{
    string input;
    while (true)
    {
        cout << "Number of copies: ";
        getline(cin, input);
        input = trim(input);
        if (isNonNegativeInteger(input)) break;
        cout << "Number of copies must be a non-negative integer!" << endl;
    }
    return stoi(input);
};

double getItemRentalFeeInput()
{
    string input;
    while (true)
    {
        cout << "Rental fee($): ";
        getline(cin, input);
        input = trim(input);
        if (isNonNegativeNumber(input)) break;
        cout << "Rental fee must be a non-negative number!" << endl;
    }
    return stod(input);
};

string getItemGenreInput()
{
    cout << "Select genre" << endl;
    cout << "1. Action" << endl;
    cout << "2. Horror" << endl;
    cout << "3. Drama" << endl;
    cout << "4. Comedy" << endl;
    int option = getOptionInput(1, 4);
    if (option == 1) return "Action";
    if (option == 2) return "Horror";
    if (option == 3) return "Drama";
    if (option == 4) return "Comedy";
    return "";
};

string getCustomerIdInput() {
    string input;
    while (true) {
        cout << "Customer ID: ";
        getline(cin, input);
        input = trim(input);
        if (isValidCustomerId(input)) break;
        cout << "Please match the following format: Cxxx (xxx: 3-digit code)" << endl;
    }
    return input;
}

string getCustomerNameInput() {
    string input;
    while (true) {
        cout << "Name: ";
        getline(cin, input);
        input = trim(input);
        if (isEmpty(input)) {
            cout << "Please enter a name!" << endl;
            continue;
        }
        if (!isAlpha(input)) {
            cout << "Name can only contain alphabetic letters and space!" << endl;
            continue;
        }
        break;
    }
    return input;
}

string getCustomerAddressInput() {
    string input;
    while (true) {
        cout << "Address: ";
        getline(cin, input);
        input = trim(input);
        if (isEmpty(input)) {
            cout << "Please enter the address!" << endl;
            continue;
        }
        if (hasComma(input)) {
            cout << "Comma is not allowed!" << endl;
            continue;
        }
        break;
    }
    return input;
}

string getCustomerPhoneNumberInput() {
    string input;
    while (true) {
        cout << "Phone number: ";
        getline(cin, input);
        input = trim(input);
        if (isEmpty(input)) {
            cout << "Please enter a phone number!" << endl;
            continue;
        }
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
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        int option = getOptionInput(1, 2);
        if (option == 1) return true;
        if (option == 2) return false;
        return false;
    }
}

int getOptionInput(int min, int max) {
    string input;
    while (true) {
        cout << "Your option: ";
        getline(cin, input);
        input = trim(input);
        if (!isNonNegativeNumber(input)) {
            cout << "Invalid input!" << endl;
            continue;
        }
        int option = stoi(input);
        if (option >= min && option <= max) break;
        cout << "Please select an option in range [" << min << "-" << max << "]" << endl;
    }
    return stoi(input);
}

int getOptionInput(int min, int max, string exitString) {
    string input;
    while (true) {
        cout << "Your option: ";
        getline(cin, input);
        input = trim(input);
        if (input == exitString) return max + 1;
        if (!isNonNegativeNumber(input)) {
            cout << "Invalid input!" << endl;
            continue;
        }
        int option = stoi(input);
        if (option >= min && option <= max) return stoi(input);
        cout << "Please select an option in range [" << min << "-" << max << "]" << endl;
    }
    return stoi(input);
}
void waitForEnter() {
    string s;
    cout << "Press Enter to continue...";
    getline(cin, s);
}
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