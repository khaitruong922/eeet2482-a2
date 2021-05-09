#pragma warning(disable : 4996)
#include <iostream>
#include "validator.h"
using namespace std;

bool isNonNegativeInteger(const string& s)
{
	int i;
	char c;
	string s1 = s;
	int matches = sscanf(s.c_str(), "%d%c", &i, &c);
	return matches == 1 && i >= 0;
}

bool isNonNegativeNumber(const string& s) {
	double d;
	char c;
	string s1 = s;
	int matches = sscanf(s.c_str(), "%lf%c", &d, &c);
	return matches == 1 && d >= 0;
}

bool isValidItemId(const string& s) {
	if (s.length() != 9) return false;
	if (s[0] != 'I') return false;
	if (s[4] != '-') return false;
	for (int i = 1; i <= 3; i++) {
		if (!isdigit(s[i])) return false;
	}
	for (int i = 5; i <= 8; i++) {
		if (!isdigit(s[i])) return false;
	}
	return true;
}

bool isValidCustomerId(const string& s) {
	if (s.length() != 4) return false;
	if (s[0] != 'C') return false;
	for (int i = 1; i <= 3; i++) {
		if (!isdigit(s[i])) return false;
	}
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
	for (int i = 0; i < s.length();i++)
	{
		if (!isdigit(s[i]))return false;
	}
	return true;
}
bool isAlpha(const string& s) {
	for (int i = 0; i < s.length();i++)
	{
		if (!isalpha(s[i]) && !isspace(s[i]))return false;
	}
	return true;
}

bool isAlnum(const string& s) {
	for (int i = 0; i < s.length();i++)
	{
		if (!isalnum(s[i]) && !isspace(s[i]))return false;
	}
	return true;
}