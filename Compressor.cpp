#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    unsigned int N = s.size(); numbers.clear();
    unsigned int capacity = !(((N/2)+512)<16384)?16384:((N/2)+512), numBuckets = 2*capacity;
    HashTable<string, unsigned short> H (numBuckets,capacity); 
    for (int j = 0 ; j < 256; j++)
        {
        string toSetPermanent (1,static_cast<char>(j)); 
        H.set(toSetPermanent, j, true);
        }
    unsigned int nextFreeID = 256; string runSoFar; 
    for (int i = 0; i < N ; i++)
        {
        string expandedRun  = runSoFar + s[i]; unsigned short value;
        if (H.get(expandedRun, value))
            {
            runSoFar = expandedRun;
            continue;
            }
        else{
            unsigned short x;
            H.get(runSoFar, x);
            numbers.push_back(x);
            H.touch(runSoFar);
            runSoFar = ""; string c(1,static_cast<char>(s[i])); unsigned short cv; // hope this works 
            H.get(c, cv);
            numbers.push_back(cv);
            if (!H.isFull())
                {
                H.set(expandedRun, nextFreeID);
                nextFreeID++;
                }
            else{
                string discarded; unsigned short val_discarded;
                H.discard(discarded, val_discarded);
                H.set(expandedRun, val_discarded);
            }
        }
        
        }
    if (!runSoFar.empty())
        {
        unsigned short x;
        H.get(runSoFar, x);
        numbers.push_back(x);
        }
    numbers.push_back(capacity);
   
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
    unsigned int capacity  = numbers[numbers.size()-1];
    unsigned int numBuckets = 2* capacity;
    HashTable<unsigned short , string> H(numBuckets,capacity); 
    for (int j = 0 ; j < 256; j++)
        {
        string toSetPermanent (1,static_cast<char>(j)); 
        H.set(j, toSetPermanent, true);
        }
    unsigned int nextFreeID = 256; string runSoFar, output;
    unsigned short us; string value; 
    for (int i = 0 ; i < (numbers.size()-1) ; i++)
        {
        us = numbers[i];
        if (us <= 255)
            {
            H.get(us, value);
            output+=us;
            
            if (runSoFar == "")
            {
            runSoFar = value;
            continue;
            }
            else if (runSoFar != "") {
            string expandedRun = runSoFar+value;
        
            if (!H.isFull())
            {
            H.set(nextFreeID, expandedRun); nextFreeID++;
            }
            
            else{
            string value; unsigned short key;
            H.discard(key, value);
            H.set(key, expandedRun);
        }  
            
            runSoFar = "" ; 
            continue;
            }
            }
        else{
            string S;
            if (!H.get (us,S))
                { cerr << "no association of us in the table" <<endl; return false;}
            else{
                H.get(us, S);
                H.touch(us);
                output+=S;
                runSoFar = S;
                continue;
            }
            
        }
        }
    s = output;
    return true;
}

 
// converts strings to a hash value between 0 and numBuckets - client provides this
unsigned int computeHash(string key) // only works for an int and a string 
{
    
    unsigned int h = 2166136261U;  
    for (int i = 0; i < key.size(); i++)
        {
        h+= key[i];
        h*=16777619;
        }
    return h;
}

unsigned int computeHash(unsigned short key)
{
    return key; 
}


/*
int main()
{
    string s = "Whaddup";//"    \t \t\t\t \t     \t\t \t \t  \t\t\t \t\t ";
    vector<unsigned short> numbers; vector <unsigned short> encoding;
    Compressor::compress(s, numbers);
    string encoded = BinaryConverter::encode(numbers);
    // decode is gonna take the string and put stuff in the vector
    BinaryConverter::decode(encoded, encoding);
    Compressor::decompress(encoding, s);
    cerr << s <<endl;
    cerr << encoded;
}
*/
