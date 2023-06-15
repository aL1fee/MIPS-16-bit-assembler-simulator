/*
 *
 */

#include "ALU.h"

Word ALU::rshift(Word w, int n) {
	//check n bounds
	string str = w.getBinaryValue();
	string retStr = Word().getBinaryValue();

	for (int i = 0; i < str.size(); i++) {
		if (i - n >= 0) {
			retStr[i] = str[i - n];
		}
	}
	return Word(retStr);
}

Word ALU::lshift(Word w, int n) {
	//check n bounds
	string str = w.getBinaryValue();
	string retStr = Word().getBinaryValue();

	for (int i = 0; i < str.size(); i++) {
		if (i - n >= 0) {
			retStr[i - n] =str[i];
		}
	}
	return Word(retStr);
}

Word ALU::invert(Word w) {
	string str = w.getBinaryValue();
	for (int i = 0; i < str.size(); i++) {
		str[i] == '0' ? str[i] = '1' : str[i] = '0';
	}
	return Word(str);
}

Word ALU::orOp(Word w1, Word w2) {
	string str1 = w1.getBinaryValue();
	string str2 = w2.getBinaryValue();
	for (int i = 0; i < str1.size(); i++) {
		(str1[i] == '1') || (str2[i] == '1') ? str1[i] = '1' : str1[i] = '0';
	}
//	cout << "ALU PRINTS HERE: " << str1 << endl;
	return Word(str1);
}


//arithmetic overflow causes an exception
//in case of an overflow, the result is not written to the destination register
Word ALU::add(Word w1, Word w2) {
	string str1 = w1.getBinaryValue();
	string str2 = w2.getBinaryValue();
	char str1sign = str1[0];
	char str2sign = str2[0];
	int carry = 0;


	for (int i = WORD_SIZE - 1; i >= 0; i--) {
		if (str1[i] != str2[i]) {
			if (carry == 0) {
				str1[i] = '1';
 			} else {
 				str1[i] = '0';
 				carry = 1;
 			}
		} else if (str1[i] == '0' && carry == 1) {
			str1[i] = '1';
			carry = 0;
		} else if (str1[i] == '1') {
			if (carry == 0) {
				str1[i] = '0';
				carry = 1;
			} else {
				str1[i] = '1';
				carry = 1;
			}
		}
	}

	//overflow check
	// pos + pos = neg    -> overflow
	// neg + neg = pos    -> overflow
	if (str1sign == str2sign && str1sign != str1[0]) {
		return Word(INCORRECT_WORD_STRING);
	}

	return Word(str1);
}

Word ALU::subtract(Word w1, Word w2) {
	Word w2TwosCompl = add(invert(w2), Word(1));
	return add(w1, w2TwosCompl);
}

// a shortcut; this isn't how the division is implemented in the CPU!
pair<Word, Word> ALU::div(Word w1, Word w2) {
	pair<Word, Word> result;
	int dividend = w1.convertToDec();
	int divisor = w2.convertToDec();
	int resultDiv = dividend / divisor;
	int resultMod = (dividend) % divisor;
	result.first = Word(Byte::decToSBin16(resultMod));
	result.second = Word(Byte::decToSBin16(resultDiv));
	return result;
}
