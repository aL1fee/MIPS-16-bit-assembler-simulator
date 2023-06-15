/*
 *
 */

#include "number_representation.h"


Bit::Bit(int n) {
	value = static_cast<char>(n + ASCII_INT_TO_CHAR_INTERVAL);
}

Bit::Bit(char n) {
	value = n;
}

Bit::Bit() {
	value = '0';
}

bool Bit::operator==(const Bit &b) const {
	return value == b.value;
}

char Bit::getValue() {
	return value;
}



Byte::Byte() {
	for (int i = 0; i < BYTE_SIZE; i++) {
		array[i] = Bit(0);
	}
}

Byte::Byte(string str) {
	if (str.size() == BYTE_SIZE) {
		for (int i = 0; i < WORD_SIZE; i++) {
			array[i] = Bit(str[i]);
		}
	}
	else if (str.size() == HALF_BYTE_SIZE) {
		for (int i = 0; i < HALF_BYTE_SIZE; i++) {
			array[i] = Bit(0);
		}
		for (int i = HALF_BYTE_SIZE; i < BYTE_SIZE; i++) {
			array[i] = str[i - HALF_BYTE_SIZE];
		}
	}
}

Byte::Byte(Bit a[]) {
	Byte();
	// check for errors;
	for (int i = 0; i < BYTE_SIZE; i++) {
		array[i] = a[i];
	}
}

void Byte::invert() {
	for (int i = 0; i < BYTE_SIZE; i++) {
		array[i] == '0' ? array[i] = '1' : array[i] = '0';
	}
}

bool Byte::isCorrect() {
	for (int i = 0; i < BYTE_SIZE; i++) {
		if (array[i].getValue() != '0' && array[i].getValue() != '1') {
			return false;
		}
	}
	return true;
}

string Byte::getBinaryValue() {
	char ret[BYTE_SIZE];
	for (int i = 0; i < BYTE_SIZE; i++) {
		ret[i] = array[i].getValue();
	}
	return string(ret, BYTE_SIZE);
}

string Byte::decToUBin4(int n) {
	if (n < 0 || n > 15) {
		return INCORRECT_HALF_BYTE_STRING;
	}
	string ret = EMPTY_HALF_BYTE_STRING;
	int i = HALF_BYTE_SIZE - 1;
	while (n != 0) {
		ret[i--] = n % 2 + ASCII_INT_TO_CHAR_INTERVAL;
		n /= 2;
	}
	return ret;
}

string Byte::decToUBin8(int n) {
	if (n < MIN_8_BIT_U_NUMBER || n > MAX_8_BIT_U_NUMBER) {
		return INCORRECT_BYTE_STRING;
	}
	string ret = EMPTY_BYTE_STRING;
	int i = BYTE_SIZE - 1;
	while (n != 0) {
		ret[i--] = n % 2 + ASCII_INT_TO_CHAR_INTERVAL;
		n /= 2;
	}
	return ret;
}

string Byte::decToSBin8(int n) {
	if (n < MIN_8_BIT_2SC_NUMBER || n > MAX_8_BIT_2SC_NUMBER) {
		return INCORRECT_BYTE_STRING;
	}
	bool negative = n < 0;
	if (negative) {
		n = abs(n) - 1;
	}
	string ret = EMPTY_BYTE_STRING;
	int i = BYTE_SIZE - 1;
	while (n != 0) {
		ret[i--] = n % 2 + ASCII_INT_TO_CHAR_INTERVAL;
		n /= 2;
	}
	if (negative) {
		for (int i = 0; i < ret.size(); i++) {
			ret[i] == '0' ? ret[i] = '1' : ret[i] = '0';
		}
	}
	return ret;
}


string Byte::decToUBin16(int n) {
	if (n < MIN_16_BIT_U_NUMBER || n > MAX_16_BIT_U_NUMBER) {
		return INCORRECT_WORD_STRING;
	}
	string ret = EMPTY_WORD_STRING;
	int i = WORD_SIZE - 1;
	while (n != 0) {
		ret[i--] = n % 2 + ASCII_INT_TO_CHAR_INTERVAL;
		n /= 2;
	}
	return ret;
}

string Byte::decToSBin16(int n) {
	if (n < MIN_16_BIT_2SC_NUMBER || n > MAX_16_BIT_2SC_NUMBER) {
		return INCORRECT_WORD_STRING;
	}
	bool negative = n < 0;
	if (negative) {
		n = abs(n) - 1;
	}
	string ret = EMPTY_WORD_STRING;
	int i = WORD_SIZE - 1;
	while (n != 0) {
		ret[i--] = n % 2 + ASCII_INT_TO_CHAR_INTERVAL;
		n /= 2;
	}
	if (negative) {
		for (int i = 0; i < ret.size(); i++) {
			ret[i] == '0' ? ret[i] = '1' : ret[i] = '0';
		}
	}
	return ret;
}

