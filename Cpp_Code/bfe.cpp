
#include <stdio.h>
#include <math.h>
#include <string>
#include <bitset>
#include <iostream> 
#include <algorithm> 
#include <stdlib.h> 
#include <sstream>
#include <vector>
#include <set>
#include "sha1.h"
#include "bfe.h"


using namespace std;

bfe::bfe(){ key_ = "Td45shtyvC78sYhMe34dhYTejKt56dfwkShdfRn";}
bfe::~bfe(){}

int bfe::itemToPos(string item, int hashindex)
{

		unsigned long long addr;
		//string hash;
		// Preimage = i || "-" || item 
                stringstream ss2;
                ss2 << hashindex;
                string hash = ss2.str();
                
                //hash = to_string(hashindex);
		hash += "-";
		hash += key_;
		hash += "-";
		hash += item;
                
		// Compute hash	
		hash = sha1(hash);

		// Convert the first 64 bits of hash image to long long
		addr = strtoul(hash.substr(0, 16).c_str(), NULL, 16);

		// Mod result to a bit position in Bloom filter
		addr = addr % bfelength;
                //cout << "itemToPos returns:" << addr << endl;
                
	return addr;
}

// Parses a string into a vector of n-grams
vector<string> bfe::parseGrams(string item)
{
	vector<string> grams {};
	vector<string> gram {};
        std::set<string> uniqgram; // for creating unique n-grams
	string us = "_";
        
	if (item.size() == 0)
		return grams;

        int pos = item.find_first_of(' '); //sets cursor after firstname
        string rest = item.substr(pos+1);
        int nextpos = rest.find_first_of(' '); //sets cursor after lastname
        
        string firstname = item.substr(0, pos), lastname = item.substr(pos+1,nextpos), zipcode = rest.substr(nextpos+1);
        
        //cout << "FirstName:"<<firstname<<"Last Name:"<<lastname<<"Zip:"<<zipcode<<"-----"<<endl;
        
	//Insert front and back bigrams..
	firstname = us+firstname;
        string llastname = us+lastname+us; // lastname with _ attached in front and back for creating bigram
        
	if (firstname.size() == 1 && llastname.size() == 1){
		return grams;
	}
        item = firstname+llastname;
        
        // This code currently only handles bigrams (i.e., n_= 2)
	int i;
	for (i=0; i<item.length()-1; i++)
		grams = bfe::setInsert(grams, item.substr(i, 2));
        
        // Case: Include single bigram for zipcode/phone number if InputFile has it
        if(lastname.compare(zipcode) !=0) { // add zipcode bigram if string(lastname) and string(zipcode) are not same 
          grams.push_back(zipcode);
        }
        
        // remove duplicate n-grams from each name
        
        unsigned size_grams = grams.size();
        cout << "Size of grams:" << size_grams;
        for (unsigned int i =0; i< size_grams; ++i) uniqgram.insert(grams[i]);
        grams.assign(uniqgram.begin(),uniqgram.end()); // assgin the sorted n-grams back to vector grams
        
        /*to print_check (for debugging purposes) correct bigram generation
        for (string gram : grams)
	cout << gram << " "; */
        
	return grams;
}


// Inserts a string into the Bloom filter by decomposing it into n-grams and inserting each gram separately
void bfe::set(string item)
{
	vector<string> grams;
        int track = 0; // for tracking bigram overlap count in bfe
        
	// Reset bf_
	bf_.reset();

	// Get n-grams
	grams = parseGrams(item);

	// Insert n-grams after checking bigram overlaps
	for(string gram : grams){
                track = bfe::containsGram(gram,track);
		bfe::insertGram(gram);
        }
}        


// Tests if a string into the Bloom filter by decomposing it into n-grams and testing each gram separately
int bfe::equals(string item)
{
	vector<string> grams;
        int eq = 0;

	grams = parseGrams(item);

	for (string gram : grams)
		if (!bfe::containsGram(gram,eq))
			return 0;

	return 1;
}


// Test if a particular position in bloom filter is already set
int bfe::containsGram(string gram, int track)
{
	int i;
        
	for (i=0; i<hashfunctions; i++)
	{
           cout << "Current Position BF CHECK:"<< bfe::itemToPos(gram, i) << endl; 
           // Test overlapping bit positions before inserting new grams into bfe
           if(bf_.test(bfe::itemToPos(gram, i))) { 
           cout << "overlap at position:" << bfe::itemToPos(gram, i) << "------" << endl;
           track++;
           }
        }
        cout << "total overlaps:" << track<< endl;
        if(track > overlaps) {// condition to control permitted overlaps
           cout << "We have reached the maximum permitted overlaps allowed in the bfe..." << endl;
           cout << "resize the parameters and re-run the experiment !" << endl;
           exit(0);
        }
        
	return track;
}

