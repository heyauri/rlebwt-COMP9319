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
// vars for loop.
unsigned int i, j, k, a, s, d, q, w, e;


std::string bbFN, bFN, sFN;
char s_buffer[MAXSIZE];
char b_buffer[MAXSIZE];
char bb_buffer[BB_BUFFER_SIZE];

//for s
unsigned int cs_table[CHARSCALE], lens_table[CHARSCALE];
vector<unsigned int> rank_s;
unsigned int s_section_count = 0, current_s_buffer_section = 0;
unsigned int current_s_buffer_size = 0;

//for b
vector<unsigned int> rank_b, select_b;
unsigned int rank_b_section_count = 0, select_b_section_count = 0, char_b_count = 0;
unsigned int suffix_b1_start = 0;
unsigned int current_b_buffer_size = 0, current_b_buffer_section = 0;

//for bb
unsigned int current_bb_buffer_size=0,current_bb_buffer_section=0;


FILE *sp, *bp, *bbp;
unsigned int count_of_s = 0;

void init() {
	for (i = 0; i < CHARSCALE; i++) {
		//cout<<(char)i;
		cs_table[i] = 0;
		lens_table[i] = 0;
		rank_s.push_back(0);
	}
	rank_b.push_back(0);
	select_b.push_back(0);
}

void newSectionOfRankS() {
	for (i = 0; i < CHARSCALE; i++) {
		rank_s.push_back(0);
	}
}

void printLengthTableOfS() {
	for (int i = 0; i < CHARSCALE; i++) {
		if (lens_table[i] > 0) {
			cout << char(i) << "," << lens_table[i] << endl;
		}
	}
}


void printBitArray(vector<bool> &arr) {
	for (q = 0; q < arr.size(); q++) {
		cout << arr[q];
		if ((q + 1) % 8 == 0) {
			cout << " ";
		}
	}
	cout << endl;
}

void getBitArray(vector<bool> &arr, char *str, unsigned int &size) {
	for (i = 0; i < size; i++) {
		for (j = 7; j > -1; j--) {
			arr.push_back(str[i] & (1 << j));
		}
	}
}

void initBB() {
	bbp = fopen(bbFN.c_str(), "w+");
	for (i = 0; i < rank_b_section_count; i++) {
		char tmp[SECTIONSIZE / 8];
		for (j = 0; j < SECTIONSIZE; j += 8) {
			tmp[j / 8] = -1;
		}
		fwrite(tmp, SECTIONSIZE / 8, 1, bbp);
	}
	if (char_b_count > (SECTIONSIZE * rank_b_section_count / 8)) {
		unsigned int left_count = (unsigned int) char_b_count % (SECTIONSIZE / 8);
		char tmp[left_count];
		for (j = 0; j < left_count; j++) {
			tmp[j] = -1;
		}
		fwrite(tmp, left_count, 1, bbp);
	}
}

void readSBySection(unsigned int &target_section) {
	//target section not in current buffer.
	if ((target_section - current_s_buffer_section) * SECTIONSIZE / MAXSIZE > 0 ||
		current_s_buffer_section > target_section) {
		fseek(sp, target_section * SECTIONSIZE, 0);
		current_s_buffer_size = (unsigned int) fread(s_buffer, 1, MAXSIZE, sp);
		current_s_buffer_section = target_section;
	}
}

void readBBySection(unsigned int &target_section) {
	//target section not in current buffer.
	if ((target_section - current_b_buffer_section) * SECTIONSIZE / MAXSIZE > 0 ||
		current_b_buffer_section > target_section) {
		fseek(bp, target_section * BIT_SECTION_SIZE_OF_CHAR, 0);
		current_b_buffer_size = (unsigned int) fread(b_buffer, 1, MAXSIZE, bp);
		current_b_buffer_section = target_section;
	}
}


//for occS
unsigned int result_occ_s = 0;
int gap_occ_s = 0;
unsigned int prev_occ_s = 0;
unsigned int lower_bound_occ_s = 0;

