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

int main (int argc, char *argv[])
{
 
    bfe b,c;
    vector<string> v {};
    vector<string> list1 {};
    vector<string> list2 {};
    vector<string> nameset {};
    vector<string> elems {};
    vector<string> crack {};
    vector<string> enterim {};
    vector < vector <string> > final; 
    string inter;
    int count;
    string line,item,dice_val,names;
    string bfe_output;
    float dice_bfe,dice_plain;
    
    ifstream iFile("name_list.txt");
    if(!iFile.is_open())
        cerr << "Bad file input....... closing....";

    /* While there is still a line. */
    while(iFile >> item) {     
    /* Vector v contains list of names. */ 
        v.push_back(item);
    }
    
    iFile.close();
    
    /* Read file containing Names and Dice's Coeff and store it in a vector */
    ifstream pFile("dice_coeff.txt");
    while(getline(pFile,line)) {
        
        stringstream iss(line);
             getline(iss, dice_val, ':');
             list1.push_back(dice_val);

             getline(iss, names, '-');
             list2.push_back(names);
    }
    final.push_back(list1);
    final.push_back(list2);
    /* Vector Final contains Dice's Coeff in row 0 and Names in row [1]*/
    pFile.close();
   
    
    /*Transpose the vector Final for clear reading*/
    
    vector < vector <string> > outtrans(final[0].size(),vector<string>(final.size()));
    outtrans = b.transposeName(final);
    
    /* Start processing BFE and Dice's Coeff comparisons*/
    
    vector<string>::const_iterator one;
    for( one = v.begin(); one != v.end(); ++one ) {
        b.set(*one);    
        enterim = v; // assign vector v to enterim for initial processing
        
        vector<string>::const_iterator rest;
        for( rest = v.begin(); rest != v.end(); ++rest ) {
            c.set(*rest); 
            // try reading other items in loop
            dice_bfe = c.Dice(b);
            nameset.clear();
            // Get list of plain names for a set range of dice_bfe and dice_plain match
            nameset = c.rangeNames(dice_bfe,0.05,outtrans);
           
           // perform recursive set Intersection over the list of names found 
            vector<string> buffer,result;
            result.clear();
            //result = {"ANNA","JOHN","KEITH","MARII","TAYLOR"};
            for(unsigned j=0; j < nameset.size(); ++j){
                
                elems.clear();
                buffer.clear();
                elems = c.splitName(nameset[j],' ');
                 
                //elems = {"ANNA","BARCELON","KEITH","ORGAR","ANNA"};
                //cout << "Elements:" << elems.size() << endl;
                
                buffer = c.setUnion(result,elems);
                swap(result,buffer);
                cout << "Result:" << result.size() << endl;
                
           }
            cout << "Enterim:" << enterim.size() << endl;
            crack = c.setIntersection(result,enterim);
            cout << "Crack:" << crack.size() << endl;
            vector<string>::const_iterator out_res_final;
             for( out_res_final = crack.begin(); out_res_final != crack.end(); ++out_res_final ) {
               cout << "Here we go:" << *out_res_final << "--";
             }
            cout << endl;
            if (crack.size() == v.size() - 1) break;
            enterim = crack;
            
       }
       exit(0); 
       /*vector<string>::const_iterator out_res_final;
             for( out_res_final = crack.begin(); out_res_final != crack.end(); ++out_res_final ) {
               cout << *out_res_final << " ";
             }
            cout << endl; */ 
   }
         
    // To see if Results are obtained as desired 
         
        // fetch bfe in string format and populate a vector with all the entries
	//bfe_output = b.printBF();
       /* vector<string>::const_iterator out_res_final;
             for( out_res_final = crack.begin(); out_res_final != crack.end(); ++out_res_final ) {
               cout << *out_res_final;
             }
            cout << endl;  */

    return 0;
}

