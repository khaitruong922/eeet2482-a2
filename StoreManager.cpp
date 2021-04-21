#include <iostream>
#include <string>
#include <fstream>
#include "Account.cpp"
#include "validator.h"

using namespace std;

#ifndef STORE_MANAGER
#define STORE_MANAGER
class StoreManager {
private:
	string customers_file_name;
	string items_file_name;
	List<Account*>* accounts;
	List<Item*>* items;
public:
	~StoreManager() {
		cout << "Delete store manager" << endl;
		// TODO: delete accounts pointers
		// TODO: delete items pointers
		delete accounts;
		delete items;
	}
	StoreManager(const string& customers_file_name, const string& items_file_name) {
		this->customers_file_name = customers_file_name;
		this->items_file_name = items_file_name;
		accounts = new List<Account*>();
		items = new List<Item*>();
	}
	bool loadDataFromFiles() {
		if (!loadItemsFromFile()) return false;
		if (!loadCustomersFromFile()) return false;
		return true;
	}
	bool loadCustomersFromFile() {
		ifstream infile(customers_file_name);
		if (!infile)
		{
			cout << "Cannot open file " << customers_file_name << endl;
			return false;
		}
		cout << "Customers file: " << customers_file_name << endl;

		infile.close();
		return true;
	}
	bool loadItemsFromFile() {
		ifstream infile(items_file_name);
		if (!infile)
		{
			cout << "Cannot open file " << items_file_name << endl;
			return false;
		}
		cout << "Items file: " << items_file_name << endl;
		string line;
		string field;
		string delim = ",";
		while (getline(infile, line)) {
			int start = 0;
			int end = line.find(delim);
			int i = 0;
			string fields[7];
			// Loop until there are 6 fields
			while (end != string::npos && i < 7) {
				string field = line.substr(start, end - start);
				start = end + delim.length();
				end = line.find(delim, start);
				fields[i] = field;
				i++;
			}
			// Get last field
			fields[i] = line.substr(start, end - start);
			i++;
			// Skip the line if there are invalid number of fields
			if (i < 6) continue;
			// Skip the line if number of copies <= 0
			if (!isNonNegativeInteger(fields[4])) continue;
			if (!isNonNegativeNumber(fields[5])) continue;

			// Without genre: 6, with genre: 7
			string id = fields[0];
			string title = fields[1];
			string rentalType = fields[2];
			string loanType = fields[3];
			int numberOfCopies = stoi(fields[4]);
			double rentalFee = stod(fields[5]);
			string genre = fields[6];
			Item* item = createItem(id, title, rentalType, loanType, numberOfCopies, rentalFee, genre);
			// Skip if the item cannot be created
			if (item == nullptr) continue;

			// TODO: Check if item ID exists
			if (false) {
				delete item;
			}
			items->add(item);
		}
		infile.close();
		return true;
	}

	void saveData() {
		saveItems();
		saveCustomers();
	}
	void saveCustomers() {
		ofstream outfile(customers_file_name);
		if (!outfile)
		{
			cout << "Cannot save file " << customers_file_name << endl;
		}
	}
	void saveItems() {
		ofstream outfile(items_file_name);
		if (!outfile)
		{
			cout << "Cannot save file " << items_file_name << endl;
		}
	}

	Item* createItem(const string& id, const string& title, const string& rentalType, const string& loanType, int numberOfCopies, double rentalFee, const string& genre) {
		if (rentalType == "Record") {
			return new Record(id, title, loanType, numberOfCopies, rentalFee, genre);
		}
		else if (rentalType == "DVD") {
			return new DVD(id, title, loanType, numberOfCopies, rentalFee, genre);
		}
		else if (rentalType == "Game") {
			return new Game(id, title, loanType, numberOfCopies, rentalFee);
		}
		else {
			// Skip the line if the rental type is not valid
			return nullptr;
		}
	}


	// Menu functions

