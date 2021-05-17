#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "StoreManager.h"
#include "validator.h"
#include "input.h"
#include "sort.h"

// Constructor with 2 database file names as parameters
StoreManager::StoreManager(const string& customers_file_name, const string& items_file_name) {
	this->customers_file_name = customers_file_name;
	this->items_file_name = items_file_name;
	// Initialize customer and item lists
	customers = new List<Customer*>();
	items = new List<Item*>();
}

// Destructor: clean up all heap memory of customers and items
StoreManager::~StoreManager() {
	// Loop over all customers and deallocate their memory
	auto current_customer_node = customers->getHead();
	while (current_customer_node) {
		delete current_customer_node->data;
		current_customer_node = current_customer_node->next;
	}
	// Loop over all items and deallocate their memory
	auto current_item_node = items->getHead();
	while (current_item_node) {
		delete current_item_node->data;
		current_item_node = current_item_node->next;
	}
	// Delete the lists, which will invoke the deletion of all nodes inside the list
	delete customers;
	delete items;
}

void StoreManager::displayMenu()
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
	cout << "Type Exit to quit the program." << endl;
	// Ask for user input
	int option = getOptionInput(1, 10, "Exit");
	if (option == 1) displayItemMenu();
	else if (option == 2) displayCustomerMenu();
	else if (option == 3) promoteCustomer();
	else if (option == 4) rentItem();
	else if (option == 5) returnItem();
	else if (option == 6) displayAllItems();
	else if (option == 7) displayOutOfStockItems();
	else if (option == 8) displayAllCustomers();
	else if (option == 9) displayGroupOfCustomers();
	else if (option == 10) displaySearchMenu();
	else if (option == 11) {
		exit();
		// After exit, return from the function so the menu is not called again
		return;
	}
	// Call the menu again
	displayMenu();
}

// Check if the data can be loaded from both files
bool StoreManager::loadDataFromFiles() {
	// False if the items data cannot be loaded
	if (!loadItemsFromFile()) return false;
	// False if the customer data cannot be loaded
	if (!loadCustomersFromFile()) return false;
	// True if both can be loaded
	return true;
}

