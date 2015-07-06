/* 
 * File:   bfe_trim_list.cpp
 * Author: Vasundhara Sharma
 *
 * Created on 09 October, 2014, 11:52 PM
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <bitset>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include "sha1.h"
#include "bfe.h"

using namespace std;

string IntToString(int a);
vector<vector<string>> parseFile(string file);

int main (int argc, char *argv[])
{
 
    bfe r;
    ofstream out("trimmed_output.txt", ios::out | ios::trunc);
    cout.rdbuf(out.rdbuf()); 
    
    ofstream trim_count("trim_count.txt", ios::out | ios::trunc);
    trim_count.close();
    
    vector<vector<string>> unproc_final, poss_final;
    vector<string> p_names,u_bigrams,p_bigrams,name_intersect;
    float dice_bfe,dice_plain;
    int bfe_intersect_count;
    
    // Read file containing unprocessed name-bfe pairs 
   
   unproc_final = parseFile("unprocessed_bfe.txt");
   cout << "Size of unprocessed bfe set:" << unproc_final.size() << endl;
   if(unproc_final.size() == 0) {
       cout << "No more Singletons available for Processing.. Program Terminating" << endl;
       exit(0);
   }
   //unproc_final.erase( unique( unproc_final.begin(), unproc_final.end() ), unproc_final.end() );
   
   //emptying unprocessed list after each new unique entry is processed
   ofstream reset_unproc("unprocessed_bfe.txt", ios::out | ios::trunc);
   reset_unproc.close();
   
   // processing each element from unprocessed bfe list
   for(int i=0; i<unproc_final.size();i++){
       cout << "LOOP:"<<i+1<<endl;
       
       // place element from unprocessed to processed list for tracking purposes
       ofstream final_poss("processed_bfe.txt", ios::app);
       final_poss << unproc_final[i][0] <<":" <<unproc_final[i][1] << "|" <<endl;
       final_poss.close();
       
       // convert this to function after unit testing beyond this point 
       poss_final = parseFile("bfe_possibility.txt");
       if(poss_final.size() == 0) {
       cout << "No more Possibility Sets available for Processing.. Program Terminating" << endl;
       exit(0);
      }
       // clear existing bfe_possibility file for storing fresh trimmed set
       ofstream update_poss("bfe_possibility.txt", ios::out | ios::trunc);
       update_poss.close();
       
       
       // iterate through possibility set list
       for(int j=0; j<poss_final.size();j++){
           
           // for debugging purposes 
           //cout << "Poss Set Size:" << poss_final.size() << "Loop:" << j+1 << endl;
           
           bitset<bfelength> a (unproc_final[i][0]);
           bitset<bfelength> b (poss_final[j][0]);
           bfe_intersect_count = (a&b).count();
           u_bigrams = r.parseGrams(unproc_final[i][1]);
           p_names = r.splitName(poss_final[j][1],','); 
           string u_last_elem = u_bigrams.back(); // extract last bigram for exposed singleton 
           
           // TRIMMING POSSIBILITY SET: Conditions Begin
           
           //cout << "process all possible names and trim non-matching entries";
            for(int k=0; k<p_names.size();k++){
             // remove the names from possibility list which is already in unprocessed name list
             if(unproc_final[i][1] == p_names[k]){
                cout << "we need to remove this name:" <<p_names[k] <<endl;
                p_names.erase(p_names.begin()+k);   
                // for debugging purposes //cout << "Next Name:" << p_names[k] << endl;
                }
            }
            
           // further processing based on the intersection mismatch
            // keep loops separate to avoid iterator invalidation upon erase
            // Case I: Trimming condition 1: based on bigram similarity in plain names
           for(int k=0; k<p_names.size();k++){
             p_bigrams = r.parseGrams(p_names[k]); 
             if(bfe_intersect_count < hashfunctions) { 
             //cout << "Non matching bfe:" << poss_final[j][0] << endl;
             name_intersect = r.setIntersection(u_bigrams,p_bigrams); 
             if(name_intersect.size() > 0) {
                 cout << "we need to remove this name:" <<p_names[k] <<endl;
                 p_names.erase(p_names.begin()+k);
                 }
             }
          }
           
           // Case II. Trimming condition 2: based on first bigram differences. 
           for(int k=0; k<p_names.size();k++){
            p_bigrams = r.parseGrams(p_names[k]); 
            if(bfe_intersect_count < hashfunctions) {
            if(p_bigrams[0] == u_bigrams[0]){    
                 cout << "we need to remove this name:" <<p_names[k] <<endl;
                 p_names.erase(p_names.begin()+k);
                 }
              }
           }
           
           // Case III. Trimming condition 2: based on last bigram differences.
           for(int k=0; k<p_names.size();k++){
             p_bigrams = r.parseGrams(p_names[k]); 
             if(bfe_intersect_count < hashfunctions) {
             string p_last_elem = p_bigrams.back(); // extract last bigram for possibility set names   
             if(p_last_elem == u_last_elem){
                 cout << "we need to remove this name:" <<p_names[k] <<endl;
                 p_names.erase(p_names.begin()+k); 
                 }
              }  
           }
           // TRIMMING POSSIBILITY SET: Conditions End
           
           // IF NEW SINGLETONS FOUND
           // Case: add new singletons to unprocessed BFE list
             if(p_names.size() == 1) {
                 cout << "entering new singleton loop"<<endl;
                 cout << "adding new cracked name to unprocessed list:"<< endl;
                 cout << poss_final[j][0] <<":" << p_names[0]<<endl;
                 ofstream update_unproc("unprocessed_bfe.txt", ios::out | ios::trunc);
                 update_unproc <<poss_final[j][0] <<":" << p_names[0]<<"|" << endl;
                 update_unproc.close();
                 
                 // create new file for recording trimmed possibility name count
                 ofstream trim_count("trim_count.txt", ios::app);
                 trim_count << "updated Size:" << p_names.size() << endl; // for recording the size of trimmed possibility set
                 trim_count.close();
                 
                 poss_final.erase(poss_final.begin()+j);
                 continue;
                 }
           
           // UPDATE POSSIBILTY SET AFTER EACH ROUND 
           ofstream update_poss("bfe_possibility.txt", ios::app);
           update_poss << poss_final[j][0] <<":";
           for(string test: p_names) {
                 update_poss << test << ",";
                  }
           update_poss << "|" << endl;
           update_poss.close();
           
           // create new file for recording trimmed possibility name count
           ofstream trim_count("trim_count.txt", ios::app);
           trim_count << "updated Size:" << p_names.size() << endl; // for recording the size of trimmed possibility set
           trim_count.close();
           
           // printing the updated possibility set 
       }
       
   }
    
    return 0;
}

/*template <typename T>
void remove(vector<T>& vec, size_t pos)
{
    vector<T>::iterator it = vec.begin();
    advance(it, pos);
    vec.erase(it);
} */

vector<vector<string>> parseFile(string file)
{
 
    bfe b;
    vector<string> set1, set2 {};
    vector<vector<string>> parse;
    string line;
    int count;
    string p_bfe, p_name;
    
    ifstream bFile(file);
    //check for the file to exist
    if(!bFile.is_open()){
        cerr << "File <"<<file<<"> not Found.......Terminating....";
        exit(0);
    }
    while(getline(bFile,line)) {
        
        stringstream iss(line);
             getline(iss, p_bfe, ':');
             set1.push_back(p_bfe);

             getline(iss, p_name, '|');
             set2.push_back(p_name);
    }
    parse.push_back(set1);
    parse.push_back(set2);
    // Vector Final contains Dice's Coeff in row 0 and Names in row [1] 
    bFile.close();
    // Transpose the vector Final for clear reading 
    vector<vector<string>> parse_res(parse[0].size(),vector<string>(parse.size()));
    parse_res = b.transposeName(parse);

    return parse_res;
}
