#include <iostream>
#include <string>
#include <fstream>
#include "account.cpp"
using namespace std;

bool isNumber(string s)
{
	for (int i = 0; i < s.length(); i++)
		if (isdigit(s[i]) == false)
			return false;
	return true;
}

// 1
// 1.1
void addItem()
{
	cout << "Add item" << endl;
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
	cout << "Enter an option below" << endl;
	cout << "1. Add item" << endl;
	cout << "2. Update item" << endl;
	cout << "3. Delete item" << endl;
	cout << "4. Back" << endl;
	cout << "Your option: ";
	// Ask for user input
	string input;
	cin >> input;
	// Validate input
	if (!isNumber(input)) {
		cout << "Invalid input!" << endl;
		displayItemMenu();
		return;
	}
	// Convert input to integer
	int option = stoi(input);
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
	cout << "Your option: ";
	// Ask for user input
	string input;
	cin >> input;
	// Validate input
	if (!isNumber(input)) {
		cout << "Invalid input!" << endl;
		displayCustomerMenu();
		return;
	}
	// Convert input to integer
	int option = stoi(input);
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
	cout << "Display all items" << endl;
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
	cout << "Your option: ";
	// Ask for user input
	string input;
	cin >> input;
	// Handle exit
	if (input.compare("Exit") == 0) {
		exit();
		return;
	}
	// Validate input
	if (!isNumber(input)) {
		cout << "Invalid input!" << endl;
		displayMenu();
		return;
	}
	// Convert input to integer
	int option = stoi(input);
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
		// If the input is not in range [1-10]
	default:
		cout << "Please enter an option in range [1-10]!" << endl;
		break;
	}
	// Display menu again
	displayMenu();
}

int main(int argc, char* argv[])
{
	// Please uncomment this on submission

	// if (argc != 3){
	//   cout << "Please run the program by the following format: " << endl;
	//   cout << "assignment2_group26.exe [customers_file_name] [items_file_name]";
	//   return -1;
	// }
	// char* customers_file_name = argv[1];
	// char* items_file_name = argv[2];

	// Default file names
	string customers_file_name = "customers.txt";
	string items_file_name = "items.txt";

	ifstream customers_infile(customers_file_name);
	if (!customers_infile)
	{
		cout << "Cannot open file " << customers_file_name << endl;
		return -1;
	}
	ifstream items_infile(items_file_name);
	if (!customers_infile)
	{
		cout << "Cannot open file " << items_file_name << endl;
		return -1;
	}


	cout << "Customers file: " << customers_file_name << endl;
	cout << "Items file: " << items_file_name << endl;
	displayMenu();
}