#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <map>

#include "utils.h"
int q,w,e;
map<unsigned int,unsigned int>::iterator iter;

using namespace std;

void printBitArray(vector<bool> &arr) {
	for (q = 0; q < arr.size(); q++)
	{
		cout << arr[q];
		if ((q + 1) % 8 == 0) {
			cout << " ";
		}
	}
	cout << endl;
}

void writeBitToBitArray(vector<bool> &arr, int l) {
	arr.push_back(true);
	while (l > 0) {
		arr.push_back(false);
		l--;
	}
}

void printSelectBitArray(vector<bool> &select_b){
	for(q=0;q<select_b.size();q++){
		cout<<q+1<<","<<select_b[q]<<endl;
	}
}
void printRankBMap(map<unsigned int,unsigned int> &rank_b){
	for(iter = rank_b.begin(); iter != rank_b.end(); iter++) {
		cout << iter->first+1 << " : " << iter->second << endl;
	}
}