int Byte::convertToUDec() {
	int sum = 0;
	for (int i = BYTE_SIZE - 1; i > 0; i--) {
		if (array[i] == '1') {
			sum += pow(2, BYTE_SIZE - 1 - i);
		}
	}
	return sum;
}

int Byte::convertToDec() {
	int sum = 0;
	int isNegative = 1;
	if (array[0] == '1') {
		isNegative = -1;
		invert();
	}
	for (int i = BYTE_SIZE - 1; i > 0; i--) {
		if (array[i] == '1') {
			sum += pow(2, BYTE_SIZE - 1 - i);
		}
	}
	if (isNegative == -1) {
		sum += 1;
		invert();
	}
	return sum * isNegative;
}

Word::Word() {
	for (int i = 0; i < WORD_SIZE; i++) {
		array[i] = Bit(0);
	}
}

Word::Word(string str) {
	if (str.size() == WORD_SIZE) {
		for (int i = 0; i < WORD_SIZE; i++) {
			array[i] = Bit(str[i]);
		}
	} else if (str.size() == BYTE_SIZE) {
		for (int i = 0; i < WORD_SIZE; i++) {
			if (i < BYTE_SIZE) {
				array[i] = Bit(0);
			} else {
				array[i] = Bit(str[i - BYTE_SIZE]);
			}
		}
	}
}

Word::Word(Bit a[]) {
//	Word();
	// check for errors;
	for (int i = 0; i < WORD_SIZE; i++) {
		array[i] = a[i];
	}
}

Word::Word(Byte b) {
	string bStr = b.getBinaryValue();
	for (int i = 0; i < BYTE_SIZE; i++) {
		array[i] = '0';
	}
	for (int i = BYTE_SIZE; i < WORD_SIZE; i++) {
		array[i] = bStr[i - BYTE_SIZE];
	}
}

Word::Word(int n) {
	string str = Byte::decToUBin16(n);
	for (int i = 0; i < WORD_SIZE; i++) {
		array[i] = str[i];
	}
}

string Word::getBinaryValue() {
	char ret[WORD_SIZE];
	for (int i = 0; i < WORD_SIZE; i++) {
		ret[i] = array[i].getValue();
	}
	return string(ret, WORD_SIZE);
}

void Word::invert() {
	for (int i = 0; i < WORD_SIZE; i++) {
		array[i] == '0' ? array[i] = '1' : array[i] = '0';
	}
}

bool Word::isCorrect() {
	for (int i = 0; i < WORD_SIZE; i++) {
		if (array[i].getValue() != '0' && array[i].getValue() != '1') {
			return false;
		}
	}
	return true;
}

bool Word::operator==(Word &w) {
	return getBinaryValue() == w.getBinaryValue();
}

//differentiate b/n positive and negative
int Word::convertToDec() {
	int sum = 0;
	int isNegative = 1;

	//handle negative #s
	if (array[0] == '1') {
		isNegative = -1;
		invert();
	}

	for (int i = WORD_SIZE - 1; i > 0; i--) {
		if (array[i] == '1') {
			sum += pow(2, WORD_SIZE - 1 - i);
		}
	}
	if (isNegative == -1) {
		sum += 1;
		invert();
	}

	return sum * isNegative;
}

int Word::convertToUDec() const {
	int sum = 0;
	for (int i = WORD_SIZE - 1; i >= 0; i--) {
		if (array[i] == '1') {
			sum += pow(2, WORD_SIZE - i - 1);
		}
	}
	return sum;
}


WordPseudo::WordPseudo() {
	for (int i = 0; i < WORD_PSEUDO_SIZE; i++) {
		array[i] = Bit(0);
	}
}

WordPseudo::WordPseudo(string str) {
	for (int i = 0; i < WORD_PSEUDO_SIZE; i++) {
		array[i] = Bit(str[i]);
	}
}

bool WordPseudo::isCorrect() {
	for (int i = 0; i < WORD_PSEUDO_SIZE; i++) {
		if (array[i].getValue() != '0' && array[i].getValue() != '1') {
			return false;
		}
	}
	return true;
}


string WordPseudo::getBinaryValue() {
	char ret[WORD_PSEUDO_SIZE];
	for (int i = 0; i < WORD_PSEUDO_SIZE; i++) {
		ret[i] = array[i].getValue();
	}
	return string(ret, WORD_PSEUDO_SIZE);
}