// Read the customers from file and their items and store them inside the list.
// All fields will be trimmed
bool StoreManager::loadCustomersFromFile() {
	// Initialize input file from customer file name
	ifstream infile(customers_file_name);
	// Return false if customer file cannot be opened
	if (!infile)
	{
		cout << "Cannot open file " << customers_file_name << endl;
		return false;
	}

	string line;
	int lineNum = 0;
	string delim = ",";

	// Track the last customer to insert the items
	Customer* lastCustomer = nullptr;
	// Variables to track the number of rentals in the customer row and below the customer row
	int maxNumberOfRentals = 0;
	int currentNumberOfRentals = 0;
	// Loop over all lines in file
	while (getline(infile, line)) {
		lineNum++;
		// Trim line
		line = trim(line);
		// Check if line is item ID
		if (isValidItemId(line)) {
			// Skip the item line if there is no valid customer above it
			if (lastCustomer == nullptr) continue;

			Item* item = getItemById(line);
			// Skip if the item does not exist
			if (item == nullptr) {
				stringstream ss;
				ss << lastCustomer->getId() << ": Item " << line << " does not exist!";
				displayCustomerError(lineNum, ss.str());
				continue;
			};

			// Skip if the item is already rented by the customer
			bool hasDuplicateItem = false;
			auto current = lastCustomer->getRentalIds()->getHead();
			while (current) {
				if (item->getId() == current->data) {
					stringstream ss;
					ss << "Item " << item->getId() << " is already rented by " << lastCustomer->getId();
					displayCustomerError(lineNum, ss.str());
					hasDuplicateItem = true;
					break;
				}
				current = current->next;
			}
			if (hasDuplicateItem) continue;

			currentNumberOfRentals++;
			// Skip the item row if the number of rentals below the customer row exceeds the number of rentals in the customer row
			if (currentNumberOfRentals > maxNumberOfRentals) {
				continue;
			}
			// Skip if the item is out of stock
			if (!item->isAvailable()) {
				stringstream ss;
				ss << "Item out-of-stock: " << item->getId() << " is not avaialble to be rented by " << lastCustomer->getId();
				displayCustomerError(lineNum, ss.str());
				continue;
			}
			lastCustomer->rentItem(line);
			item->rented();
			continue;
		}

		// Log the error message if the number of rentals below the customer row does not match the number of rentals in the customer row 
		if (currentNumberOfRentals != maxNumberOfRentals && lastCustomer != nullptr) {
			int finalNumberOfRentals = currentNumberOfRentals < maxNumberOfRentals ? currentNumberOfRentals : maxNumberOfRentals;
			cout << lastCustomer->getId() << ": Number of rentals mismatch. Expected: " << maxNumberOfRentals << ". Actual: " << currentNumberOfRentals << endl;
			cout << "Program will modify the number of rentals to " << finalNumberOfRentals << endl;
		}
		// Set the last customer to null at first if it is not an item line
		lastCustomer = nullptr;
		// Skip if there is a invalid number of commas for customer row
		int commaCount = 0;
		for (int i = 0; i < line.length(); i++) {
			if (line[i] == ',') commaCount++;
		}
		if (commaCount != 5) continue;

		// Variables to track field start and end position when parsing the csv row
		size_t start = 0;
		size_t end = line.find(delim);
		int i = 0;
		string fields[6];

		// Loop through all fields
		while (end != string::npos) {
			string field = line.substr(start, end - start);
			start = end + delim.length();
			end = line.find(delim, start);
			fields[i] = field;
			i++;
		}

		// Get last field
		fields[i] = line.substr(start, end - start);
		string id = trim(fields[0]);
		if (!isValidCustomerId(id)) {
			stringstream ss;
			ss << "Invalid customer ID: " << id;
			displayCustomerError(lineNum, ss.str());
			continue;
		}

		// Skip if id already exists
		if (customerExists(id)) {
			stringstream ss;
			ss << "Customer " << id << " already exists!";
			displayCustomerError(lineNum, ss.str());
			continue;
		};
		// Validate the number of rentals
		string maxNumberOfRentalsString = trim(fields[4]);
		if (!isNonNegativeInteger(maxNumberOfRentalsString)) {
			stringstream ss;
			ss << "Invalid number of rentals: " << maxNumberOfRentalsString;
			displayCustomerError(lineNum, ss.str());
			continue;
		}
		// Reset the current number of rentals
		currentNumberOfRentals = 0;
		// Assign the new max number of rentals
		maxNumberOfRentals = stoi(maxNumberOfRentalsString);
		// Get the fields from string array, trim and validate them
		string name = trim(fields[1]);
		if (!isValidName(name)) {
			stringstream ss;
			ss << "Invalid name: " << name;
			displayCustomerError(lineNum, ss.str());
			continue;
		}
		string address = trim(fields[2]);
		if (isEmpty(address)) {
			stringstream ss;
			ss << "Invalid address: " << address;
			displayCustomerError(lineNum, ss.str());
			continue;
		}
		string phoneNumber = trim(fields[3]);
		if (!isValidPhoneNumber(phoneNumber)) {
			stringstream ss;
			ss << "Invalid phone number: " << phoneNumber;
			displayCustomerError(lineNum, ss.str());
			continue;
		}
		string type = trim(fields[5]);
		if (!isValidCustomerType(type)) {
			stringstream ss;
			ss << "Invalid customer type: " << type;
			displayCustomerError(lineNum, ss.str());
			continue;
		}
		Customer* customer = createCustomer(id, name, address, phoneNumber, type);
		// Skip if the customer cannot be created
		if (customer == nullptr) {
			continue;
		}
		// Store the customer inside the list
		customers->add(customer);
		// Assign the last customer to this customer
		lastCustomer = customer;
	}

	// Check last customer number of rentals mismatch
	// Log the error message if the number of rentals below the customer row does not match the number of rentals in the customer row 
	if (currentNumberOfRentals != maxNumberOfRentals && lastCustomer != nullptr) {
		int finalNumberOfRentals = currentNumberOfRentals < maxNumberOfRentals ? currentNumberOfRentals : maxNumberOfRentals;
		cout << lastCustomer->getId() << ": Number of rentals mismatch. Expected: " << maxNumberOfRentals << ". Actual: " << currentNumberOfRentals << endl;
		cout << "Program will modify the number of rentals to " << finalNumberOfRentals << endl;
	}
	// Close the input file
	infile.close();
	// Return true to notify that the file is successfully loaded
	return true;
}

