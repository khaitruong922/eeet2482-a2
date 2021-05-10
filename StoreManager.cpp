#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "StoreManager.h"
#include "validator.h"
#include "input.h"
#include "sort.h"
StoreManager::StoreManager(const string& customers_file_name, const string& items_file_name) {
	this->customers_file_name = customers_file_name;
	this->items_file_name = items_file_name;
	customers = new List<Customer*>();
	items = new List<Item*>();
}

StoreManager::~StoreManager() {
	auto current_customer_node = customers->getHead();
	while (current_customer_node) {
		delete current_customer_node->data;
		current_customer_node = current_customer_node->next;
	}
	auto current_item_node = items->getHead();
	while (current_item_node) {
		delete current_item_node->data;
		current_item_node = current_item_node->next;
	}
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
		return;
	}
	displayMenu();
}

bool StoreManager::loadDataFromFiles() {
	if (!loadItemsFromFile()) return false;
	if (!loadCustomersFromFile()) return false;
	return true;
}

bool StoreManager::loadCustomersFromFile() {
	ifstream infile(customers_file_name);
	if (!infile)
	{
		cout << "Cannot open file " << customers_file_name << endl;
		return false;
	}

	string line;
	string delim = ",";

	Customer* lastCustomer = nullptr;
	int maxNumberOfRentals = 0;
	int currentNumberOfRentals = 0;
	while (getline(infile, line)) {
		// Trim line
		line = trim(line);
		// Check if line is item ID
		if (isValidItemId(line)) {
			// Skip the item line if there is no valid customer above it
			if (lastCustomer == nullptr) continue;

			Item* item = getItemById(line);
			// Skip if the item does not exist
			if (item == nullptr) {
				cout << lastCustomer->getId() << ": Item " << line << " does not exist!" << endl;
				continue;
			};

			// Skip if the item is already rented by the customer
			bool hasDuplicateItem = false;
			auto current = lastCustomer->getRentalIds()->getHead();
			while (current) {
				if (item->getId() == current->data) {
					cout << "Item " << item->getId() << " is already rented by " << lastCustomer->getId() << endl;
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
				cout << "Item out-of-stock: " << item->getId() << " is not avaialble to be rented by " << lastCustomer->getId() << endl;
				continue;
			}
			lastCustomer->rentItem(line);
			item->rented();
			continue;
		}

		// If not, line is customer info
		int commaCount = 0;
		for (int i = 0;i < line.length();i++) {
			if (line[i] == ',') commaCount++;
		}

		// Skip if there are invalid number of commas
		if (commaCount != 5) continue;

		int start = 0;
		int end = line.find(delim);
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


		// Skip if id already exists
		if (customerExists(id)) {
			cout << "Customer " << id << " already exists!" << endl;
			lastCustomer = nullptr;
			continue;
		};

		// Now the customer is valid
		// Log the error message if the number of rentals below the customer row does not match the number of rentals in the customer row 
		if (currentNumberOfRentals != maxNumberOfRentals && lastCustomer != nullptr) {
			cout << lastCustomer->getId() << ": Number of rentals mismatch. Expected: " << maxNumberOfRentals << ". Actual: " << currentNumberOfRentals << endl;
		}

		// Validate the number of rentals
		string maxNumberOfRentalsString = trim(fields[4]);
		if (!isNonNegativeInteger(maxNumberOfRentalsString)) continue;
		currentNumberOfRentals = 0;
		maxNumberOfRentals = stoi(maxNumberOfRentalsString);
		string name = trim(fields[1]);
		string address = trim(fields[2]);
		string phoneNumber = trim(fields[3]);
		string type = trim(fields[5]);
		Customer* customer = createCustomer(id, name, address, phoneNumber, type);
		// Skip if the customer cannot be created
		if (customer == nullptr) {
			lastCustomer = nullptr;
			delete customer;
			continue;
		}
		customers->add(customer);
		lastCustomer = customer;
	}

	// Check last customer number of rentals mismatch
	// Log the error message if the number of rentals below the customer row does not match the number of rentals in the customer row 
	if (currentNumberOfRentals != maxNumberOfRentals && lastCustomer != nullptr) {
		cout << lastCustomer->getId() << ": Number of rentals mismatch. Expected: " << maxNumberOfRentals << ". Actual: " << currentNumberOfRentals << endl;
	}
	infile.close();
	return true;
}

bool StoreManager::loadItemsFromFile() {
	ifstream infile(items_file_name);
	if (!infile)
	{
		cout << "Cannot open file " << items_file_name << endl;
		return false;
	}

	string line;
	string delim = ",";
	while (getline(infile, line)) {
		int commaCount = 0;
		for (int i = 0;i < line.length();i++) {
			if (line[i] == ',') commaCount++;
		}
		// Skip if there are invalid number of commas
		if (!(commaCount == 5 || commaCount == 6)) continue;

		int start = 0;
		int end = line.find(delim);
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

		//Skip if the item id alreasy exists
		if (itemExists(id)) {
			cout << "Item " << id << " already exists!" << endl;
			continue;
		}
		string title = trim(fields[1]);
		string rentalType = trim(fields[2]);
		string loanType = trim(fields[3]);
		string numberOfCopiesString = trim(fields[4]);
		string rentalFeeString = trim(fields[4]);
		string genre = trim(fields[6]);
		// Skip the line if the rental type is invalid
		if (!isValidRentalType(rentalType)) continue;
		// Skip the line if the loan type is invalid
		if (!isValidLoanType(loanType)) continue;
		// Skip the line if number of copies <= 0 or not integer
		if (!isNonNegativeInteger(numberOfCopiesString)) continue;
		// Skip the line if the rental fee is not a non negative number
		if (!isNonNegativeNumber(rentalFeeString)) continue;
		// Skip the line if the genre is invalid for non-game item
		if (!isValidGenre(genre) && rentalType != "Game") continue;
		// Skip the line if the game item has genre
		if (!isEmpty(genre) && rentalType == "Game") continue;

		int numberOfCopies = stoi(fields[4]);
		double rentalFee = stod(fields[5]);

		Item* item = createItem(id, title, rentalType, loanType, numberOfCopies, rentalFee, genre);
		// Skip if the item cannot be created
		if (item == nullptr) {
			continue;
		}
		items->add(item);
	}
	infile.close();
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

Item* StoreManager::getItemById(const string& id) {
	auto current = items->getHead();
	while (current) {
		if (current->data->getId() == id) return current->data;
		current = current->next;
	}
	return nullptr;
}
string StoreManager::getNonExistItemIdInput()
{
	string input;
	while (true)
	{
		input = getItemIdInput();
		if (!itemExists(input)) return input;
		cout << "Item ID " << input << " already exists!" << endl;
	}
	return input;
}
Item* StoreManager::getExistItemInput()
{
	string input;
	Item* item = nullptr;
	while (true) {
		input = getItemIdInput();
		item = getItemById(input);
		if (item != nullptr) return item;
		cout << "Item ID " << input << " does not exist!" << endl;
	}
	return item;
}

Customer* StoreManager::getCustomerById(const string& id) {
	auto current = customers->getHead();
	while (current) {
		if (current->data->getId() == id) return current->data;
		current = current->next;
	}
	return nullptr;
}

string StoreManager::getNonExistCustomerIdInput() {
	string input;
	while (true) {
		input = getCustomerIdInput();
		if (!customerExists(input)) return input;
		cout << "Customer ID " << input << " already exists!" << endl;
	}
	return input;
}

Customer* StoreManager::getExistCustomerInput() {
	string input;
	Customer* customer;
	while (true) {
		input = getCustomerIdInput();
		customer = getCustomerById(input);
		if (customer != nullptr) return getCustomerById(input);
		cout << "Customer ID " << input << " does not exist!" << endl;
	}
	return customer;
}

int StoreManager::getCustomerIndex(const std::string& id) {
	int i = 0;
	auto current = customers->getHead();
	while (current) {
		if (current->data->getId() == id) return i;
		current = current->next;
		i++;
	}
	return -1;
}

int StoreManager::getItemIndex(const std::string& id) {
	int i = 0;
	auto current = items->getHead();
	while (current) {
		if (current->data->getId() == id) return i;
		current = current->next;
		i++;
	}
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
	else if (option == 5) return;
	waitForEnter();
	displayItemMenu();
}
// 1.1 - ask about genre / loan type input ?
void StoreManager::addItem()
{
	cout << "-------------------------------" << endl;
	cout << "Select item type" << endl;
	cout << "1. Record" << endl;
	cout << "2. DVD" << endl;
	cout << "3. Game" << endl;
	// ngan gon
	int option = getOptionInput(1, 3);
	string rentalType = "";
	if (option == 1) rentalType = "Record";
	else if (option == 2) rentalType = "DVD";
	else if (option == 3) rentalType = "Game";
	string id = getNonExistItemIdInput();
	string title = getItemTitleInput();
	string loanType = getItemLoanTypeInput();
	int numberOfCopies = getItemNumberOfCopiesInput();
	double rentalFee = getItemRentalFeeInput();
	string genre;
	if (rentalType != "Game") {
		genre = getItemGenreInput();
	}
	Item* item = createItem(id, title, rentalType, loanType, numberOfCopies, rentalFee, genre);
	items->add(item);
	cout << "Add item " << id << " succesfully!" << endl;;
}

// 1.2 - done
void StoreManager::addItemCopiesToStock()
{
	Item* item = getExistItemInput();
	cout << item->toConsoleString();
	int numberOfCopies = getItemNumberOfCopiesInput();
	item->addToStock(numberOfCopies);
	cout << "Add " << numberOfCopies << " copies of item " << item->getId() << " to stock succesfully!" << endl;
}

// 1.3 - Nam 
void StoreManager::updateItem()
{
	Item* item = getExistItemInput();
	cout << item->toConsoleString();
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
	Item* item = getExistItemInput();

	// Check if item is rented by any customer
	// Loop over all customers
	stringstream ss;
	int count = 0;
	auto currentCustomerNode = customers->getHead();
	while (currentCustomerNode) {
		auto currentItemIdNode = currentCustomerNode->data->getRentalIds()->getHead();
		// Loop over all item ids
		while (currentItemIdNode) {
			if (currentItemIdNode->data == item->getId()) {
				ss << currentCustomerNode->data->getId() << endl;
				count++;
			}
			currentItemIdNode = currentItemIdNode->next;
		}
		currentCustomerNode = currentCustomerNode->next;
	}
	// If the item is rented by any customer
	if (count > 0) {
		cout << "Can't delete this item as it is being rented by these customers: " << endl;
		cout << ss.str();
		return;
	}
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
	else if (option == 3) return;
	waitForEnter();
	displayCustomerMenu();
}

// 2.1 - done
void StoreManager::addCustomer()
{
	string id = getNonExistCustomerIdInput();
	string name = getCustomerNameInput();
	string address = getCustomerAddressInput();
	string phoneNumber = getCustomerPhoneNumberInput();
	string type = "Guest";
	Customer* customer = createCustomer(id, name, address, phoneNumber, type);
	if (customer == nullptr) {
		cout << "Cannot add customer " << id << endl;
		return;
	}
	customers->add(customer);
	cout << "Add customer " << id << " successfully!" << endl;
}

// 2.2//done
void StoreManager::updateCustomer()
{
	Customer* customer = getExistCustomerInput();
	cout << customer->toConsoleString();
	cout << "-----------------------" << endl;
	cout << "1. Update name" << endl;
	cout << "2. Update phone number " << endl;
	cout << "3. Update address" << endl;
	cout << "4. Back" << endl;
	int option = getOptionInput(1, 4);
	if (option == 1)
	{
		string name = getCustomerNameInput();
		customer->setId(name);
		cout << "Update customer ID successfully!!" << endl;
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
	Customer* customer = getExistCustomerInput();
	if (customer->getType() == "VIP") {
		cout << "VIP customer cannot be promoted!" << endl;
		waitForEnter();
		return;
	}
	if (!customer->isPromotable()) {
		cout << "Guest and Regular customer must return 3 items to be promoted!" << endl;
		cout << "Current number of returns: " << customer->getNumberOfReturns() << endl;
		waitForEnter();
		return;
	}

	int i = getCustomerIndex(customer->getId());
	customers->deleteNode(i);

	string type = customer->getType();
	string newType;
	if (type == "Regular")newType = "VIP";
	else if (type == "Guest") newType = "Regular";
	customers->add(createCustomer(customer->getId(), customer->getName(), customer->getAddress(), customer->getPhoneNumber(), newType));
	cout << "Promote customer " << customer->getId() << " successfully from " << type << " to " << newType << endl;
	waitForEnter();
}

// 4
void StoreManager::rentItem()
{
	Customer* customer = getExistCustomerInput();
	cout << customer->toConsoleString();
	if (customer->getType() == "Guest" && customer->getNumberOfRentals() >= 2) {
		cout << "Guest customer cannot rent more than 2 items at a time!" << endl;
		waitForEnter();
		return;
	}
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
	if (customer->canRentForFree()) {
		cout << "Do you want to use 100 VIP points to rent this item for free? Current VIP points: " << customer->getPoints() << endl;
		bool rentForFree = getYesNoInput();
		if (rentForFree) {
			costString = "0.00";
			customer->consumePoints(100);
			cout << "Consumed 100 VIP points! Current VIP points: " << customer->getPoints() << endl;
		}
	}
	cout << "Rent item successfully! Cost($): " << costString << endl;
	customer->rentItem(item->getId());
	item->rented();
	waitForEnter();
}

// 5
void StoreManager::returnItem()
{
	Customer* customer = getExistCustomerInput();
	cout << customer->toConsoleString();
	Item* item = getExistItemInput();
	// If the item is in the customer rental list
	if (customer->returnItem(item->getId())) {
		cout << "Return item sucessfully!" << endl;
		item->returned();
		if (customer->getType() == "VIP") cout << "Receive 10 VIP points! Current VIP points: " << customer->getPoints() << endl;
		waitForEnter();
		return;
	}
	// If the item is not rented by the customer
	cout << "Item " << item->getId() << " is not rented by " << customer->getId() << endl;
	waitForEnter();
}
// 6 - TODO: sort by id, title
void StoreManager::displayAllItems()
{
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
	cout << items->getSize() << " item(s)" << endl;
	auto current = items->getHead();
	while (current) {
		cout << current->data->toConsoleString();
		current = current->next;
	}
	waitForEnter();
}

// 7 - TODO: sort by id, title 
void StoreManager::displayOutOfStockItems()
{
	stringstream ss;
	int count = 0;
	auto current = items->getHead();
	while (current) {
		// Only print the item if it is out of stock
		if (!current->data->isAvailable()) {
			ss << current->data->toConsoleString();
			count++;
		}
		current = current->next;
	}
	cout << count << " out-of-stock item(s)" << endl;
	cout << ss.str();
	waitForEnter();
}

//8 - TODO: sort by id, name
void StoreManager::displayAllCustomers()
{
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

	cout << "There are " << customers->getSize() << " customer(s)" << endl;
	auto current = customers->getHead();
	while (current) {
		cout << "---------------" << endl;
		cout << current->data->toConsoleString();
		current = current->next;
	}
	waitForEnter();
}

// 9 - TODO: sort by id, name
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
	stringstream ss;
	auto current = customers->getHead();
	int count = 0;
	while (current) {
		if (current->data->getType() == type) {
			ss << "---------------" << endl;
			ss << current->data->toConsoleString();
			count++;
		}
		current = current->next;
	}
	cout << count << " " << type << " customer(s)" << endl;
	cout << ss.str();
	waitForEnter();
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
	string id = getItemIdInput();
	Item* item = getItemById(id);
	if (item == nullptr) {
		cout << "Item with ID " << id << " does not exist!" << endl;
		return;
	}
	cout << item->toConsoleString();

}
//10.2
void StoreManager::searchItemByTitle() {
	string title = getItemTitleInput();
	int count = 0;
	stringstream ss;
	auto current = items->getHead();
	while (current) {
		if (current->data->getTitle() == title) {
			ss << current->data->toConsoleString();
			count++;
		}
		current = current->next;
	}
	if (count > 0) {
		cout << count << " item(s) with title " << title << endl;
		cout << ss.str();
		return;
	}
	cout << "Item with title " << title << " does not exist!" << endl;
}

//10.3
void StoreManager::searchCustomerById() {
	string id = getCustomerIdInput();
	Customer* customer = getCustomerById(id);
	if (customer == nullptr) {
		cout << "Customer with ID " << id << " does not exist!" << endl;
		return;
	}
	cout << customer->toConsoleString();
}

//10.4
void StoreManager::searchCustomerByName() {
	string name = getCustomerNameInput();
	int count = 0;
	stringstream ss;
	auto current = customers->getHead();
	while (current) {
		if (current->data->getName() == name) {
			ss << "---------------" << endl;
			ss << current->data->toConsoleString();
			count++;
		}
		current = current->next;
	}
	if (count > 0) {
		cout << count << " customer(s) with name " << name << endl;
		cout << ss.str();
		return;
	}
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