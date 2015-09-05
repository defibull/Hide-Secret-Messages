#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	if (hostIn.empty())
        return false;
    vector<string> lines; string temp; 
    for (int i = 0 ; i < hostIn.size(); i++)
        {
        int j = i; //int increment_i = 0; 
        for (;  hostIn[j] != '\n' && j != hostIn.size();j++)
            {
            temp += hostIn[j]; //  increment_i++;
            }
        i+=temp.size();
        if (!temp.empty() && temp[temp.size()-1] == '\r')
            temp = temp.substr(0, temp.size()-1);
        string temp_nospace; int trim = 0;
        for (int k =temp.size()-1 ; k > 0; k--)
            {
            if (temp[k] != ' ' && temp[k] != '\t')
                break;
            trim++;
            }
        temp_nospace = temp.substr(0, temp.size()- (trim+1)); 
        lines.push_back(temp_nospace);
        temp = ""; temp_nospace = "";
        }

    /*lines are fine - messes up after whitespace added
    cerr << "ARE LINES FINE ? " <<endl;
    for (int i = 0 ; i < lines.size(); i++)
        cerr << lines[i] <<endl;
    */
    
    vector<unsigned short> numbers; 
    Compressor::compress(msg,numbers);
    string translated = BinaryConverter::encode(numbers);
    // comparing the two 
    cerr << "Translated msg size " << translated.size() <<endl; 
    cerr << "Translated msg contains " << endl; 
    for (int i = 0 ; i < translated.size(); i++)
        {
        if (translated[i] == ' ')
            cerr << "_";
        else if (translated[i] == '\t')
            cerr<< "-";
        else
            {
            cerr << "?" ; 
            }
        }

    int N = lines.size(); int L = translated.size(); vector<string> substrings; 
    int startVal = 0 ; 
    for (int i = 0; i < L%N; i++)
        {
        substrings.push_back(translated.substr(startVal, (L/N)+1));
        startVal+=((L/N)+1);
        }
    int startVal_2 = startVal;
    for (int j = 0; j < (N-(L%N)); j++)
        {
        substrings.push_back(translated.substr(startVal_2, (L/N))); // no error check cause we trust that right numb
        startVal_2+=(L/N);
        }
    
    // substring array - error checking
    cerr << " the substring array has " << endl; int substringsTotal = 0;
    for (int j = 0 ; j < substrings.size(); j++)
        { for (int i = 0 ; i < substrings[j].size(); i++)
        {
        if (substrings[j][i] == ' ')
            {cerr << "_"; substringsTotal++;}
        else if (substrings[j][i] == '\t')
            {cerr<< "-";substringsTotal++;}
        else
            {
            cerr << "?" ; substringsTotal++;
            }
        }
        }
    cerr << " the substring array size " << substringsTotal << endl;
    cerr << endl;    //cerr << "substring size " <<substrings.size();
    
    for (int k = 0 ; k < lines.size(); k++)
        {
        lines[k]+=substrings[k];
        }
    hostOut = "";

    for (int l = 0; l < lines.size();l++)
        {
        hostOut+=lines[l];
        hostOut+='\n';
        }
    
    return true; 
}

bool Steg::reveal(const string& hostIn, string& msg) 
{
    if (hostIn.empty())
        return false;
    vector<unsigned short> encoding; string temp; string trailingSpace; int ultaTrailingSpacesize = 0;
    for (int i = 0 ; i < hostIn.size(); i++)
        {
        // int increment_i = 0; 
        for (int j = i;  hostIn[j] != '\n' && j != hostIn.size();j++)
            {
            temp += hostIn[j];// increment_i++;
            }
        i+=temp.size();
        if (!temp.empty() && temp[temp.size()-1] == '\r')
            temp = temp.substr(0, temp.size()-1);
        string ultaTrailingSpace;
        for (int i =temp.size()-1 ; i >= 0; i--)
            {
            if (temp[i] != ' ' && temp[i] != '\t')
                break;
            ultaTrailingSpace+=temp[i];
           // trailingSpace+=temp[k];
            }
        reverse(ultaTrailingSpace.begin(), ultaTrailingSpace.end());
        ultaTrailingSpacesize+=ultaTrailingSpace.size();
        trailingSpace+=ultaTrailingSpace;
        temp = "";
        }
    
    //testing for what's in trailing space
    cerr << "Ulta trailing space total size" << ultaTrailingSpacesize << endl;
    cerr << "Trailing space size " << trailingSpace.size() <<endl; 
    cerr << "Trailing space contains " << endl; 
    for (int i = 0 ; i < trailingSpace.size(); i++)
        {
            if (trailingSpace[i] == ' ')
                cerr << "_";
            else if (trailingSpace[i] == '\t')
                cerr<< "-";
            else
                {
                cerr << "?" ; 
                }
        }
    cerr << endl;
    
    BinaryConverter::decode(trailingSpace, encoding);
    // testing if trailing space works
   /* 
    for (int i =0 ; i < encoding.size(); i++)
        {
        cerr << encoding[i] <<endl;
        }
    */
    
    Compressor::decompress(encoding, msg);
    
    return true;

}