unsigned int occS(unsigned int target_num, unsigned int &target_char_int) {
	char target_char = (char) target_char_int;
	target_num += 1;
	//while k=0,all rank_s[char] is 0,hence k start at 1.
	for (k = 1; k <= s_section_count; k++) {
		if (target_num <= rank_s[k * CHARSCALE + target_char_int]) {
			break;
		}
	}

	lower_bound_occ_s = k - 1;
	// now k is the section that occ[x] > target;
	result_occ_s = (unsigned int) SECTIONSIZE * lower_bound_occ_s;
	prev_occ_s = (unsigned int) rank_s[lower_bound_occ_s * CHARSCALE + target_char_int];
	//if the target section in the current buffer?
	gap_occ_s = (k - 1) - current_s_buffer_section;
	//yes.
	if (gap_occ_s >= 0 && (gap_occ_s < (MAXSIZE / SECTIONSIZE))) {
		for (a = gap_occ_s * SECTIONSIZE; a < (k - current_s_buffer_section) * SECTIONSIZE; a++) {
			if (s_buffer[a] == target_char) {
				prev_occ_s++;
				if (prev_occ_s == target_num) {
					result_occ_s = a + current_s_buffer_section * SECTIONSIZE;
					return result_occ_s;
				}
			}
		}
	} else {
		//no,fetch the new s_buffer.
		readSBySection(lower_bound_occ_s);
		for (a = 0; a < SECTIONSIZE; a++) {
			if (s_buffer[a] == target_char) {
				prev_occ_s++;
				if (prev_occ_s == target_num) {
					result_occ_s += a;
					return result_occ_s;
				}
			}
		}

	}
}

//rankB
int gap_rank_b = 0;
unsigned int lower_bound_rank_b = 0;
unsigned int result_rank_b = 0, prev_rank_b = 0;
unsigned int outer = 0, inner = 0;
unsigned int rankB(unsigned int target_num) {
	target_num += 1;
	for (w = 1; w <= rank_b_section_count; w++) {
		if (target_num <= rank_b[w]) {
			break;
		}
	}
	lower_bound_rank_b = w - 1;
	result_rank_b = rank_b[lower_bound_rank_b];
	prev_rank_b = rank_b[lower_bound_rank_b];
	gap_rank_b = lower_bound_rank_b - current_b_buffer_section;
	//in buffer
	if (gap_rank_b >= 0 && (gap_rank_b < (8 * MAXSIZE / SECTIONSIZE))) {
		for (outer = gap_rank_b*BIT_SECTION_SIZE_OF_CHAR;
			 outer <= (w - current_b_buffer_section) * BIT_SECTION_SIZE_OF_CHAR; outer++) {
			//every char
			for (inner = 0; inner < 8; inner++) {
				//every bit in each char
				if (b_buffer[outer] & (128 >> inner)) {
					prev_rank_b++;
					if (prev_rank_b == target_num) {
						result_rank_b = inner + 8 * (outer + current_b_buffer_section * BIT_SECTION_SIZE_OF_CHAR);
						return result_rank_b;
					}
				}
			}
		}
	} else {
		//not in buffer
		readBBySection(lower_bound_rank_b);
		for (outer = 0; outer < BIT_SECTION_SIZE_OF_CHAR; outer++) {
			//every char
			for (inner = 0; inner < 8; inner++) {
				//every bit in each char
				if (b_buffer[outer] & (128 >> inner)) {
					prev_rank_b++;
					if (prev_rank_b == target_num) {
						result_rank_b = inner + 8 * (outer + current_b_buffer_section * BIT_SECTION_SIZE_OF_CHAR);
						return result_rank_b;
					}
				}
			}
		}
	}
	return 0;
}

//get the following zero of a bit 1 inside B
unsigned int char_loc=0;
unsigned int section_of_loc=0;
unsigned int getZeros(unsigned int location){
	unsigned int result=0;
	while(true){
		location++;
		char_loc=location/8;
		//if this char in the current buffer?
		section_of_loc=char_loc/BIT_SECTION_SIZE_OF_CHAR;
		//not in buffer
		if(current_b_buffer_section>section_of_loc || section_of_loc-current_b_buffer_section>=MAXSIZE/SECTIONSIZE){
			readBBySection(section_of_loc);
		}
		if(!(b_buffer[location/8-current_b_buffer_section*BIT_SECTION_SIZE_OF_CHAR] & (128 >> (location%8)))){
			result++;
		}else{
			return result;
		}
	}
}