// Read the items from file and store them inside the list
bool StoreManager::loadItemsFromFile() {
	// Create the input file from the items file name
	ifstream infile(items_file_name);
	// Return false if the items file cannot be opened
	if (!infile)
	{
		cout << "Cannot open file " << items_file_name << endl;
		return false;
	}

	int lineNum = 0;
	string line;
	string delim = ",";
	// Loop through all fields
	while (getline(infile, line)) {
		lineNum++;
		// Skip if there are invalid number of commas
		int commaCount = 0;
		for (int i = 0; i < line.length(); i++) {
			if (line[i] == ',') commaCount++;
		}
		// 5: without genre, 6: with genre
		if (!(commaCount == 5 || commaCount == 6)) continue;

		// Variables to track start and end position of fields in csv row
		size_t start = 0;
		size_t end = line.find(delim);
		int i = 0;
		string fields[7];

		// Loop through all fields
		while (end != string::npos) {
			string field = line.substr(start, end - start);
			start = end + delim.length();
			end = line.find(delim, start);
			fields[i] = field;
			i++;
		}
		// Get last field
		fields[i] = line.substr(start, end - start);
		i++;
		string id = trim(fields[0]);
		if (!isValidItemId(id)) {
			stringstream ss;
			ss << "Invalid item ID: " << id;
			displayItemError(lineNum, ss.str());
			continue;
		}
		//Skip if the item id alreasy exists
		if (itemExists(id)) {
			stringstream ss;
			ss << "Item " << id << " already exists!";
			displayItemError(lineNum, ss.str());
			continue;
		}
		// Get the fields from string array, trim and validate them
		string title = trim(fields[1]);
		if (isEmpty(title)) {
			stringstream ss;
			ss << "Invalid title: " << title;
			displayItemError(lineNum, ss.str());
			continue;
		}
		string rentalType = trim(fields[2]);
		if (!isValidRentalType(rentalType)) {
			stringstream ss;
			ss << "Invalid rental type: " << rentalType;
			displayItemError(lineNum, ss.str());
			continue;
		}
		string loanType = trim(fields[3]);
		if (!isValidLoanType(loanType)) {
			stringstream ss;
			ss << "Invalid loan type: " << loanType;
			displayItemError(lineNum, ss.str());
			continue;
		}
		string numberOfCopiesString = trim(fields[4]);
		if (!isNonNegativeInteger(numberOfCopiesString)) {
			stringstream ss;
			ss << "Invalid number of copies: " << numberOfCopiesString;
			displayItemError(lineNum, ss.str());
			continue;
		}
		string rentalFeeString = trim(fields[5]);
		if (!isNonNegativeNumber(rentalFeeString)) {
			stringstream ss;
			ss << "Invalid rental fee: " << rentalFeeString;
			displayItemError(lineNum, ss.str());
			continue;
		}
		string genre = trim(fields[6]);
		if (!isValidGenre(genre) && rentalType != "Game") {
			stringstream ss;
			ss << "Invalid genre for non-game items: " << genre;
			displayItemError(lineNum, ss.str());
			continue;
		}
		if (!isEmpty(genre) && rentalType == "Game") {
			stringstream ss;
			ss << "Game items cannot have genre. Found: " << genre;
			displayItemError(lineNum, ss.str());
			continue;
		}

		// Convert from string to correct data types
		int numberOfCopies = stoi(fields[4]);
		double rentalFee = stod(fields[5]);

		// Create the item from those fields
		Item* item = createItem(id, title, rentalType, loanType, numberOfCopies, rentalFee, genre);
		// Skip if the item cannot be created
		if (item == nullptr) {
			continue;
		}
		// Add the item to list
		items->add(item);
	}
	// Close the input file
	infile.close();
	// Return true to notify that the file is successfully loaded
	return true;
}

void StoreManager::saveData() {
	saveItems();
	saveCustomers();
}
void StoreManager::saveCustomers() {
	ofstream outfile(customers_file_name);
	if (!outfile)
	{
		cout << "Cannot save file " << customers_file_name << endl;
		return;
	}
	auto current = customers->getHead();
	while (current) {
		outfile << current->data->toFileContent();
		current = current->next;
	}

	outfile.close();
}
void StoreManager::saveItems() {
	ofstream outfile(items_file_name);
	if (!outfile)
	{
		cout << "Cannot save file " << items_file_name << endl;
		return;
	}
	auto current = items->getHead();
	while (current) {
		outfile << current->data->toFileContent();
		current = current->next;
	}

	outfile.close();
}
void StoreManager::displayCustomerError(int lineNum, string content)
{
	cout << customers_file_name << " - line " << lineNum << ": " << content << endl;
}
void StoreManager::displayItemError(int lineNum, string content)
{
	cout << items_file_name << " - line " << lineNum << ": " << content << endl;
}
// Return the item pointer from parameters, return nullptr if the rental type is not Record, DVD or Game
Item* StoreManager::createItem(const string& id, const string& title, const string& rentalType, const string& loanType, int numberOfCopies, double rentalFee, const string& genre) {
	if (rentalType == "Record") return new Record(id, title, loanType, numberOfCopies, rentalFee, genre);
	if (rentalType == "DVD") return new DVD(id, title, loanType, numberOfCopies, rentalFee, genre);
	if (rentalType == "Game") return new Game(id, title, loanType, numberOfCopies, rentalFee);
	return nullptr;
}

