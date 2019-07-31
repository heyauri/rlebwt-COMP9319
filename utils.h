//
// Created by rui on 2019-07-28.
//

#ifndef ASS2CPP_PUBLIC_H
#define ASS2CPP_PUBLIC_H

using namespace std;

//MAXSIZE should larger than SECTIONSIZE

#define MAXSIZE 32
#define BITMAX MAXSIZE*8
#define CHARSCALE 128

#define SECTIONSIZE 8
#define BIT_SECTION_SIZE_OF_CHAR (SECTIONSIZE/8)

//in char i.e. 8 bits
#define BB_BUFFER_SIZE 1

void printBitArray(std::vector<bool> &arr);
void writeBitToBitArray(vector<bool> &arr, int l);
void printSelectBitArray(vector<bool> &select_b);
void printRankBMap(map<unsigned int,unsigned int> &rank_b);

#endif //ASS2CPP_PUBLIC_H
