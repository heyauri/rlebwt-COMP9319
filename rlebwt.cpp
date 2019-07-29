#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <map>
#include <bitset>
#include "sys/types.h"
#include "sys/stat.h"

#include "utils.h"

using namespace std;
// the max size of buffers
int i, j, k;


std::string bbFN, bFN, sFN;
char s_buffer[MAXSIZE];
char b_buffer[MAXSIZE];
char bb_buffer[MAXSIZE];
char fs[MAXSIZE];

//for s
unsigned int cs_table[256], lens_table[256];
vector<unsigned int> rank_s;
int s_section_count = 0;

//for b
vector<unsigned int> rank_b, select_b;
int rank_b_section_count = 0, select_b_section_count = 0, char_b_count = 0;
int suffix_b1_start = 0;

FILE *sp, *bp, *bbp;
int flag = 0, count_of_char = 0, last = 0;
int baseline = 0, length_prev = 0, length_next = 0, status = 0;
int fst, lst;
unsigned int count_of_s = 0;

void init() {
	for (i = 0; i < 256; i++) {
		cs_table[i] = 0;
		lens_table[i] = 0;
		rank_s.push_back(0);
	}
	rank_b.push_back(0);
	select_b.push_back(0);
}

void newSectionOfRankS() {
	for (i = 0; i < 256; i++) {
		rank_s.push_back(0);
	}
}


void getBitArray(vector<bool> &arr, char *str) {
	for (i = 0; i < strlen(str); i++) {
		for (j = 7; j > -1; j--) {
			arr.push_back(str[i] & (1 << j));
		}
	}
}

void initBB() {
	bbp = fopen(bbFN.c_str(), "w");
	for (i = 0; i < rank_b_section_count; i++) {
		char tmp[SECTIONSIZE / 8];
		for (j = 0; j < SECTIONSIZE; j += 8) {
			tmp[j / 8] = 0;
		}
		fwrite(tmp, SECTIONSIZE / 8, 1, bbp);
	}
	if (char_b_count > (SECTIONSIZE * rank_b_section_count / 8)) {
		unsigned int left_count = (unsigned int)char_b_count % (SECTIONSIZE/8);
		char tmp[left_count];
		for (j = 0; j < left_count; j ++) {
			tmp[j] = 0;
		}
		fwrite(tmp, left_count, 1, bbp);
	}
}

void generateBB() {
	initBB();



}

void readSB(string &fileName) {
	bbFN = fileName + ".bb";
	sFN = fileName + ".s";
	bFN = fileName + ".b";
	sp = fopen(sFN.c_str(), "r");
	bp = fopen(bFN.c_str(), "r");
	bbp = fopen(bbFN.c_str(), "r");
	if (!sp || !bp) {
		cout << fileName + ".s/.b not exists!" << endl;
	}
	//c_table rank_s
	while (!feof(sp)) {
		fgets(s_buffer, MAXSIZE, sp);
		for (i = 0; i < strlen(s_buffer); i++) {
			lens_table[(int) s_buffer[i]]++;
			if ((i + 1) % SECTIONSIZE == 0) {
				for (j = 0; j < 256; j++) {
					rank_s.push_back(lens_table[j]);
				}
				s_section_count++;
			}
		}
		unsigned prev_chars_count=0;
		count_of_s += strlen(s_buffer);
	}
	//rank_b select_b
	while (!feof(bp)) {
		fgets(b_buffer, MAXSIZE, bp);
		vector<bool> b_arr;
		getBitArray(b_arr, b_buffer);
		char_b_count += strlen(b_buffer);
		//b_count : how many 1 in b
		unsigned int b_count = rank_b[s_section_count];
		for (i = 0; i < b_arr.size(); i++) {
			if (b_arr[i]) {
				b_count++;
				if (b_count % SECTIONSIZE == 0 && b_count > 0) {
					select_b.push_back((unsigned int) select_b_section_count * SECTIONSIZE + i % SECTIONSIZE);
				}
				if (b_count + 1 == count_of_s) {
					suffix_b1_start = rank_b_section_count * SECTIONSIZE + i % SECTIONSIZE;
				}
			}
			if ((i + 1) % SECTIONSIZE == 0) {
				rank_b.push_back(b_count);
				rank_b_section_count++;
			}
		}
	}
	if (!bbp) {
		generateBB();
	}
	fclose(bbp);
}


void searchForTimes(string target) {

}


int main(int argc, char *argv[]) {
	if (argc < 4) {

	}

	std::string fileName = argv[2];
	std::string mode = argv[1];
	std::string target = argv[4];
	init();
	readSB(fileName);

	//after read , Assume that all S B and BB is good.
	if (mode == "-m") {
		searchForTimes(target);
	}


	if (access(argv[3], 00) < 0) {
		mkdir(argv[3], 0777);
	}


	return 0;
}