	  // 1
	// 1.1
	void addItem()
	{
		cout << "-------------------------------" << endl;
		cout << "Select item type" << endl;
		cout << "1. Record" << endl;
		cout << "2. DVD" << endl;
		cout << "3. Game" << endl;

		bool valid = true;
		string optionInput;
		int option;
		do {
			if (!valid) {
				cout << "Invalid input!" << endl;
			}
			cout << "Your option: ";
			cin >> optionInput;
			valid = isNonNegativeInteger(optionInput);
			if (!valid) continue;
			option = stoi(optionInput);
			valid = option >= 1 && option <= 3;
		} while (!valid);

		string rentalType = "";
		switch (option)
		{
		case 1:
			rentalType = "Record";
			break;
		case 2:
			rentalType = "DVD";
			break;
		case 3:
			rentalType = "Game";
			break;
		}
		string id, title, loanType, numberOfCopiesInput, rentalFeeInput, genre;
		do {
			if (!valid) {
				cout << "Please match the following format: Ixxx-yyyy (xxx: unique 3-digit code, yyyy: published year)" << endl;
			}
			cout << "Item ID: ";
			cin >> id;
			valid = isValidItemId(id);
			// Check unique id
		} while (!valid);

		cout << "Title: ";
		cin >> title;

		cout << "Loan type: ";
		cin >> loanType;

		do {
			if (!valid) {
				cout << "Number of copies must be a non-negative integer!" << endl;
			}
			cout << "Number of copies: ";
			cin >> numberOfCopiesInput;
			valid = isNonNegativeInteger(numberOfCopiesInput);
		} while (!valid);

		do {
			if (!valid) {
				cout << "Rental fee must be a non-negative number!" << endl;
			}
			cout << "Rental fee: ";
			cin >> rentalFeeInput;
			valid = isNonNegativeNumber(rentalFeeInput);
		} while (!valid);

		if (rentalType != "Game") {
			cout << "Genre: ";
			cin >> genre;
		}

		int numberOfCopies = stoi(numberOfCopiesInput);
		double rentalFee = stod(rentalFeeInput);

		Item* item = createItem(id, title, rentalType, loanType, numberOfCopies, rentalFee, genre);

		if (item == nullptr) {
			cout << "Cannot add item " << id << endl;
			return;
		}

		items->add(item);
		cout << "Add item " << id << " succesfully!" << endl;;
	}

	// 1.2
	void updateItem()
	{
		cout << "Update item" << endl;
	}

	// 1.3
	void deleteItem()
	{
		cout << "Delete item" << endl;
	}

	// Menu
	void displayItemMenu()
	{
		cout << "-------------------------------" << endl;
		cout << "Manage items" << endl;
		cout << "1. Add item" << endl;
		cout << "2. Update item" << endl;
		cout << "3. Delete item" << endl;
		cout << "4. Back" << endl;

		bool valid = true;
		string optionInput;
		int option;
		do {
			if (!valid) {
				cout << "Invalid input!" << endl;
			}
			cout << "Your option: ";
			cin >> optionInput;
			valid = isNonNegativeInteger(optionInput);
			if (!valid) continue;
			option = stoi(optionInput);
			valid = option >= 1 && option <= 4;
		} while (!valid);

		if (option == 4)
			return;
		switch (option) {
		case 1:
			addItem();
			break;
		case 2:
			updateItem();
			break;
		case 3:
			deleteItem();
			break;
		default:
			cout << "Please enter an option in range [1-4]!" << endl;
			break;
		}
		displayItemMenu();
	}

	// 2
	// 2.1
	void addCustomer()
	{
		cout << "Add customer" << endl;
	}

	// 2.2
	void updateCustomer()
	{
		cout << "Update customer" << endl;
	}

