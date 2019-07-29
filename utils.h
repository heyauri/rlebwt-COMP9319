//
// Created by rui on 2019-07-28.
//

#ifndef ASS2CPP_PUBLIC_H
#define ASS2CPP_PUBLIC_H

using namespace std;


#define MAXSIZE 10240
#define BITMAX MAXSIZE*8

void printBitArray(std::vector<bool> &arr);
void writeBitToBitArray(vector<bool> &arr, int l);
void printSelectBitArray(vector<bool> &select_b);
void printRankBMap(map<unsigned int,unsigned int> &rank_b);

#endif //ASS2CPP_PUBLIC_H
