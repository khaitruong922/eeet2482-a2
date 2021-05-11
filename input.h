#include <iostream>

using namespace std;

string getItemIdInput();
string getItemTitleInput();
string getItemLoanTypeInput();
int getItemNumberOfCopiesInput();
double getItemRentalFeeInput();
string getItemGenreInput();
string getCustomerIdInput();
string getCustomerNameInput();
string getCustomerAddressInput();
string getCustomerPhoneNumberInput();
bool getYesNoInput();
int getOptionInput(int min, int max);
int getOptionInput(int min, int max, const string& exitString);
void waitForEnter();
string trim(const string& s);