// Inserts a single n-gram into the Bloom filter
void bfe::insertGram(string gram)
{
	int i;
        
        cout << "Debug Gram:" << gram << "-------" << endl;
	for (i=0; i<hashfunctions; i++)
	{
                // Set bit corresponding to h_i(item)
              cout << "Current Position BF set:"<< bfe::itemToPos(gram, i) << endl;
              bf_.set(bfe::itemToPos(gram, i));
              cout << "Current Position AF set:"<< bfe::itemToPos(gram, i) << endl;
        }
}


// calculate Jaccard Distance between two BFEs
float bfe::Jaccard(bfe x)
{

	int a,b;

	// count ( this.bf_ \INTERSECTION x.bf_ ) 
	a = (x.getBF()&bf_).count();

	// count ( this.bf_ \UNION x.bf_ ) 
	b = (x.getBF()|bf_).count();

	if (b==0)
		return 0;

	return (float)(a) / (float)(b);

}

// calculate Dice's Coefficient between two BFEs
float bfe::Dice(bfe x)
{

	int a,b;

	// count ( this.bf_ \INTERSECTION x.bf_ ) 
	a = (x.getBF()&bf_).count();
        
	// count ( this.bf_+ x.bf_ ) 
	b = (x.getBF()).count() + bf_.count();
        if (b==0)
		return 0;

	return (float)(2*a) / (float)(b);

}


// Inserts a string s into set v1
vector<string> bfe::setInsert(vector<string> &v1, string s)
{
	vector<string> v2;

	v2.push_back(s);
	// Using insert function for creating list of bigrams in order (in place of sorting))
        v1.insert(v1.end(),v2.begin(),v2.end());
        
	return v1;
}

// Returns the set difference of v1 - v2
vector<string> bfe::setDifference(vector<string> &v1, vector<string> &v2)
{
    vector<string> v3;
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    
    set_difference(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));
    
    return v3;
}

// Returns the set intersection of v1 and v2
vector<string> bfe::setIntersection(vector<string> &v1, vector<string> &v2)
{
    vector<string> v3;

    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    
    set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));

    return v3;
}

// Returns the set union of v1 and v2
vector<string> bfe::setUnion(vector<string> &v1t, vector<string> &v2t)
{
    vector<string> v1(v1t);
    vector<string> v2(v2t);
    vector<string> v3;

    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    set_union(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));

    return v3;
}

// Returns cross product of v1 and v2
vector<string> bfe::crossProduct(vector<string> &v1, vector<string> &v2){

  vector<string>v3;
  for (string n : v1)
    for (string m : v2)
      v3.push_back(n+m);

  sort(v3.begin(), v3.end());

  return v3;

}

// transpose vector matrix
vector<vector<string>> bfe::transposeName(vector<vector<string>> &final){
    vector<vector<string>> outtrans(final[0].size(),vector<string>(final.size()));
    for (unsigned x = 0; x < final.size(); ++x){
        for (unsigned y = 0; y < final[0].size(); ++y){
            outtrans[y][x] = final[x][y];
             
        }
    }
    return outtrans;
}

// calculate the name range between given range difference between dice
vector<string> bfe::rangeNames(float dice_bfe, float range, vector<vector<string>> &outtrans){
    vector<string> v1;
    for (unsigned i = 0; i < outtrans.size(); ++i){
        float dice_plain;
                dice_plain = atof((outtrans[i][0]).c_str());
                float lower_bound, upper_bound;
                lower_bound = dice_bfe - range;
                //upper_bound = dice_bfe + range;
                upper_bound = dice_bfe;
                if(dice_plain > lower_bound && dice_plain <= upper_bound && dice_plain != 0) {    
                    cout << "Dice_plain found in given range <"<<range<<">:" << dice_plain << endl;
                    v1.push_back(outtrans[i][1]); 
                 }   
                }
    return v1; 
}

// Function to extract names which match certain conditions related to bfe-plain decoding
vector<string> bfe::filterNames(bfe x, float dice_bfe, vector<vector<string>> &outtrans){
    vector<string> v1;
    vector<string> params {};
    int a,b,c;
    int a_p,b_p,c_p;
    b = bf_.count();
    a = (x.getBF()).count();
    c = (x.getBF()&bf_).count(); 
    //cout << "BFE |A|/k = " << a/hashfunctions << endl;
    //cout << "BFE |B|/k = " << b/hashfunctions << endl;
    //cout << "BFE |A n B|/k =  " << c/hashfunctions << endl;
    for (unsigned i = 0; i < outtrans.size(); ++i){
        float dice_plain; 
                dice_plain = atof((outtrans[i][0]).c_str()); 
                params.clear(); 
                params = x.splitName(outtrans[i][2],'*');  

                a_p = atoi((params[1]).c_str()); 
                b_p = atoi((params[2]).c_str());
                c_p = atoi((params[3]).c_str());
                
                //if(dice_bfe > dice_plain && a_p >= ceil(a/hashfunctions) && b_p >= ceil(a/hashfunctions) && b_p >= ceil(b/hashfunctions) && c_p <= floor(c/hashfunctions)) { 
                  if(dice_bfe >= dice_plain && a_p >= floor(a/hashfunctions) && a_p <= floor((a+3)/hashfunctions) && b_p >= floor(b/hashfunctions) && b_p <= floor((b+3)/hashfunctions) && c_p <= floor(c/hashfunctions)) {    
                    //cout << "Name Pairs matching all conditions: " << outtrans[i][1] << "with Plain Dice:" << dice_plain << endl;
                    //cout << "Plain |A| = " << a_p << endl;
                    //cout << "Plain |B| = " << b_p << endl;
                    //cout << "Plain |A n B| =  " << c_p << endl;
                    v1.push_back(outtrans[i][1]); 
                 }  
                //else 
                //cout << outtrans[i][1] << "Current pairs didn't match" << endl;
                }
    return v1; 
}


