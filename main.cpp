#include <iostream>

#include "StoreManager.h"
using namespace std;

int main(int argc, char* argv[])
{

	if (argc != 3) {
		cout << "Please run the program by the following format: " << endl;
		cout << "assignment2_group26.exe [customers_file_name] [items_file_name]";
		return -1;
	}
	string customers_file_name = argv[1];
	string items_file_name = argv[2];

	// Default file names - comment this on submission
	//string customers_file_name = "customers.txt";
	//string items_file_name = "items.txt";

	// Create the store manager instance from two database files
	StoreManager storeManager(customers_file_name, items_file_name);
	// Exit program if files cannot be loaded
	if (!storeManager.loadDataFromFiles()) return -1;
	storeManager.displayMenu();
}