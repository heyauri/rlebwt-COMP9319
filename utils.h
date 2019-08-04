//
// Created by rui on 2019-07-28.
//

#ifndef ASS2CPP_PUBLIC_H
#define ASS2CPP_PUBLIC_H

using namespace std;

//MAXSIZE should larger than SECTIONSIZE

#define MAXSIZE 8
#define BITMAX MAXSIZE*8
#define CHARSCALE 128

//section size should >= 8
#define SECTIONSIZE 8
#define BIT_SECTION_SIZE_OF_CHAR (SECTIONSIZE/8)

//in char i.e. 8 bits
//dont know why...this size can't be 102400 =_=...
#define BB_BUFFER_SIZE 204800

void printBitArray(std::vector<bool> &arr);
void writeBitToBitArray(vector<bool> &arr, int l);
void printSelectBitArray(vector<bool> &select_b);
void printRankBMap(map<unsigned int,unsigned int> &rank_b);

#endif //ASS2CPP_PUBLIC_H
