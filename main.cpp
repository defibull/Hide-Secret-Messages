// We have not yet produced the test driver main routine for you.
#include "provided.h"
#include "http.h"
#include <iostream>
using namespace std;

int main()
{
    string originalPageText = 
    "<html>\n"
    "<head>\n"
    " <title>My Pretend Web Page</title>\n"
    "</head>\n"
    "<body>\n"
    "<h2>My Pretend Web Page<h1>\n"
    "<p>\n"
    "I wish I were creative enough to have something interesting \n"
    "to say here.  I'm not, though, so this is a boring page.\n"
    "</p>\n"
    "<p>\n"
    "Oh, well.\n"
    "</p>\n"
    "</body>\n"
    "</html>\n"
    ;
    HTTP().set("http://boring.com", originalPageText);
    string plan =
    "Lefty and Mugsy enter the bank by the back door.\n"
    "Shorty waits out front in the getaway car.\n"
    "Don't let the teller trip the silent alarm.\n"
    ;
    string newPageText;
    if ( ! WebSteg::hideMessageInPage("http://boring.com", plan, newPageText))
        {
        cout << "Error hiding!" << endl;
        return 1;
        }
    HTTP().set("http://boring.com", newPageText);
    string msg;
    if ( ! WebSteg::revealMessageInPage("http://boring.com", msg))
        {
        cout << "Error revealing!" << endl;
        return 1;
        }
    if (msg != plan)
        {
        cout << "Message not recovered properly:\n" << msg;
        return 1;
        }
    cout << "Recovered text is what was hidden:\n" << msg;
}


/*
#include "provided.h"
#include "HashTable.h"
#include "http.h"
#include <iostream>
using namespace std;

int main()
{
	cout << "Test driver not yet written." << endl;

	 string text;
	 if ( ! WebSteg::hideMessageInPage("http://cs.ucla.edu", "Hello there!", text))
		cout << "Error hiding!" << endl;
     else{
         cout << "HIDE WORKS !" << endl;
     }
    string whats;
    if ( ! WebSteg::revealMessageInPage("http://cs.ucla.edu", whats))
    cout << "Error revealing!" << endl;
    else{
        cout << "REVEAL WORKS" << endl;
    }
    
    string msg = "This class is finally over!";
    string host;
    HTTP().set("http://a.com", "<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ");
    if(WebSteg::hideMessageInPage("http://a.com", msg, host) ) 
        {
        HTTP().set("http://a.com",host); // replace the original webpage by the new content
        string msg2;
        if(WebSteg::revealMessageInPage("http://a.com", msg2)) {
            if(msg == msg2)
                cout << "Successfully hide and reveal message in pages!!\n" << whats << endl;
            else {
                cout << "Hidden message and revealed message do not match!!" << endl;
                cout << "Hidden message:   " << msg << endl;
                cout << "Revealed message: " << msg2 << endl;
            }
        }
        else 
            cout << "Failed to reveal message in page!!" << endl;
        
        }
    else 
        cout << "Failed to hide message in page!!" << endl;
}

 


 
 #include "HashTable.h"
 #include <iostream>
 using namespace std;
 
 int main ()
 {
 HashTable<string, int> nameToAge(100, 200);
 nameToAge.set("Carey", 43);
 nameToAge.set("David", 97);
 nameToAge.set("Timothy", 43, true);
 nameToAge.set("Ivan", 28);
 nameToAge.set("Sally", 22);
 nameToAge.set("David", 55);
 nameToAge.touch("Carey");
 // let's discard the two least recently written items
 for (int k = 0; k < 2; k++)
 {
 string discardedName;
 int discardedAge;
 if (nameToAge.discard(discardedName,discardedAge))
 cout << "Discarded " << discardedName
 << " who was " << discardedAge
 << " years old.\n";
 else
 cout << "There are no items to discard!\n";
 }
 }
 

 // my own stuff for testing compressor
 int main()
 {
 string s = "AAAAAAAAAB"; vector<unsigned short> sup;
 Compressor::compress(s, sup);
 for(int i =0 ; i< sup.size(); i++)
 {
 cout << sup[i] << endl;
 }
 } 
 // my own stuff for testing decompressor 
 
 int main()
 {
 vector<unsigned short> v;
 v.push_back(65);v.push_back(65);v.push_back(256);v.push_back(65);v.push_back(65);v.push_back(65);
 v.push_back(256);v.push_back(66);v.push_back(257);
 string output;
 Compressor::decompress(v, output);
 cout << output;
 }

 
 int main()
 {
 string hostIn = " <html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ";
 string hostOut; 
 string msg = "AAAAAAAAAB";
 
 }

 

 int main()
 {
 string html = "<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ";
 string msg = "Whap"; string hostOut; string revealed_message;
 
 // comparing test string 
 cerr << "If just message is converted to binary then it contains" << endl ;
 // convert 
 Steg::hide(html, msg, hostOut);
 cerr << "hostOut is " << endl;
 for (int i = 0 ; i < hostOut.size(); i++)
 {
 if (hostOut[i] == ' ')
 cerr << "_";
 else if (hostOut[i] == '\t')
 cerr<< "-";
 else
 {
 cerr << hostOut[i] ; 
 }
 }
 
 Steg::reveal(hostOut, revealed_message);
 cerr<< "revealed message is " << revealed_message <<endl;
 // cerr << "msg is " << msg << endl;
 
 
 }
 
 
 int main()
 {
 string msg = "This class is finally over!";
 string host;
 HTTP().set("http://a.com", "<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ");
 if(WebSteg::hideMessageInPage("http://a.com", msg, host) ) 
 {
 HTTP().set("http://a.com",host); // replace the original webpage by the new content
 string msg2;
 if(WebSteg::revealMessageInPage("http://a.com", msg2)) {
 if(msg == msg2)
 cout << "Successfully hide and reveal message in pages!!\n" << msg << endl;
 else {
 cout << "Hidden message and revealed message do not match!!" << endl;
 cout << "Hidden message:   " << msg << endl;
 cout << "Revealed message: " << msg2 << endl;
 }
 }
 else 
 cout << "Failed to reveal message in page!!" << endl;
 
 }
 else 
 cout << "Failed to hide message in page!!" << endl;
 }

 
}
*/
