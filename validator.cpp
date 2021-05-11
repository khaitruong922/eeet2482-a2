#pragma warning(disable : 4996)
#include <iostream>
#include "validator.h"
using namespace std;

// This file contains all the functions which validate the input string from the user in multiple field types.

bool isNonNegativeInteger(const string& s)
{
	int i;
	char c;
	string s1 = s;
	// Make sure an integer is found with no trailing character
	int matches = sscanf(s.c_str(), "%d%c", &i, &c);
	// Return true if it is an integer and not negative
	return matches == 1 && i >= 0;
}

bool isNonNegativeNumber(const string& s) {
	double d;
	char c;
	string s1 = s;
	// Make sure a double is found with no trailing character
	int matches = sscanf(s.c_str(), "%lf%c", &d, &c);
	// Return false if it is a double and not negative
	return matches == 1 && d >= 0;
}

bool isValidItemId(const string& s) {
	// False if it is not 9 characters
	if (s.length() != 9) return false;
	// False if ID does not start with I
	if (s[0] != 'I') return false;
	// False if the 5th character is not hyphen
	if (s[4] != '-') return false;
	// False if any remaining character is not digit
	for (int i = 1; i <= 3; i++) {
		if (!isdigit(s[i])) return false;
	}
	for (int i = 5; i <= 8; i++) {
		if (!isdigit(s[i])) return false;
	}
	// Item ID is valid
	return true;
}

bool isValidCustomerId(const string& s) {
	// False if it is not 4 characters
	if (s.length() != 4) return false;
	// False if the first character is not C
	if (s[0] != 'C') return false;
	// False if any remaining character is not digit
	for (int i = 1; i <= 3; i++) {
		if (!isdigit(s[i])) return false;
	}
	// Customer ID is valid
	return true;
}

bool isNotEmpty(const string& s) {
	return !s.empty();
}

bool isEmpty(const string& s) {
	return s.empty();
}

bool isValidLoanType(const string& s) {
	return s == "1-week" || s == "2-day";
}
bool isValidRentalType(const string& s) {
	return s == "Game" || s == "DVD" || s == "Record";
}
bool isValidGenre(const string& s) {
	return s == "Action" || s == "Comedy" || s == "Horror" || s == "Drama";
}

bool hasComma(const string& s) {
	return s.find(',') != string::npos;
}

bool isNumeric(const string& s) {
	// Loop over all characters
	for (int i = 0; i < s.length();i++)
	{
		// If the character is not a digit, return false
		if (!isdigit(s[i]))return false;
	}
	return true;
}

bool isAlpha(const string& s) {
	// Loop over all characters
	for (int i = 0; i < s.length();i++)
	{
		// If the character is not an alphabetic character or a space, return false
		if (!isalpha(s[i]) && !isspace(s[i]))return false;
	}
	return true;
}