// Return the customer pointer from parameters, return nullptr if the account type is not Guest, Regular or VIP
Customer* StoreManager::createCustomer(const string& id, const string& name, const string& address, const string& phoneNumber, const string& type) {
	if (type == "Guest") return new GuestCustomer(id, name, address, phoneNumber);
	if (type == "Regular") return new RegularCustomer(id, name, address, phoneNumber);
	if (type == "VIP") return new VIPCustomer(id, name, address, phoneNumber);
	return nullptr;
}

// Return the item from id, return nullptr if the id does not exist
Item* StoreManager::getItemById(const string& id) {
	// Get the head node of the item list
	auto current = items->getHead();
	// Loop through all nodes
	while (current) {
		// If the id is found, return the item
		if (current->data->getId() == id) return current->data;
		// Set the current node to next node
		current = current->next;
	}
	// If not found, return nullptr
	return nullptr;
}

// Return the non-exist item id from input
string StoreManager::getNonExistItemIdInput()
{
	string input;
	while (true)
	{
		// Get the item id input
		input = getItemIdInput();
		// If the item does not exists with that id, return the id
		if (!itemExists(input)) return input;
		// Display the error message and require the user to type again
		cout << "Item ID " << input << " already exists!" << endl;
	}
	return input;
}

// Return the item from id input
Item* StoreManager::getExistItemInput()
{
	string input;
	Item* item = nullptr;
	while (true) {
		// Ask for the item id
		input = getItemIdInput();
		item = getItemById(input);
		// If the item is not null, return it
		if (item != nullptr) return item;
		// Display the error message and require the user to type again
		cout << "Item ID " << input << " does not exist!" << endl;
	}
	return item;
}

// Return the customer from id, return nullptr if the id does not exist
Customer* StoreManager::getCustomerById(const string& id) {
	// Get the head node of customer list
	auto current = customers->getHead();
	// Loop through all nodes
	while (current) {
		// If the id is found, return the customer
		if (current->data->getId() == id) return current->data;
		current = current->next;
	}
	// If the customer is not found, return null
	return nullptr;
}

// Return the non-exist customer id from input
string StoreManager::getNonExistCustomerIdInput() {
	string input;
	while (true) {
		// Ask for customer id
		input = getCustomerIdInput();
		// If the id does not exist, return the id
		if (!customerExists(input)) return input;
		// Require the user to type again
		cout << "Customer ID " << input << " already exists!" << endl;
	}
	return input;
}

// Return the customer from id input
Customer* StoreManager::getExistCustomerInput() {
	string input;
	Customer* customer;
	while (true) {
		// Ask for customer id
		input = getCustomerIdInput();
		customer = getCustomerById(input);
		// If the customer is not null, return it
		if (customer != nullptr) return getCustomerById(input);
		// Display the error message and require the user to type again
		cout << "Customer ID " << input << " does not exist!" << endl;
	}
	return customer;
}

// Find the customer index inside the list from id
int StoreManager::getCustomerIndex(const std::string& id) {
	int i = 0;
	auto current = customers->getHead();
	while (current) {
		// Return the index if the id is found
		if (current->data->getId() == id) return i;
		current = current->next;
		i++;
	}
	// Return -1 if the customer is not found
	return -1;
}

