#ifndef BFE_H
#define BFE_H

#include <string>
#include <bitset>
#include <vector>
#include "sha1.h"

using namespace std;

const int bfelength = 500;
const int hashfunctions = 2;
const int overlaps = 5;

class bfe
{ 

public:

	bfe();
	~bfe();

	
	void set(string item);
	int equals(string item);

	void insertGram(string gram);
	int containsGram(string gram, int track);

	// Returns the number of bits set in bf_
	int count();
	float Jaccard(bfe bf);
        float Dice(bfe bf);
	string printBF();
	void setKey(string key);
        vector<string> filterNames(bfe x, float dice_bfe, vector<vector<string>> &outtrans);
        vector<string> createMatrix(bfe x, vector<string> &namelist1, vector<string> &namelist2);
        
	bitset<bfelength> getBF();
	// int setBF(bitset<bfelength> bf);

	static vector<string> setIntersection(vector<string> &v1, vector<string> &v2);
	static vector<string> setUnion(vector<string> &v1, vector<string> &v2);
	static vector<string> setDifference(vector<string> &v1, vector<string> &v2);
        static vector<string> setInsert(vector<string> &v1, string s);
	static vector<string> crossProduct(vector<string> &v1, vector<string> &v2);
        static vector<string> intersectPlain(string &s1, string &s2);
	static vector<string> parseGrams(string item);
        static vector<vector<string>> transposeName(vector<vector<string>> &final);
        static vector<string> rangeNames(float dice, float range, vector<vector<string>> &outtrans);
        static vector<string> setIntersectNames(vector<vector<string>> &outtrans, vector<string> &nameset);
        static vector<string> splitName(string &s, char delim);
        static vector<string> createNamelist(vector<int> rangebigram, vector<vector<string>> &outtrans);
        static vector<vector<string>> createBigramDB();
        vector<int> bigramCount();
        
private:

	bitset<bfelength> bf_;
	string key_;
	const int n_ = 2;

	int itemToPos(string item, int hashindex);


};

#endif //BFE_H