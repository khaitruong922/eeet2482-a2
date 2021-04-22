#include <iostream>
#include <string>
#include <fstream>
#include "Customer.cpp"
#include "validator.h"
#include "input.h"

using namespace std;

#ifndef STORE_MANAGER
#define STORE_MANAGER
class StoreManager {
  public:
    StoreManager(const string& customers_file_name, const string& items_file_name) {
      this->customers_file_name = customers_file_name;
      this->items_file_name = items_file_name;
      customers = new List<Customer*>();
      items = new List<Item*>();
    }

    ~StoreManager() {
      cout << "Delete store manager" << endl;
      // TODO: delete accounts pointers
      // TODO: delete items pointers
      delete customers;
      delete items;
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
      cout << "11. Exit" << endl;
      // Ask for user input
      int option = getOptionInput(1, 11);
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
      else if (option == 11){
        exit();
        return;
      }
      displayMenu();
    }
    bool loadDataFromFiles() {
      if (!loadItemsFromFile()) return false;
      if (!loadCustomersFromFile()) return false;
      return true;
    }
  private:
    // Members
    string customers_file_name;
    string items_file_name;
    List<Customer*>* customers;
    List<Item*>* items;

    bool loadCustomersFromFile() {
      ifstream infile(customers_file_name);
      if (!infile)
      {
        cout << "Cannot open file " << customers_file_name << endl;
        return false;
      }
      cout << "Customers file: " << customers_file_name << endl;

      string line;
      string delim = ",";

      Customer* lastCustomer = nullptr;
      while (getline(infile, line)) {
        // Check if line is item ID
        if (isValidItemId(line)){
          if(lastCustomer) lastCustomer->getItemIds()->add(line);
          continue;
        }
        // If not, line is customer info
        int start = 0;
        int end = line.find(delim);
        int i = 0;
        string fields[6];
        // Loop until there are 6 fields
        while (end != string::npos && i < 6) {
          string field = line.substr(start, end - start);
          start = end + delim.length();
          end = line.find(delim, start);
          fields[i] = field;
          i++;
        }
        // Get last field
        fields[i] = line.substr(start, end - start);
        i++;
        // Skip if there are not enough fields
        if (i<6) continue;
        string id = fields[0];
        // Skip if id already exists
        if (customerExists(id)) continue;
        string name = fields[1];
        string address = fields[2];
        string phoneNumber = fields[3];
        string type = fields[5];
        Customer* customer = createCustomer(id,name,address,phoneNumber,type);
        // Skip if the customer cannot be created
        if (customer == nullptr){
          lastCustomer = nullptr;
          delete customer;
          continue;
        }
        customers->add(customer);
        lastCustomer = customer;
      }
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
        //Skip if the item id alreasy exists
        if (itemExists(id)) {
          continue;
        }
        string title = fields[1];
        string rentalType = fields[2];
        string loanType = fields[3];
        int numberOfCopies = stoi(fields[4]);
        double rentalFee = stod(fields[5]);
        string genre = fields[6];
        Item* item = createItem(id, title, rentalType, loanType, numberOfCopies, rentalFee, genre);
        // Skip if the item cannot be created
        if (item == nullptr){
          delete item;
          continue;
        }
        items->add(item);
      }
      infile.close();
      return true;
    }

    void saveData() {
      // saveItems();
      // saveCustomers();
    }
    void saveCustomers() {
      ofstream outfile(customers_file_name);
      if (!outfile)
      {
        cout << "Cannot save file " << customers_file_name << endl;
        return;
      }

      outfile.close();
    }
    void saveItems() {
      ofstream outfile(items_file_name);
      if (!outfile)
      {
        cout << "Cannot save file " << items_file_name << endl;
        return;
      }

      outfile.close();
    }
    // Return the item pointer from parameters, return nullptr if the rental type is not Record, DVD or Game
    Item* createItem(const string& id, const string& title, const string& rentalType, const string& loanType, int numberOfCopies, double rentalFee, const string& genre) {
      if (rentalType == "Record") return new Record(id, title, loanType, numberOfCopies, rentalFee, genre);
      if (rentalType == "DVD") return new DVD(id, title, loanType, numberOfCopies,rentalFee, genre);
      if (rentalType == "Game") return new Game(id, title, loanType, numberOfCopies, rentalFee);
      return nullptr;
    }
    
    // Return the customer pointer from parameters, return nullptr if the account type is not Guest, Regular or VIP
    Customer* createCustomer(const string& id, const string& name, const string& address, const string& phoneNumber, const string& type){
      if (type == "Guest") return new GuestCustomer(id,name,address,phoneNumber);
      if (type == "Regular") return new RegularCustomer(id,name,address,phoneNumber);
      if (type == "VIP") return new VIPCustomer(id,name,address,phoneNumber);
      return nullptr;
    }

    Item* getItemById(const string& id){
      auto head = items->getHead();
      while(head){
        if (head->data->getId()==id) return head->data;
        head = head->next;
      }
      return nullptr;
    }

    bool itemExists(const string& id){return getItemById(id) != nullptr;}
    string getNonExistItemIdInput(){
      string input;
      while(true){
        input = getItemIdInput();
        if (!itemExists(input)) return input;
        cout << "Item ID " << input << " already exists!" << endl;
      }
      return input;
    }
    string getExistItemIdInput(){
      string input;
      while(true){
        input = getItemIdInput();
        if (itemExists(input)) return input;
        cout << "Item ID " << input << " does not exist!" << endl;
      }
      return input;
    }

    Customer* getCustomerById(const string& id){
      auto head = customers->getHead();
      while(head){
        if (head->data->getId()==id) return head->data;
        head = head->next;
      }
      return nullptr;
    }

    string getNonExistCustomerIdInput(){
      string input;
      while(true){
        input = getCustomerIdInput();
        if (!customerExists(input)) return input;
        cout << "Customer ID " << input << " already exists!" << endl;
      }
      return input;
    }

    string getExistCustomerIdInput(){
      string input;
      while(true){
        input = getCustomerIdInput();
        if (customerExists(input)) return input;
        cout << "Customer ID " << input << " does not exist!" << endl;
      }
      return input;
    }

    bool customerExists(const string& id){return getCustomerById(id)!= nullptr;}

    // Menu functions

    // 1.1 - done?
    void addItem()
    {
      cout << "-------------------------------" << endl;
      cout << "Select item type" << endl;
      cout << "1. Record" << endl;
      cout << "2. DVD" << endl;
      cout << "3. Game" << endl;
      cout << "or" << endl;
      cout << "4. Back" << endl;
    // ngan gon
      int option = getOptionInput(1, 4);
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
      case 4:
        return;
      }
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

    // 1.2 
    void updateItem()
    {
      cout << "------------------------" << endl;
      cout << "1. Update title" << endl;
      cout << "2. Update loan type" << endl;
      cout << "3. Update rental fee" << endl;
      cout << "4. Update genre" << endl;
      cout << "5. Back "<< endl;
      int option = getOptionInput(1, 5);
      string id; 
    }

    // 1.3 - Thuy / Quy
    void deleteItem()
    {
      string id = getExistItemIdInput();
    }

    // 1.4 - done
    void addItemToStock()
    {
      string id = getExistItemIdInput();
      int numberOfCopies = getItemNumberOfCopiesInput();
      Item* item = getItemById(id);
      item->addToStock(numberOfCopies);
    }

    // Menu
    void displayItemMenu()
    {
      cout << "-------------------------------" << endl;
      cout << "Manage items" << endl;
      cout << "1. Add new item" << endl;
      cout << "2. Add item to stock" << endl;
      cout << "3. Update item" << endl;
      cout << "4. Delete item" << endl;
      cout << "5. Back" << endl;

      int option = getOptionInput(1,5);
      if (option == 1) addItem();
      else if (option ==2) addItemToStock();
      else if (option ==3) updateItem();
      else if (option ==4) deleteItem();
      else if (option ==5) return;
      displayItemMenu();
    }

    // 2
    // 2.1
    void addCustomer()
    {
      string id = getNonExistCustomerIdInput();
      string name = getCustomerNameInput();
      string phoneNumber = getCustomerPhoneNumberInput();
      string address = getCustomerAddressInput();
      string type = "Guest";
      Customer* customer = createCustomer(id,name,address,phoneNumber,type);
      if (customer == nullptr){
        cout << "Cannot add customer " << id << endl;
        delete customer;
        return;
      }
      customers->add(customer);
      cout << "Add customer " << id << " successfully!" << endl;
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
      cout << "1. Add customer" << endl;
      cout << "2. Update customer" << endl;
      cout << "3. Back" << endl;
      int option = getOptionInput(1, 3);
      if (option == 1) addCustomer();
      else if (option==2) updateCustomer();
      else if (option ==3) return;
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

    // 6 - TODO: sort by id, title
    void displayAllItems()
    {
      auto head = items->getHead();
      while (head) {
        cout << head->data->toString();
        head = head->next;
      }
    }

    // 7 - TODO: sort by id, title 
    /*
    • The ability to display all items, sorted by titles or IDs.
    */
    void displayOutOfStockItems()
    {
      auto head = items->getHead();
      while (head) {
        // Only print the item if it is out of stock
        if (!head->data->isAvailable()) cout << head->data->toString();
        head = head->next;
      }
    }

    //8 - TODO: sort by id, name
    void displayAllCustomers()
    {
      auto head = customers->getHead();
      while (head) {
        cout << head->data->toString();
        head = head->next;
      }
    }

    // 9
    void displayGroupOfCustomers()
    {
      cout << "-------------------------------" << endl;
      cout << "Select customer group" << endl;
      cout << "1. Guest" << endl;
      cout << "2. Regular" << endl;
      cout << "3. VIP" << endl;
      cout << "or " << endl;
      cout << "4. Back" << endl;

      int option = getOptionInput(1, 4);
      string type;
      if (option == 1) type = "Guest";
      else if (option ==2) type = "Regular";
      else if (option == 3) type = "VIP";
      else if (option ==4) return;
      auto head = customers->getHead();
      while (head) {
        if(head->data->getType() == type) cout << head->data->toString();
        head = head->next;
      }
    }

    // 10
    void displaySearchMenu()
    {
      cout << "-------------------------------" << endl;
      cout << "1. Search item by id" << endl;
      cout << "2. Search item by title" << endl;
      cout << "3. Search customer by id" << endl;
      cout << "4. Search customer by name" << endl;
      cout << "5. Back" << endl;
    }

    void searchItemById(){
      string id = getItemIdInput();
      Item *item = getItemById(id);
      if (item == nullptr){
        cout << "Item with id " << id  << " does not exist!" << endl;
      }
    }
    void searchItemByTitle(){
      
    }

    void searchCustomerById(){

    }

    void searchCustomerByName(){

    }

    // 11
    void exit()
    {
      cout << "ASSIGNMENT 2 GROUP 26" << endl;
      cout << "s3818074, s3818074@rmit.edu.vn, Khai, Truong" << endl;
      cout << "s3878636, s3878636@rmit.edu.vn, Quy, Nguyen" << endl;
      cout << "s3877746, s3877746@rmit.edu.vn, Thuy, Mai" << endl;
      cout << "s3905273, s3905273@rmit.edu.vn, Nam, Pham" << endl;
      saveData();
    }

};
#endif