// Function to convert a string of multiple names into string vector of individual names
vector<string> bfe::splitName(string &s, char delim){
    vector<string> v1;
    stringstream ss(s);
    string token;
    while(getline(ss,token,delim)) {
        v1.push_back(token);
    }
    return v1;
}

// Function to calculate the list of names based on size of bfe names (bigram count + overlap error)
vector<string> bfe::createNamelist(vector<int> rangebigram, vector<vector<string>> &outtrans){ 
    vector<string> split_v;
    vector<string> namelist;
    string name_val;
    int name_size;
    namelist.clear(); // to reset namelist afresh createNamelist
    for (int i=0; i<rangebigram.size();i++){
            for(int j=0; j<outtrans.size();j++){
              name_val = outtrans[j][1];
              name_size = atof((outtrans[j][0]).c_str());
              if(rangebigram[i] == name_size) {
                  split_v.clear();
                  split_v = splitName(name_val, '*');
              }
            }
            namelist.insert(namelist.end(), split_v.begin(), split_v.end() );
    }
    return namelist;
}

// function to calculate common bigrams in plain name pairs 
vector<string> bfe::intersectPlain(string &s1, string &s2){
    vector<string> v1;
    vector<string> grams1, grams2;

        grams1 = parseGrams(s1);
        grams2 = parseGrams(s2);
        v1 = setIntersection(grams1,grams2);
        
    return v1;
}

// Function to create matrix of possible name combinations for a given size Names
vector<string> bfe::createMatrix(bfe x, vector<string> &namelist1, vector<string> &namelist2){
    vector<char> v1;
    vector<string> params {}, names {};
    int c,c_p;
    char matrix[namelist1.size()][namelist2.size()];
    cout << "Namelist1:" << namelist1.size() << "Namelist2:" << namelist2.size() << endl;
    c = (x.getBF()&bf_).count();
    //cout << "BFE |A n B|/k =  " << c/hashfunctions << endl;
    for (unsigned i = 0; i < namelist1.size(); ++i ){
     for (unsigned j = 0; j < namelist2.size(); ++j) {
       
         names = intersectPlain(namelist1[i],namelist2[j]);
         c_p = names.size();
         if (c_p <= floor(c/hashfunctions)){
             matrix[i][j] = 'P';
         }   
         else matrix[i][j] = 'x';
         cout <<matrix[i][j]<<" ";
         }
     cout << endl;
       }
    
    return names;
}


// Function to create Bigram Database for all possible English alphabet bigrams
vector<vector<string>> bfe::createBigramDB()
{
    vector<string> bigram_list; // for storing unique bigrams
    vector<string> bigram_val;  // for storing bigram value
    vector<vector<string>> bigram_db; // represents complete bigram DB
    bitset<bfelength> bs;
    string bfeval;
    int  bigram_count[26][26];
    
    bs.set(); // set the initial value of bfe to all 1's
   
    // need to convert bitset to string to store it in string vector
    bfeval = bs.to_string<char,string::traits_type,string::allocator_type>();
   
    // assigning various possible name bigrams 
	const string ALPHABET[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","_"};
                
	// looping for storing each bigram with initial bfe value in bigram_db
		for (int i=0 ; i<=26 ; i++){
			for(int j=0; j<=26;j++){
                            string b_name = ALPHABET[i] + ALPHABET[j];
                            bigram_list.push_back(b_name); 
                            bigram_val.push_back(bfeval);
                            
                            //cout << b_name << endl; // for checking individual bigrams
			}
		}
        bigram_db.push_back(bigram_list);
        bigram_db.push_back(bigram_val);
        
        return bigram_db;
}



bitset<bfelength> bfe::getBF()
{
	return bf_;
}

void bfe::setKey(string key)
{
	key_ = key;
}

int bfe::count()
{
	return bf_.count();
}

/*void bfe::printBF()
{
	cout << bf_ << "\n";
}*/

string bfe::printBF()
{
string mystring =
    bf_.to_string<char,string::traits_type,string::allocator_type>();

  //cout << "BFE: " << mystring << '\n';
  return mystring;
}

/* Function to calculate the range of bigrams based on bigram count+error factor*/
vector<int> bfe::bigramCount()
{
    vector <int> v1;
    int x = bf_.count();
    int start = floor(x/hashfunctions);
    int end = floor((x+3)/hashfunctions);
    for (int i = start; i <= end; ++i)
        v1.push_back(i);
    return v1;
}