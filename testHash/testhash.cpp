#include<iostream>

using namespace std;
 
unsigned int appSieveHashString(string str){
    unsigned int seed = 131;
    unsigned int hash = 0;
    string::iterator it;
    for( it = str.begin(); it!=str.end(); ++it)
        hash = hash * seed + (*it);
    return (hash & 0x7FFFFFFF);
}



int main(int argc, char* argv[]){
    string  str = "Ljava/security/MessageDigest;digest,([B)[B";
    cout << appSieveHashString(str) << endl;
}