// Find the item index inside the list from id
int StoreManager::getItemIndex(const std::string& id) {
	int i = 0;
	auto current = items->getHead();
	while (current) {
		// Return the index if the id is found
		if (current->data->getId() == id) return i;
		current = current->next;
		i++;
	}
	// Return -1 if the item is not found
	return -1;
}
// 1 
void StoreManager::displayItemMenu()
{
	cout << "-------------------------------" << endl;
	cout << "Manage items" << endl;
	cout << "1. Add new item" << endl;
	cout << "2. Add item copies to stock" << endl;
	cout << "3. Update item" << endl;
	cout << "4. Delete item" << endl;
	cout << "5. Back" << endl;

	int option = getOptionInput(1, 5);
	if (option == 1) addItem();
	else if (option == 2) addItemCopiesToStock();
	else if (option == 3) updateItem();
	else if (option == 4) deleteItem();
	else if (option == 5) return; // Exit from item menu
	waitForEnter();
	// Display item menu again
	displayItemMenu();
}
// 1.1
void StoreManager::addItem()
{
	// Ask for item type
	cout << "-------------------------------" << endl;
	cout << "Select item type" << endl;
	cout << "1. Record" << endl;
	cout << "2. DVD" << endl;
	cout << "3. Game" << endl;
	int option = getOptionInput(1, 3);
	string rentalType = "";
	// Set the item type based on option
	if (option == 1) rentalType = "Record";
	else if (option == 2) rentalType = "DVD";
	else if (option == 3) rentalType = "Game";
	// Get other fields
	string id = getNonExistItemIdInput();
	string title = getItemTitleInput();
	string loanType = getItemLoanTypeInput();
	int numberOfCopies = getItemNumberOfCopiesInput();
	double rentalFee = getItemRentalFeeInput();
	string genre;
	// Skip asking genre if the item type is Game
	if (rentalType != "Game") {
		genre = getItemGenreInput();
	}
	// Create the item from user input
	Item* item = createItem(id, title, rentalType, loanType, numberOfCopies, rentalFee, genre);
	// Add the item to list
	items->add(item);
	// Display success message
	cout << "Add item " << id << " succesfully!" << endl;;
}

// 1.2
void StoreManager::addItemCopiesToStock()
{
	// Get the exist item
	Item* item = getExistItemInput();
	// Display item info
	cout << item->toConsoleString();
	// Get the number of copies to add to stock
	int numberOfCopies = getItemNumberOfCopiesInput();
	item->addToStock(numberOfCopies);
	// Display success message
	cout << "Add " << numberOfCopies << " copies of item " << item->getId() << " to stock succesfully!" << endl;
}

// 1.3
void StoreManager::updateItem()
{
	// Get the exist item
	Item* item = getExistItemInput();
	// Display item info
	cout << item->toConsoleString();
	// Display update options
	cout << "------------------------" << endl;
	cout << "1. Update title" << endl;
	cout << "2. Update loan type" << endl;
	cout << "3. Update rental fee" << endl;
	cout << "4. Update genre" << endl;
	cout << "5. Back " << endl;
	int option = getOptionInput(1, 5);
	if (option == 1)
	{
		string title = getItemTitleInput();
		item->setTitle(title);
		cout << "Update title sucessfully!" << endl;
	}
	else if (option == 2)
	{
		string loanType = getItemLoanTypeInput();
		item->setLoanType(loanType);
		cout << "Update loan type sucessfully!" << endl;
	}
	else if (option == 3)
	{
		double rentalFee = getItemRentalFeeInput();
		item->setRentalFee(rentalFee);
		cout << "Update rental fee sucessfully!" << endl;
	}
	else if (option == 4) {
		// Display error message if the user tries to update genre of a Game item
		if (item->getRentalType() == "Game") {
			cout << "Game items do not have genre!" << endl;
		}
		else {
			string genre = getItemGenreInput();
			item->setGenre(genre);
			cout << "Update genre sucessfully!" << endl;
		}
	}
	else if (option == 5) return;
}

// 1.4 
void StoreManager::deleteItem()
{
	// Get the exist item
	Item* item = getExistItemInput();

	// Check if item is rented by any customer

	stringstream ss;
	int count = 0;
	auto currentCustomerNode = customers->getHead();
	// Loop through all customers
	while (currentCustomerNode) {
		auto currentItemIdNode = currentCustomerNode->data->getRentalIds()->getHead();
		// Loop over all item ids of the current customer
		while (currentItemIdNode) {
			if (currentItemIdNode->data == item->getId()) {
				ss << currentCustomerNode->data->getId() << endl;
				count++;
			}
			currentItemIdNode = currentItemIdNode->next;
		}
		currentCustomerNode = currentCustomerNode->next;
	}
	// If the item is rented by any customer, display all customers that are renting the item and exit the option
	if (count > 0) {
		cout << "Can't delete this item as it is being rented by these customers: " << endl;
		cout << ss.str();
		return;
	}
	// Otherwise, delete the item and display success message
	int i = getItemIndex(item->getId());
	items->deleteNode(i);
	cout << "Delete item " << item->getId() << " successfully!" << endl;
}


