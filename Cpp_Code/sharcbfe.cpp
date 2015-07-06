/* 
 * File:   sharcbfe.cpp
 * Author: Vasundhara Sharma
 *
 * Created on 30 January, 2015, 4:10 PM
 * This program is customized for running on SHARCNET and generate bfe_possibility set for only one chosen bfe from the entire bfe_list
 * Idea is to utilize multiple cores on SHARCNET for parallel processing of bfe intersection attack
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

string IntToString(int a); // to convert int to string 
vector<vector<string>> parseData(vector<string> fn); // to parse multiple param files

int main (int argc, char *argv[])
{
    
    int target = atoi(argv[1]);
    //ofstream out("/cygdrive/c/University/workspace/TestSharc/out_test_"+IntToString(target)+".txt");
    ofstream out("out_test_"+IntToString(target)+".txt");
    cout.rdbuf(out.rdbuf()); 
 
    //ofstream write_poss("/cygdrive/c/University/workspace/TestSharc/bfe_possibility_"+IntToString(target)+".txt"); //creating file of bfe-name possibility set 
    //ofstream write_unprocessed("/cygdrive/c/University/workspace/TestSharc/unprocessed_bfe_"+IntToString(target)+".txt"); // create list of singletons for processing
    
    ofstream write_poss("bfe_possibility_"+IntToString(target)+".txt"); //creating file of bfe-name possibility set 
    ofstream write_unprocessed("unprocessed_bfe_"+IntToString(target)+".txt"); // create list of singletons for processing
    
    bfe b,c;
    vector<int> range_bigram {};
    vector<string> v {};
    vector<string> files {};
    vector<string> nameset {};
    vector<string> elems {};
    vector<string> crack {};
    vector<string> interim {};
    vector<string> fullname {};
    vector<vector<string>> final;
    vector<string> p_bfe;
    vector<vector<string>> p_list;

    int count;
    string item1,item2;
    string bfe_output;
    float dice_bfe,dice_plain;
    
    // ****************** reading bfe list into vector v ******************
    //ifstream iFile("/cygdrive/c/University/workspace/TestSharc/bfe_list.txt");
    ifstream iFile("bfe_list.txt");
    if(!iFile.is_open())
        cerr << "Bad file input....... closing....";

    /* While there is still a line. */
    while(getline(iFile,item1)) {     
        /* removing trailing newline from the names */
        if ( item1.size() > 0 ) 
        item1.resize( item1.length()-1 );
        /* Vector v contains list of names. */ 
        v.push_back(item1);
    }
    
    iFile.close();

    // *************** reading all names into vector fullname **************
    //ifstream nFile("/cygdrive/c/University/workspace/TestSharc/name_list.txt");
    ifstream nFile("name_list.txt");
    if(!nFile.is_open())
        cerr << "Bad file input....... closing....";

    /* While there is still a line. */
    while(getline(nFile,item2)) {     
        /* removing trailing newline from the names */
        if ( item2.size() > 0 ) 
        item2.resize( item2.length()-1 );
        /* Vector v contains list of names. */ 
        fullname.push_back(item2);
    }
    
    nFile.close();
    
    /* ****** Read file containing Names and Dice's Coeff and store it in vector final ********** */
    
    //files.push_back("/cygdrive/c/University/workspace/TestSharc/dice_name.txt");
    files.push_back("dice_name.txt"); //if default directory path is set in IDE
    final = parseData(files);
    
    /*Transpose the vector Final for clear reading*/
    vector < vector <string> > outtrans(final[0].size(),vector<string>(final.size()));
    outtrans = b.transposeName(final);
    
    /* Start processing BFE and Dice's Coeff comparisons*/
    
      //replacing for loop with argv[1] for single line run of code
        
    
    // make sure that the Index is within bfe file size 
    if(target < 0 || target > v.size()){
        cout << "Trying to read bfe Out of Range.. Program shall terminate here !!" << endl;
        write_poss.close();
        write_unprocessed.close();
        cerr << "Trying to read bfe Out of Range.. Program shall terminate here !!" << endl;
        exit(0);
    }
        string one;
        one = v.at(target); // capturing name from bfe_list at position int(target)
        b.set(one); //setting bfe 
        bfe_output = b.printBF();
        
        interim = fullname; // assign vector v to interim for initial processing
        cout << "============================================" << endl;
        cout << "Full Name List Size:" << interim.size() << endl;
        cout << "Attempt to decode Plain Name:" << one << endl;
    
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
            
       }  // inner "for" loop ends 
       
       // putting brackets to take care for goto statement skip over
       {
       cout << "Printing the intersection output for:" << one << endl;
       cout << "End list Size:" << crack.size() << endl;
        
        write_poss << bfe_output << ":";
        vector<string>::const_iterator out_res_final; 
        for( out_res_final = crack.begin(); out_res_final != crack.end(); ++out_res_final ) {
            cout << *out_res_final << ", ";
            write_poss << *out_res_final << ",";
        }
        cout << endl; 
        write_poss << "|" << endl;  
        cout << "============================================" << endl;  
        goto done;
       }
            
       found:
       cout << "Found Singleton for:" << one << endl;
       write_unprocessed << bfe_output << ":";
       write_unprocessed << crack[0] << "|" << endl;  
       cout << "============================================" << endl;  

   //} // for outer for loop
         
    // To see if Results are obtained as desired 
         
        // fetch bfe in string format and populate a vector with all the entries
	//bfe_output = b.printBF();
       /* vector<string>::const_iterator out_res_final;
             for( out_res_final = crack.begin(); out_res_final != crack.end(); ++out_res_final ) {
               cout << *out_res_final;
             }
            cout << endl;  */
   
   // close the file handles before exiting
   done:    
   write_poss.close();
   write_unprocessed.close();
   
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
    string line,dice_val,names,params;
    vector<string> list1 {};
    vector<string> list2 {};
    vector<string> list3 {};
    vector < vector <string> > temp,final; 
    
/* Read file and store different parameters it in a vector */
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

    pFile.close();
    }
    final.insert(final.end(), temp.begin(), temp.end());
    
    return final;
}