//write the zeros value into the bb_buffer
void writeZerosIntoBB(unsigned int &baseline_bb, unsigned int zeros){
	while(zeros>0){
		//current location not in buffer: write the current content to file,
		//and then read the next section.
		if((baseline_bb/8)>=current_bb_buffer_size+BB_BUFFER_SIZE*current_bb_buffer_section){
			//cout<<"reading"<<endl;
			fseek(bbp,-BB_BUFFER_SIZE,1);
			//rewind(bbp);
			fwrite(bb_buffer,1,current_bb_buffer_size,bbp);
			current_bb_buffer_size=(unsigned int)fread(bb_buffer,1,BB_BUFFER_SIZE,bbp);
			current_bb_buffer_section++;
		}
		char_loc=baseline_bb/8-current_bb_buffer_section*BB_BUFFER_SIZE;
		//cout<<baseline_bb<<", "<<char_loc<<", "<<(1<<baseline_bb%8)<<","<< (255-(1>>(baseline_bb%8)))<<endl;
		//cout<<baseline_bb<<", "<<char_loc<<", "<<(int)(bb_buffer[char_loc])<<","<< (255-(1>>(baseline_bb%8)))<<endl;
		//cout<<char_loc<<", "<<(int)(bb_buffer[char_loc])<<endl;
		//bb_buffer[char_loc] &= 0;
		bb_buffer[char_loc] &= (255-(128>>(baseline_bb%8)));
		/*
		cout<<baseline_bb/8<<endl;
		cout<<current_bb_buffer_section*BB_BUFFER_SIZE<<endl;
		 */
		baseline_bb++;
		zeros--;
	}
}

//generate the bb file.
void generateBB() {
	initBB();
	rewind(sp);
	rewind(bp);
	rewind(bbp);
	current_s_buffer_size=(unsigned int)fread(s_buffer, 1,MAXSIZE, sp);
	current_b_buffer_size=(unsigned int)fread(b_buffer, 1,MAXSIZE, bp);
	current_bb_buffer_size=(unsigned int)fread(bb_buffer,1,BB_BUFFER_SIZE,bbp);
	current_bb_buffer_section=0;
	unsigned int zeros=0;
	unsigned int baseline_bb=0;
	for (i = 0; i < CHARSCALE; i++) {
		if (lens_table[i] < 1) {
			continue;
		}
		j = 0;
		while (j < lens_table[i]) {
			//cout<<(char)i<<","<<getZeros(rankB(occS(j,i)))<<endl;
			zeros=getZeros(rankB(occS(j,i)));
			baseline_bb++;
			if(zeros>0){
				writeZerosIntoBB(baseline_bb,zeros);
			}
			j++;
		}
	}
	//final write
	fseek(bbp,current_bb_buffer_section*BB_BUFFER_SIZE,0);
	fwrite(bb_buffer,1,current_bb_buffer_size,bbp);
}

void readSB(string &fileName) {
	bbFN = fileName + ".bb";
	sFN = fileName + ".s";
	bFN = fileName + ".b";
	sp = fopen(sFN.c_str(), "r");
	bp = fopen(bFN.c_str(), "r");
	bbp = fopen(bbFN.c_str(), "r+");
	if (!sp || !bp) {
		cout << fileName + ".s/.b not exists!" << endl;
	}
	//c_table rank_s
	while (!feof(sp)) {
		current_s_buffer_size = (unsigned int) fread(s_buffer, 1, MAXSIZE, sp);
		for (i = 0; i < current_s_buffer_size; i++) {
			lens_table[(int) s_buffer[i]]++;
			if ((i + 1) % SECTIONSIZE == 0) {
				for (j = 0; j < CHARSCALE; j++) {
					rank_s.push_back(lens_table[j]);
				}
				s_section_count++;
			}
		}
		unsigned prev_chars_count = 0;
		count_of_s += current_s_buffer_size;
	}
	//rank_b select_b
	while (!feof(bp)) {
		current_b_buffer_size = (unsigned int) fread(b_buffer, 1, MAXSIZE, bp);
		char_b_count += current_b_buffer_size;
		//b_count : how many 1 in b
		unsigned int b_count = rank_b[rank_b_section_count];
		for (i = 0; i < current_b_buffer_size; i++) {
			for (j = 0; j < 8; j++) {
				if (b_buffer[i] & (128 >> j)) {
					b_count++;
					if (b_count % SECTIONSIZE == 0 && b_count > 0) {
						select_b.push_back(select_b_section_count * SECTIONSIZE + (i * 8 + j) % SECTIONSIZE);
						select_b_section_count++;
					}
					if (b_count + 1 == count_of_s) {
						suffix_b1_start = rank_b_section_count * SECTIONSIZE + (i * 8 + j) % SECTIONSIZE;
					}
				}
			}
			if ((i+1) % BIT_SECTION_SIZE_OF_CHAR == 0) {
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