// 2
void StoreManager::displayCustomerMenu()
{
	cout << "-------------------------------" << endl;
	cout << "Manage customers" << endl;
	cout << "1. Add customer" << endl;
	cout << "2. Update customer" << endl;
	cout << "3. Back" << endl;
	int option = getOptionInput(1, 3);
	if (option == 1) addCustomer();
	else if (option == 2) updateCustomer();
	else if (option == 3) return; // Exit from customer menu
	waitForEnter();
	// Display customer menu again
	displayCustomerMenu();
}

// 2.1
void StoreManager::addCustomer()
{
	// Get all fields from input
	string id = getNonExistCustomerIdInput();
	string name = getCustomerNameInput();
	string address = getCustomerAddressInput();
	string phoneNumber = getCustomerPhoneNumberInput();
	// Default type when creating a customer will be Guest
	string type = "Guest";
	// Create customer from inputs
	Customer* customer = createCustomer(id, name, address, phoneNumber, type);
	// Exit if the customer cannot be created
	if (customer == nullptr) {
		cout << "Cannot add customer " << id << endl;
		return;
	}
	// Add customer to list
	customers->add(customer);
	// Display success message
	cout << "Add customer " << id << " successfully!" << endl;
}

// 2.2
void StoreManager::updateCustomer()
{
	// Get exist customer
	Customer* customer = getExistCustomerInput();
	// Display customer info
	cout << customer->toConsoleString();
	// Select field to update
	cout << "-----------------------" << endl;
	cout << "1. Update name" << endl;
	cout << "2. Update phone number " << endl;
	cout << "3. Update address" << endl;
	cout << "4. Back" << endl;
	int option = getOptionInput(1, 4);
	if (option == 1)
	{
		string name = getCustomerNameInput();
		customer->setName(name);
		cout << "Update customer name successfully!!" << endl;
	}
	else if (option == 2)
	{
		string phoneNumber = getCustomerPhoneNumberInput();
		customer->setPhoneNumber(phoneNumber);
		cout << "Update phone number successfully!! " << endl;
	}
	else if (option == 3)
	{
		string address = getCustomerAddressInput();
		customer->setAddress(address);
		cout << "Update address successfully!! " << endl;
	}
	else if (option == 4) return;
}

// 3
void StoreManager::promoteCustomer()
{
	// Get exist customer
	Customer* customer = getExistCustomerInput();
	// Exit if the customer type is VIP
	if (customer->getType() == "VIP") {
		cout << "VIP customer cannot be promoted!" << endl;
		waitForEnter();
		return;
	}
	// Exit if the customer has not returned 3 items
	if (!customer->isPromotable()) {
		cout << "Guest and Regular customer must return 3 items to be promoted!" << endl;
		cout << "Current number of returns: " << customer->getNumberOfReturns() << endl;
		waitForEnter();
		return;
	}

	// Delete the current customer from list
	int i = getCustomerIndex(customer->getId());
	customers->deleteNode(i);

	// Create the new customer with the same attributes except for the type
	string type = customer->getType();
	string newType;
	if (type == "Regular")newType = "VIP";
	else if (type == "Guest") newType = "Regular";

	// Add the promoted customer to list
	customers->add(createCustomer(customer->getId(), customer->getName(), customer->getAddress(), customer->getPhoneNumber(), newType));
	// Display success message
	cout << "Promote customer " << customer->getId() << " successfully from " << type << " to " << newType << endl;
	waitForEnter();
}

