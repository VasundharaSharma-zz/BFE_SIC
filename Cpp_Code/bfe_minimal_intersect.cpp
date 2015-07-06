/* 
 * File:   bfe_minimal_intersect.cpp
 * Author: Vasundhara Sharma
 *
 * Created on 21 September, 2014, 12:43 PM
 */

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

struct compare {
    bool operator()(const string& first, const string& second) {
        return first.size() > second.size();
    }
};



int main (int argc, char *argv[])
{
 
    ofstream out("out.txt");
    cout.rdbuf(out.rdbuf()); 
    
    bfe b,c;
    compare name_comp;
    vector<int> range_bigram1,range_bigram2 {};
    vector<string> v,rev_v {};
    vector<string> namelist1,namelist2 {};
    vector<string> mat {};
    vector<string> nameset {};
    vector<string> elems {};
    vector<string> crack {};
    vector<string> interim {};
    string line,item,dice_val,name_len,names,params;
    vector<string> list1,list2 {};
    vector<string> files {};
    vector < vector <string> > final,parselist;
    
    string inter;
    int count;
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
    
    sort(v.begin(), v.end(), name_comp);
    vector<string>::const_iterator sort_res;
             for( sort_res = v.begin(); sort_res != v.end(); ++sort_res ) {
               cout << *sort_res << " " << endl;
             }
    rev_v = v; 
    /* Parse the name database file into vector for pre-processing */
            ifstream pFile("bigram_size.txt");
               while(getline(pFile,line)) {
        
                stringstream iss(line);
                getline(iss, name_len, ':');
                list1.push_back(name_len);

                getline(iss, names, '|');
                list2.push_back(names);
                
               }
    final.push_back(list1);
    final.push_back(list2);

    /*Transpose the vector Final for clear reading*/
    
    vector < vector <string> > outtrans(final[0].size(),vector<string>(final.size()));
    outtrans = b.transposeName(final);
    //cout << "Testing outtrans:" << outtrans[0][1] << outtrans[1][1] << endl;
    /* Start processing BFE and Dice's Coeff comparisons*/
    
    vector<string>::const_iterator one;
    for( one = v.begin(); one != v.end(); ++one ) {
        b.set(*one);
        cout << "Attempt to decode Plain Name:" << *one << endl;
        
        // calculate the length of names as per bfe bigram count + error factor
        range_bigram1 = b.bigramCount();
        for(int i=0; i< range_bigram1.size(); ++i) {
            cout << range_bigram1[i] << endl;
        }
        // read the right names for creating the initial list based on size
        namelist1 = b.createNamelist(range_bigram1, outtrans);
        
        //--------------------------------------------------------------------------------------------
        // moving to the shortest names in the bfe list
        reverse(rev_v.begin(), rev_v.end()); // to start picking the smallest names
        vector<string>::const_iterator rest;
        for( rest = rev_v.begin(); rest != rev_v.end(); ++rest ) {
            c.set(*rest); 
            cout << "Inner Loop Name:" << *rest << endl;
            
            range_bigram2 = c.bigramCount();
            namelist2 = c.createNamelist(range_bigram2, outtrans);
            
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
            mat = c.createMatrix(b,namelist1,namelist2);
            
        }       
           
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
    vector < vector <string> > temp,parselist; 
    
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
    parselist.insert(parselist.end(), temp.begin(), temp.end());
    
    return parselist;
}