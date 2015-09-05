#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
	
    string encoded_string; string temp; 
    
    for (int i =0; i < numbers.size(); i++)
        {
        temp = convertNumberToBitString(numbers[i]);
        for (int j = 0; j <temp.size(); j++)
            {
                if (temp[j] == '0')
                    encoded_string+= ' ';
                else if (temp[j] == '1')
                    encoded_string += '\t';
            }
        }
    
    
    // convert to bit string 
    // go through bit string if you see a 0 put tab or something.. 
    
    return encoded_string;  // This compiles, but may not be correct
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
    string binaryString;
    for (int i = 0; i < bitString.size();)
        {
        string temp = bitString.substr(i,16); unsigned short number;
        if (temp.size() != 16)
            return false;
        for (int j = 0 ; j < temp.size(); j++)
            {
            if (temp[j] != ' ' && temp[j] != '\t')
                return false;
            if (temp[j] == ' ')
                binaryString+= '0';
            else if (temp[j] == '\t')
                binaryString+= '1';
            }
        convertBitStringToNumber(binaryString, number);
        binaryString = "";
        numbers.push_back(number);
        i+=16;
        }
    
    return true;  // This compiles, but may not be correct
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}

