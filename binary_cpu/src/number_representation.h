/*
 *
 */

#ifndef NUMBER_REPRESENTATION_H_
#define NUMBER_REPRESENTATION_H_


#include <iostream>
#include "constants.h"
#include <iostream>
#include <cmath>

using namespace std;


class Bit {
private:
	char value;

public:
	Bit(int n);
	Bit(char n);
	Bit();

	bool operator==(const Bit &b) const;
	char getValue();
};


//big-endian! (most significant at smallest address)
class Byte {
private:
	Bit array[BYTE_SIZE];

	void invert();

public:
	Byte();
	Byte(string str);
	Byte(Bit a[]);

	static string decToUBin4(int n);
	static string decToUBin8(int n);
	static string decToSBin8(int n);
	static string decToUBin16(int n);
	static string decToSBin16(int n);

	bool isCorrect();
	bool operator==(const Byte &b) const;
	string getBinaryValue();

	int convertToDec();
	int convertToUDec();
};


class Word {
private:
	Bit array[WORD_SIZE];

	void invert();

public:
	Word();
	Word(string str);
	Word(Bit a[]);
	Word(Byte b);

	Word(int n); //unsigned word

	Word operator+(const Word &w) {
		return Word(this->convertToUDec() + w.convertToUDec());
	}

	bool isCorrect();
	bool operator==(Word &w);
	string getBinaryValue();

	int convertToDec();
	int convertToUDec() const;
};


class WordPseudo {
private:
	Bit array[WORD_PSEUDO_SIZE];

public:
	WordPseudo();
	WordPseudo(string str);

	bool isCorrect();
	string getBinaryValue();
};


#endif /* NUMBER_REPRESENTATION_H_ */
