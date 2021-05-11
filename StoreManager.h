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
    // Database
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

    // Object creation methods
    Item* createItem(const string& id, const string& title, const string& rentalType, const string& loanType, int numberOfCopies, double rentalFee, const string& genre);
    Customer* createCustomer(const string& id, const string& name, const string& address, const string& phoneNumber, const string& type);


    // Object search methods
    Item* getItemById(const string& id);
    bool itemExists(const string& id) { return getItemById(id) != nullptr; }
    Customer* getCustomerById(const string& id);
    bool customerExists(const string& id) { return getCustomerById(id) != nullptr; }
    int getCustomerIndex(const string& id);
    int getItemIndex(const string& id);

    // Input functions
    string getNonExistItemIdInput();
    Item* getExistItemInput();
    string getNonExistCustomerIdInput();
    Customer* getExistCustomerInput();


    // Menu methods

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

    // 6
    void displayAllItems();

    // 7
    void displayOutOfStockItems();

    //8
    void displayAllCustomers();

    // 9
    void displayGroupOfCustomers();

    // 10
    void displaySearchMenu();
    // Options
    void searchItemById();
    void searchItemByTitle();
    void searchCustomerById();
    void searchCustomerByName();

    // Exit
    void exit();
};