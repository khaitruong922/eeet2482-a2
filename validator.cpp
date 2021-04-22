#include <iostream>
#include <string>
#include <fstream>
#include "validator.h"
#pragma warning(disable : 4996)
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

bool isValidItemId(const string& id) {
	if (id.length() != 9) return false;
	if (id[0] != 'I') return false;
	if (id[4] != '-') return false;
	for (int i = 1; i <= 3; i++) {
		if (!isdigit(id[i])) return false;
	}
	for (int i = 5; i <= 8; i++) {
		if (!isdigit(id[i])) return false;
	}
	return true;
}

bool isValidCustomerId(const string& id) {
	if (id.length() != 4) return false;
	if (id[0] != 'C') return false;
	for (int i = 1; i <= 3; i++) {
		if (!isdigit(id[i])) return false;
	}
	return true;
}

bool isNotEmpty(const string& s) {
	return !s.empty();
}

