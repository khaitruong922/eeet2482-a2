#pragma once
#include "List.h"
#include "Customer.h"
#include "Item.h"
using namespace std;
class StoreManager {
public:
    StoreManager(const string& customers_file_name, const string& items_file_name);
    ~StoreManager();
    bool loadDataFromFiles();
    void displayMenu();
private:
    // Members
    string customers_file_name;
    string items_file_name;
    List<Customer*>* customers;
    List<Item*>* items;

    // File I/O methods
    bool loadCustomersFromFile();
    bool loadItemsFromFile();
    void saveData();
    void saveCustomers();
    void saveItems();

    // Object creation functions
    Item* createItem(const string& id, const string& title, const string& rentalType, const string& loanType, int numberOfCopies, double rentalFee, const string& genre);
    Customer* createCustomer(const string& id, const string& name, const string& address, const string& phoneNumber, const string& type);


    // Object search functions
    Item* getItemById(const string& id);
    Item* getItemByTitle(const string& title);
    bool itemExists(const string& id) { return getItemById(id) != nullptr; }
    Customer* getCustomerById(const string& id);
    Customer* getCustomerByName(const string& name);
    bool customerExists(const string& id) { return getCustomerById(id) != nullptr; }
    int getCustomerIndex(const string& id);
    int getItemIndex(const string& id);

    // Input functions
    string getNonExistItemIdInput();
    Item* getExistItemInput();
    string getNonExistCustomerIdInput();
    Customer* getExistCustomerInput();


    // Menu functions

    // 1
    void displayItemMenu();
    // Options
    void addItem();
    void addItemCopiesToStock();
    void updateItem();
    void deleteItem();

    // 2
    void displayCustomerMenu();
    // Options
    void addCustomer();
    void updateCustomer();

    // 3
    void promoteCustomer();

    // 4
    void rentItem();

    // 5
    void returnItem();

    // 6 - TODO: sort by id, title
    void sortItembyID();
    void displayAllItems();

    // 7 - TODO: sort by id, title 
    void displayOutOfStockItems();

    //8 - TODO: sort by id, name
    void displayAllCustomers();

    // 9 - TODO: sort by id, name
    void displayGroupOfCustomers();

    // 10
    void displaySearchMenu();
    // Options
    void searchItemById();
    void searchItemByTitle();
    void searchCustomerById();
    void searchCustomerByName();

    // 11
    void exit();
};