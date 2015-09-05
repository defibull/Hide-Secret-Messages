#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
    string page; 
    if (!HTTP().get(url,page))
        return false;
    else{
        HTTP().get(url, page);
        if (Steg::hide(page, msg, host))
            { cerr << "Web steg is beautiful" <<endl;//HTTP().set(url, host);
                return true;}
    }
    return false;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
    string page; 
    if (!HTTP().get(url,page))
        return false;
    else{
        HTTP().get(url, page);
        if (Steg::reveal(page, msg))
            return true;
            }
    
    return false;
}


