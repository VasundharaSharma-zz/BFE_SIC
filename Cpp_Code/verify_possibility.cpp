/* 
 * File:   verify_possibility.cpp
 * Author: Vasundhara Sharma
 *
 * Created on 25 March, 2015, 3:33 PM
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
void verify_possibility_set(string file1, string file2);

/*
 * 
 */
int main(int argc, char** argv) {

    verify_possibility_set("bfe_name_cheatsheet.txt","bfe_possibility.txt");
    return 0;
}



// Function Definitions below:

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


// function to verify the integrity of bfe_possibility sets

void verify_possibility_set(string file1, string file2){
    
    bfe r;
    vector<vector<string>> bfe_c,bfe_p;
    vector<string> p_names,n_bigrams;
    int bfeCount, nameCount;
    
    int count = 0;
    int flag =0;
    
    bfe_c = parseFile(file1);
    bfe_p = parseFile(file2);
    
    
    cout << "Size of bfe_c" << bfe_c.size() << endl;
    for(int i=0; i<bfe_c.size();i++){
        flag = 0; // resetting flag before each loop
        for(int j=0; j<bfe_p.size();j++){
            if(bfe_c[i][0] == bfe_p[j][0])
            {
                count++; // tracks count of found values
                p_names = r.splitName(bfe_p[j][1],','); // extract all names for each bfe_possibility match
                for(int k=0; k<p_names.size();k++){
                    if(bfe_c[i][1] == p_names[k]){
                        flag = 1;
                        //cout << "Valid Name "<<bfe_c[i][1]<<" present in the possibility set"<<endl;
                    } 
                        
                }
                if (flag == 0) {
                    cout << "Valid name " <<bfe_c[i][1]<<" Missing"<<endl; 
                    cout << "Some debug Stats:"<<endl;
                    bitset<bfelength> a (bfe_c[i][0]);
                    bfeCount = a.count()/hashfunctions;
                    n_bigrams = r.parseGrams(bfe_c[i][1]);
                    cout<< "Plain Name Bigrams count: " <<n_bigrams.size() << " BFE bigrams count: " << bfeCount << endl;
                    cout << "------------------------------------\n";
                }    
            }
        }
        
    }
   cout << "Matching bfe's found for:" << count<< endl;
    
}