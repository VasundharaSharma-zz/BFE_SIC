/* 
 * File:   automate_trimbfe.cpp
 * Author: vasundhara
 * Tasks: This tool performs bfe-attack part-2 (Recursive Trim Step)
 * Input File Requirements: processed_bfe.txt, unprocessed_bfe.txt, bfe_possibility.txt, bfe_name_cheatsheet.txt
 * Output Files: 1) trimmed_output.txt [prints the runtime console logs] 2) trim_count.txt files which track DoA
 * Created on 8 March, 2015, 7:05 PM
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <bitset>
#include <sstream>
#include <vector>
#include <algorithm>
#include "sha1.h"
#include "bfe.h"

using namespace std;

string IntToString(int a); // to convert int to string 
vector<vector<string>> parseFile(string file);
int bfe_trim_list(int var);
void renameFile(int var);


int main (int argc, char *argv[])
{
    string line1;
    int count=0;
    int newcount;
    vector<vector<string>> check_possibility,check_unproc;
    
    // redirect console output to file trimmed_output.txt
    ofstream out("trimmed_output.txt", ios::out | ios::trunc);
    cout.rdbuf(out.rdbuf());
    
    // clear processed_bfe file before test starts
    ofstream final_processed("processed_bfe.txt", ios::out | ios::trunc);
    final_processed.close();
    
    ifstream iFile("bfe_name_cheatsheet.txt");
    if(!iFile.is_open())
        cerr << "Bad file input....... closing....";

    /* While there is still a line. */
    while(getline(iFile,line1)) {     
        
        // Check to see if bfe_possibility file is not empty, else exit program
        check_possibility = parseFile("bfe_possibility.txt");
        if(check_possibility.size() == 0) {
           cout << "No more Possibility Sets available for Processing.. Program Terminating" << endl;
           exit(0);
        }
        
        // Check to see if unprocessed bfe list already has some data
        check_unproc = parseFile("unprocessed_bfe.txt");
        if(check_unproc.size() > 0) {
            cout << "We aready have some bfe-name pairs in unprocessed list for running trim step... proceeding to trimming" << endl;
            count++; // track run
        } else {
            count++; // track run
            // induce bfe-name pair in unprocessed bfe list every time
            ofstream update_poss("unprocessed_bfe.txt", ios::out | ios::trunc);
            update_poss << line1 << endl;
            update_poss.close(); 
        }
    //=============================================================================================
    
     // call trim bfe possibility list
     newcount = bfe_trim_list(count);
     count = newcount;    
    //==============================================================================================
    
     cout << "Looping within the cheatsheet file..." << endl;       
    
    } // ending cheatsheet loop here
    
    iFile.close();
    
    return 0;
}

// function to rename all trim_count files after ech iteration
void renameFile(int count)
{
    char src_file[] = "/cygdrive/c/University/workspace/NewBloomFilter/NewExperiment/trim_count.txt";
    string dest = "/cygdrive/c/University/workspace/NewBloomFilter/NewExperiment/trim_count_"+IntToString(count)+".txt";
    char* dest_file = new char[dest.length() + 1];
    strcpy(dest_file, dest.c_str());        
        
    if (rename(src_file, dest_file) != 0) {
    cout << "Failed to rename File:" << src_file << endl;
    }  
    delete[] dest_file; // call destructor for dest_file
}