// 4
void StoreManager::rentItem()
{
	// Get exist customer
	Customer* customer = getExistCustomerInput();
	// Display customer info
	cout << customer->toConsoleString();
	// Exit if the guest customer is renting 2 items
	if (customer->getType() == "Guest" && customer->getNumberOfRentals() >= 2) {
		cout << "Guest customer cannot rent more than 2 items at a time!" << endl;
		waitForEnter();
		return;
	}
	// Get exist item
	Item* item = getExistItemInput();
	// Exit if the item is not available
	if (!item->isAvailable()) {
		cout << "Item " << item->getId() << " is out of stock!" << endl;
		waitForEnter();
		return;
	}
	// Exit if a guest customer is trying to rent a 2-day item
	if (customer->getType() == "Guest" && item->getLoanType() == "2-day") {
		cout << "Guest customer cannot rent a 2-day item!" << endl;
		waitForEnter();
		return;
	}

	// Exit if the item is already rented by the customer
	auto current = customer->getRentalIds()->getHead();
	while (current) {
		if (item->getId() == current->data) {
			cout << "Item " << item->getId() << " is already rented by " << customer->getId() << endl;
			return;
		}
		current = current->next;
	}

	string costString = item->getRentalFeeString();
	// Check if the customer can rent for free (should be VIP customer)
	if (customer->canRentForFree()) {
		// Ask to rent for free by 100 VIP points
		cout << "Do you want to use 100 VIP points to rent this item for free? Current VIP points: " << customer->getPoints() << endl;
		// Select yes or no
		bool rentForFree = getYesNoInput();
		// If yes
		if (rentForFree) {
			// Set the display cost to 0
			costString = "0.00";
			// Consume 100 points from customer account
			customer->consumePoints(100);
			cout << "Consumed 100 VIP points! Current VIP points: " << customer->getPoints() << endl;
		}
	}
	// Display success message and the cost
	cout << "Rent item successfully! Cost($): " << costString << endl;

	// Add the item id to customer rental list
	customer->rentItem(item->getId());
	item->rented();

	waitForEnter();
}

// 5
void StoreManager::returnItem()
{
	// Get exist customer
	Customer* customer = getExistCustomerInput();
	// Display customer info
	cout << customer->toConsoleString();
	// Get exist item
	Item* item = getExistItemInput();
	// If the item is in the customer rental list
	if (customer->returnItem(item->getId())) {
		cout << "Return item sucessfully!" << endl;
		item->returned();
		// Display VIP points if the customer is VIP
		if (customer->getType() == "VIP") cout << "Receive 10 VIP points! Current VIP points: " << customer->getPoints() << endl;
		waitForEnter();
		return;
	}
	// If the item is not rented by the customer
	cout << "Item " << item->getId() << " is not rented by " << customer->getId() << endl;
	waitForEnter();
}
// 6
void StoreManager::displayAllItems()
{
	// Sort based on option
	cout << "-------------------------" << endl;
	cout << "1. Display all items sorted by ID." << endl;
	cout << "2. Display all items sorted by title." << endl;
	cout << "3. Back" << endl;
	int option = getOptionInput(1, 3);
	if (option == 1)
	{
		sortItemsById(items->getHeadRef());
	}
	else if (option == 2) {
		sortItemsByTitle(items->getHeadRef());
	}
	else if (option == 3)return;
	// Display number of items
	cout << items->getSize() << " item(s)" << endl;
	// Display all items to the console
	auto current = items->getHead();
	while (current) {
		cout << current->data->toConsoleString();
		current = current->next;
	}
	waitForEnter();
}

// 7
void StoreManager::displayOutOfStockItems()
{
	// Create a string stream to store all item strings
	stringstream ss;
	int count = 0;
	auto current = items->getHead();
	while (current) {
		// Add the item string to string stream if it is not avaiable
		if (!current->data->isAvailable()) {
			ss << current->data->toConsoleString();
			count++;
		}
		current = current->next;
	}
	// Display number of out of stock items
	cout << count << " out-of-stock item(s)" << endl;
	// Display all out-of-stock items
	cout << ss.str();
	waitForEnter();
}

//8
void StoreManager::displayAllCustomers()
{
	// Sort based on option
	cout << "-------------------------" << endl;
	cout << "1. Display all customers sorted by ID." << endl;
	cout << "2. Display all customers sorted by name." << endl;
	cout << "3. Back" << endl;
	int option = getOptionInput(1, 3);
	if (option == 1)
	{
		sortCustomersById(customers->getHeadRef());
	}
	else if (option == 2) {
		sortCustomersByName(customers->getHeadRef());
	}
	else if (option == 3)return;

	// Display number of customers
	cout << customers->getSize() << " customer(s)" << endl;
	// Display all customers
	auto current = customers->getHead();
	while (current) {
		cout << "---------------" << endl;
		cout << current->data->toConsoleString();
		current = current->next;
	}
	waitForEnter();
}