	// Menu
	void displayCustomerMenu()
	{
		cout << "-------------------------------" << endl;
		cout << "Manage customers" << endl;
		cout << "Enter an option below" << endl;
		cout << "1. Add customer" << endl;
		cout << "2. Update customer" << endl;
		cout << "3. Back" << endl;

		bool valid = true;
		string optionInput;
		int option;
		do {
			if (!valid) {
				cout << "Invalid input!" << endl;
			}
			cout << "Your option: ";
			cin >> optionInput;
			valid = isNonNegativeInteger(optionInput);
			if (!valid) continue;
			option = stoi(optionInput);
			valid = option >= 1 && option <= 3;
		} while (!valid);

		if (option == 3)
			return;
		switch (option) {
		case 1:
			addCustomer();
			break;
		case 2:
			updateCustomer();
			break;
		default:
			cout << "Please enter an option in range [1-3]!" << endl;
			break;
		}
		displayCustomerMenu();
	}

	// 3
	void promoteCustomer()
	{
		cout << "Promote customer" << endl;
	}

	// 4
	void rentItem()
	{
		cout << "Rent item" << endl;
	}

	// 5
	void returnItem()
	{
		cout << "Return item" << endl;
	}

	// 6
	void displayAllItems()
	{
		auto head = items->getHead();
		while (head) {
			cout << head->data->toString() << endl;
			head = head->next;
		}
	}

	// 7
	void displayOutOfStockItems()
	{
		cout << "Display out of stock items" << endl;
	}

	//8
	void displayAllCustomers()
	{
		cout << "Display all customers" << endl;
	}

	// 9
	void displayGroupOfCustomers()
	{
		cout << "Display group of customers" << endl;
	}

	// 10
	void displaySearchMenu()
	{
		cout << "Display search menu" << endl;
	}

	// Exit
	void exit()
	{
		cout << "ASSIGNMENT 2 GROUP 26" << endl;
		cout << "s3818074, s3818074@rmit.edu.vn, Khai, Truong" << endl;
		cout << "s3878636, s3878636@rmit.edu.vn, Quy, Nguyen" << endl;
		cout << "s3877746, s3877746@rmit.edu.vn, Thuy, Mai" << endl;
		cout << "s3905273, s3905273@rmit.edu.vn, Nam, Pham" << endl;
		saveData();
	}

	void displayMenu()
	{
		// Print out menu options
		cout << "-------------------------------" << endl;
		cout << "Welcome to Genie's video store" << endl;
		cout << "Enter an option below" << endl;
		cout << "1. Add a new item, update or delete an existing one" << endl;
		cout << "2. Add new customer or update an existing customer" << endl;
		cout << "3. Promote an existing customer" << endl;
		cout << "4. Rent an item" << endl;
		cout << "5. Return an item" << endl;
		cout << "6. Display all items" << endl;
		cout << "7. Display out-of-stock items" << endl;
		cout << "8. Display all customers" << endl;
		cout << "9. Display group of customers" << endl;
		cout << "10. Search items or customers" << endl;
		cout << "Type Exit to quit the program" << endl;
		// Ask for user input
		bool valid = true;
		string optionInput;
		int option;
		do {
			if (!valid) {
				cout << "Invalid input!" << endl;
			}
			cout << "Your option: ";
			cin >> optionInput;
			if (optionInput == "Exit") {
				exit();
				return;
			}
			valid = isNonNegativeInteger(optionInput);
			if (!valid) continue;
			option = stoi(optionInput);
			valid = option >= 1 && option <= 10;
		} while (!valid);

		switch (option) {
		case 1:
			displayItemMenu();
			break;
		case 2:
			displayCustomerMenu();
			break;
		case 3:
			promoteCustomer();
			break;
		case 4:
			rentItem();
			break;
		case 5:
			returnItem();
			break;
		case 6:
			displayAllItems();
			break;
		case 7:
			displayOutOfStockItems();
			break;
		case 8:
			displayAllCustomers();
			break;
		case 9:
			displayGroupOfCustomers();
			break;
		case 10:
			displaySearchMenu();
			break;
		}
		displayMenu();
	}
};
#endif