string IntToString(int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

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


int bfe_trim_list(int curcount){

    bfe r; 
    int singleton_flag; // to track count of new singletons
    int trim_list_count = curcount;
    vector<vector<string>> unproc_final, poss_final;
    vector<string> p_names,u_bigrams,p_bigrams,name_intersect;
    float dice_bfe,dice_plain;
    int bfe_intersect_count;
    
    // Read file containing unprocessed name-bfe pairs 
   START:
   ofstream trim_count("trim_count.txt", ios::out | ios::trunc); // resetting trim_count before each iteration
   trim_count.close();
    
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
       ofstream final_proc("processed_bfe.txt", ios::app);
       final_proc << unproc_final[i][0] <<":" <<unproc_final[i][1] << "|" <<endl;
       final_proc.close();
       
       // convert this to function after unit testing beyond this point 
       poss_final = parseFile("bfe_possibility.txt");
       cout << "Initial Size of Possibility set:" << poss_final.size() << endl;
       
       if(poss_final.size() == 0) {
          cout << "No more Possibility Sets available for Processing.. Program Terminating" << endl;
          exit(0);
      } else {
          // check possibility set and remove the bfe entry corresponding to current unprocessed_bfe list 
          for(int j=0; j<poss_final.size();j++){
              if(unproc_final[i][0] == poss_final[j][0]){
                  cout << "Erasing line:" << j << "from possibility set" << endl;
                  cout << "Line Details are:" << poss_final[j][0]<< ":" << poss_final[j][1] << endl;
                  poss_final.erase(poss_final.begin()+j);
                  
              }
          } 
      }
       // clear existing bfe_possibility file for storing fresh trimmed set
       ofstream update_poss("bfe_possibility.txt", ios::out | ios::trunc);
       update_poss.close();
       
       cout << "Revised Size of Possibility set:" << poss_final.size() << endl;
       
       
       // iterate through possibility set list to trim off non-matching names 
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
                //cout << "we need to remove this name:" <<p_names[k] <<endl;
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
                 //cout << "we need to remove this name:" <<p_names[k] <<endl;
                 p_names.erase(p_names.begin()+k);
                 }
             }
          }
           
           // Case II. Trimming condition 2: based on first bigram differences. 
           for(int k=0; k<p_names.size();k++){
            p_bigrams = r.parseGrams(p_names[k]); 
            if(bfe_intersect_count < hashfunctions) {
            if(p_bigrams[0] == u_bigrams[0]){    
                 //cout << "we need to remove this name:" <<p_names[k] <<endl;
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
                 //cout << "we need to remove this name:" <<p_names[k] <<endl;
                 p_names.erase(p_names.begin()+k); 
                 }
              }  
           }
           // TRIMMING POSSIBILITY SET: Conditions End
           
           // UPDATE POSSIBILTY SET AFTER EACH ROUND 
           ofstream update_poss("bfe_possibility.txt", ios::app);
           
           update_poss << poss_final[j][0] <<":";
           for(string test: p_names) {
                 update_poss << test << ",";
                  }
           update_poss << "|" << endl;
           update_poss.close();
           
           // create new file for recording trimmed possibility name count
           ofstream trim_count("/cygdrive/c/University/workspace/NewBloomFilter/NewExperiment/trim_count.txt", ios::app);
           //ofstream trim_count("trim_count.txt", ios::app);
           trim_count << "End list Size:" << p_names.size() << endl; // for recording the size of trimmed possibility set
           trim_count.close();
           
           // printing the updated possibility set 
       }
       
       // iterate through updated possibility set to discover new singletons
       
       // IF NEW SINGLETONS FOUND
       // Case: add new singletons to unprocessed BFE list
       poss_final = parseFile("bfe_possibility.txt");
       singleton_flag = 0;      
       for(int j=0; j<poss_final.size();j++){
       p_names = r.splitName(poss_final[j][1],',');    
        if(p_names.size() == 1) {
           singleton_flag = 1; 
           cout << "entering new singleton loop"<<endl;
           cout << "adding new cracked name to unprocessed list:"<< endl;
           cout << poss_final[j][0] <<":" << p_names[0]<<endl;
           ofstream update_unproc("unprocessed_bfe.txt", ios::out | ios::app);
           update_unproc <<poss_final[j][0] <<":" << p_names[0]<<"|" << endl;
           update_unproc.close();
           poss_final.erase(poss_final.begin()+j); // need to update bfe_possibility.txt
           continue; // end the current iteration
         }
       }
       
       // rewrite bfe_possibility.txt file to accommodate removed singletons (if any)
       
       //reset the current state of bfe_possibility.txt for overwriting
       ofstream clear_poss("bfe_possibility.txt", ios::out | ios::trunc);
       clear_poss.close();
       
       for(int j=0; j<poss_final.size();j++){
        ofstream new_update_poss("bfe_possibility.txt", ios::out | ios::app);
        new_update_poss <<poss_final[j][0] <<":" << poss_final[j][1]<<"|" << endl;
        new_update_poss.close();    
        }  
       // rename trim_count files before looping
       renameFile(trim_list_count);
       if(singleton_flag == 1){ 
           trim_list_count++;
           goto START; // loop over new singletons
       } 
   }
   //exit(0); // for debugging 
   // copyback correct count before returning 
   curcount = trim_list_count;
   return curcount;
}