// 9
void StoreManager::displayGroupOfCustomers()
{
	cout << "-------------------------------" << endl;
	cout << "Select customer group" << endl;
	cout << "1. Guest" << endl;
	cout << "2. Regular" << endl;
	cout << "3. VIP" << endl;
	cout << "or" << endl;
	cout << "4. Back" << endl;

	int option = getOptionInput(1, 4);
	string type;
	if (option == 1) type = "Guest";
	else if (option == 2) type = "Regular";
	else if (option == 3) type = "VIP";
	else if (option == 4) return;

	// Create a string stream that store customer strings
	stringstream ss;
	auto current = customers->getHead();
	int count = 0;
	while (current) {
		// If the customer type is the selected type, add the customer string to the string stream
		if (current->data->getType() == type) {
			ss << "---------------" << endl;
			ss << current->data->toConsoleString();
			count++;
		}
		current = current->next;
	}
	// Display number of customers of the selected type
	cout << count << " " << type << " customer(s)" << endl;
	// Display all customers
	cout << ss.str();
	waitForEnter();
	// Display the menu again
	displayGroupOfCustomers();
}

// 10
void StoreManager::displaySearchMenu()
{
	cout << "-------------------------------" << endl;
	cout << "1. Search item by id" << endl;
	cout << "2. Search item by title" << endl;
	cout << "3. Search customer by id" << endl;
	cout << "4. Search customer by name" << endl;
	cout << "5. Back" << endl;
	int option = getOptionInput(1, 5);
	if (option == 1) searchItemById();
	else if (option == 2) searchItemByTitle();
	else if (option == 3) searchCustomerById();
	else if (option == 4) searchCustomerByName();
	else if (option == 5) return;
	waitForEnter();
	displaySearchMenu();
}
// 10.1
void StoreManager::searchItemById() {
	// Get the valid item id
	string id = getItemIdInput();
	// Find the item
	Item* item = getItemById(id);
	// Display a message if the item does not exist
	if (item == nullptr) {
		cout << "Item with ID " << id << " does not exist!" << endl;
		return;
	}
	// Display the item
	cout << item->toConsoleString();

}
//10.2
void StoreManager::searchItemByTitle() {
	// Get the title
	string title = getItemTitleInput();
	int count = 0;
	// Create the string stream to store item strings
	stringstream ss;
	auto current = items->getHead();
	while (current) {
		// Store the item strings of that title
		if (current->data->getTitle() == title) {
			ss << current->data->toConsoleString();
			count++;
		}
		current = current->next;
	}
	// Display all items with that title if there is any
	if (count > 0) {
		cout << count << " item(s) with title " << title << endl;
		cout << ss.str();
		return;
	}
	// Display a message if there is no item with that title
	cout << "Item with title " << title << " does not exist!" << endl;
}

//10.3
void StoreManager::searchCustomerById() {
	// Get the valid customer id
	string id = getCustomerIdInput();
	// Find the customer
	Customer* customer = getCustomerById(id);
	// Display a message if the id does not exist
	if (customer == nullptr) {
		cout << "Customer with ID " << id << " does not exist!" << endl;
		return;
	}
	// Display the customer
	cout << customer->toConsoleString();
}

//10.4
void StoreManager::searchCustomerByName() {
	// Get the customer name
	string name = getCustomerNameInput();
	int count = 0;
	// Create a string stream to store customer strings
	stringstream ss;
	auto current = customers->getHead();
	while (current) {
		// Add the customer strings of that name
		if (current->data->getName() == name) {
			ss << "---------------" << endl;
			ss << current->data->toConsoleString();
			count++;
		}
		current = current->next;
	}
	// Display all customers with that name if there is any
	if (count > 0) {
		cout << count << " customer(s) with name " << name << endl;
		cout << ss.str();
		return;
	}
	// Display a message if the customer does not exist
	cout << "Customer with name " << name << " does not exist!" << endl;
}

// 11
void StoreManager::exit()
{
	cout << "ASSIGNMENT 2 GROUP 26" << endl;
	cout << "s3818074, s3818074@rmit.edu.vn, Khai, Truong" << endl;
	cout << "s3878636, s3878636@rmit.edu.vn, Quy, Nguyen" << endl;
	cout << "s3877746, s3877746@rmit.edu.vn, Thuy, Mai" << endl;
	cout << "s3905273, s3905273@rmit.edu.vn, Nam, Pham" << endl;
	sortItemsById(items->getHeadRef());
	sortCustomersById(customers->getHeadRef());
	saveData();
}