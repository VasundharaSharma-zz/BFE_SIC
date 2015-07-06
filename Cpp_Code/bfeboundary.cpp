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
vector<vector<string>> parseData(vector<string> fn);

int main (int argc, char *argv[])
{
 
    ofstream out("out.txt");
    cout.rdbuf(out.rdbuf()); 
    
    bfe b,c;
    vector<int> range_bigram {};
    vector<string> v {};
    vector<string> files {};
    vector<string> nameset {};
    vector<string> elems {};
    vector<string> crack {};
    vector<string> interim {};
    vector < vector <string> > final;
    
    string inter;
    int count;
    string item;
    string bfe_output;
    float dice_bfe,dice_plain;
    
    ifstream iFile("name_list.txt");
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

    /* Read file containing Names and Dice's Coeff and store it in a vector */
    //files.push_back("/cygdrive/c/University/workspace/NewBloomFilter/dice_name.txt");
    files.push_back("dice_name.txt");
    final = parseData(files);
    
    /*Transpose the vector Final for clear reading*/
    vector < vector <string> > outtrans(final[0].size(),vector<string>(final.size()));
    outtrans = b.transposeName(final);
    
    /* Start processing BFE and Dice's Coeff comparisons*/
    
    vector<string>::const_iterator one;
    for( one = v.begin(); one != v.end(); ++one ) {
        b.set(*one);
        interim = v; // assign vector v to interim for initial processing
        cout << "============================================" << endl;
        cout << "Full Name List Size:" << interim.size() << endl;
        cout << "Attempt to decode Plain Name:" << *one << endl;
    
       // moving to the inner loop names  
       vector<string>::const_iterator rest;
        for( rest = v.begin(); rest != v.end(); ++rest ) {
            c.set(*rest); 
            //cout << "Inner Loop Name:" << *rest << endl;
            // try reading other items in loop
            dice_bfe = c.Dice(b);
            //cout << "BFE Dice:" << dice_bfe << endl;
            if ((dice_bfe == 0) || (dice_bfe == 1)) {
                //cout << "Ignoring Dice value 0 & 1....moving to next name" << endl;
                continue; 
            }
            nameset.clear(); 
            // Get list of plain names for a set range of dice_bfe and dice_plain match
            //cout << "Fetching list of names matching dice value..." << endl;
            nameset = c.filterNames(b,dice_bfe,outtrans); 
            //cout << "Total NameSets found for dice range:" << nameset.size() << endl;
            if (nameset.size() == 0) { 
                //cout << "No matching names found for the current dice value.. moving to next name" << endl;
                continue;
            }
           // perform recursive set Intersection over the list of names found 
            vector<string> buffer,result;
            result.clear();
            // Loop over all the name sets obtained within the given Dice value Range
            for(unsigned j=0; j < nameset.size(); ++j){
                
                elems.clear();
                buffer.clear(); 
                elems = c.splitName(nameset[j],','); 
                //cout << "Performing Union for the set of names found in loop:" << nameset[j] << endl;
                buffer = c.setUnion(result,elems);
                swap(result,buffer);
                //cout << "Total matching names found in Union:" << result.size()-1 << endl;
                
                /*vector<string>::const_iterator out_res;
                for( out_res = result.begin(); out_res != result.end(); ++out_res ) {
                cout << *out_res << " ";
                }
                cout << endl; 
                */
           }
            crack.clear();
            //cout << "Perform set intersection for each loop of dice values" << endl;
            crack = c.setIntersection(interim,result); 
            //cout << "Crack:" << crack.size() << endl;
            if (crack.size() == 1) goto found;
            if (crack.size() == 0) crack = interim;
            interim = crack; 
            //cout << "Moving on to next name...." << endl;
            
       }
        found:
        cout << "Printing the intersection output for:" << *one << endl;
        cout << "End list Size:" << crack.size() << endl;
        vector<string>::const_iterator out_res_final;
             for( out_res_final = crack.begin(); out_res_final != crack.end(); ++out_res_final ) {
               cout << *out_res_final << ", ";
             }
            cout << endl; 
            
        cout << "============================================" << endl;
        //exit(0);
            
            
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


string IntToString(int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}


vector<vector<string>> parseData(vector<string> fn)
{
    string line,item,dice_val,names,params;
    vector<string> list1 {};
    vector<string> list2 {};
    vector<string> list3 {};
    vector < vector <string> > temp,final; 
    
/* Read file containing Names and Dice's Coeff and store it in a vector */
    vector<string>::const_iterator f;
             for( f = fn.begin(); f != fn.end(); ++f ) {
               ifstream pFile(*f);
               while(getline(pFile,line)) {
        
                stringstream iss(line);
                getline(iss, dice_val, ':');
                list1.push_back(dice_val);

                getline(iss, names, '|');
                list2.push_back(names); 
             
                getline(iss, params, '#');
                list3.push_back(params);
                
               }
    temp.push_back(list1);
    temp.push_back(list2);
    temp.push_back(list3);
    /* Vector Final contains Dice's Coeff in row 0 and Names in row [1]*/
    pFile.close();
    }
    final.insert(final.end(), temp.begin(), temp.end());
    
    return final;
}