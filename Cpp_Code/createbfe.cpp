#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "sha1.h"
#include "bfe.h"

using namespace std;
string IntToString(int a);

int main (int argc, char *argv[])
{
 
    bfe b;
    vector<string> v {};

    string item;
    string bfe_output;
    
    ifstream iFile("bfe_list.txt");
    if(!iFile.is_open())
        cerr << "Bad file input....... closing....";

    /* While there is still a line. */
    while(getline(iFile,item)) {     
        /* removing trailing newline from the names */
        if ( item.size() > 0 ) 
        item.resize( item.length()-1 );
        /* Vector v contains list of names. */ 
        v.push_back(item);
    }
    
    iFile.close();

    /* Read each plain Names, check for overlaps and create corresponding bfe */
    
    ofstream create_bfe("bfe_name_cheatsheet.txt", ios::trunc);
    cout << "Full bfe List Size:" << v.size() << endl;
    
    vector<string>::const_iterator one;
    for( one = v.begin(); one != v.end(); ++one ) {
        
        b.set(*one);
        bfe_output = b.printBF();
        
        create_bfe << bfe_output << ":";
        create_bfe << *one << "|" << endl;

       //string encoded = base64_encode(reinterpret_cast<const unsigned char*>(bfe_output.c_str()), bfe_output.length());
        //create_bfe << encoded << endl;
   } 
   
  //create_bfe << "Successfully Created BFEs for the given Name List";
  create_bfe.close(); 
  cout << "Successfully Created BFEs for the given Name List";
  return 0;

}


string IntToString(int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}
