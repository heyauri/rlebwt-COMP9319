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
unsigned int i, j, k,a,s,d;


std::string bbFN, bFN, sFN;
char s_buffer[MAXSIZE];
char b_buffer[MAXSIZE];
char bb_buffer[MAXSIZE];

//for s
unsigned int cs_table[CHARSCALE], lens_table[CHARSCALE];
vector<unsigned int> rank_s;
unsigned int s_section_count = 0,current_s_buffer_section=0;
unsigned int current_s_buffer_size=0;

//for b
vector<unsigned int> rank_b, select_b;
int rank_b_section_count = 0, select_b_section_count = 0, char_b_count = 0;
int suffix_b1_start = 0;
unsigned int current_b_buffer_size=0;


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

void printLengthTableOfS(){
	for(int i=0;i<CHARSCALE;i++){
		if(lens_table[i]>0){
			cout<<char(i)<<","<<lens_table[i]<<endl;
		}
	}
}

void getBitArray(vector<bool> &arr, char *str, unsigned int &size) {
	for (i = 0; i < size; i++) {
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

void readSBySection(unsigned int &target_section){
	//target section not in current buffer.
	if((target_section-current_s_buffer_section)*SECTIONSIZE/MAXSIZE >0 || current_s_buffer_section>target_section){
		fseek(sp,target_section*SECTIONSIZE,0);
		current_s_buffer_size=(unsigned int)fread(s_buffer,1,MAXSIZE,sp);
		current_s_buffer_section=target_section;
	}
}

//for occS
unsigned int result_occ_s=0;
unsigned int gap_occ_s=0;
unsigned int prev_occ_s=0;
unsigned int lower_bound_occ_s=0;
unsigned int occS(unsigned int target_num, unsigned int &target_char_int){
	char target_char=(char)target_char_int;
	target_num+=1;
	//while k=0,all rank_s[char] is 0,hence k start at 1.
	for(k=1;k<=s_section_count;k++){
		if(target_num<=rank_s[k*CHARSCALE+target_char_int]){
			//cout<<"  "<<(char)i<<","<<rank_s[k*CHARSCALE+target_char_int]<<"  ";
			break;
		}
	}

	lower_bound_occ_s=k-1;
	// now k is the section that occ[x] > target;
	result_occ_s=(unsigned int)SECTIONSIZE*lower_bound_occ_s;
	prev_occ_s=(unsigned int)rank_s[lower_bound_occ_s*CHARSCALE+target_char_int];
	//if the target section in the current buffer?
	gap_occ_s=(k-1)-current_s_buffer_section;
	//yes.
	if(gap_occ_s>=0 && (gap_occ_s<(MAXSIZE/SECTIONSIZE))){
		/*
		cout<<"lower bound :"<<lower_bound_occ_s<<", ";
		cout<<"gap_occ_s :"<<gap_occ_s<<", ";
		cout<<"current_s_buffer_section :"<<current_s_buffer_section<<", ";
		cout<<((gap_occ_s>=0) && (gap_occ_s<(current_s_buffer_section+MAXSIZE/SECTIONSIZE)))<<", ";
		 */
		for(a=gap_occ_s*SECTIONSIZE;a<(k-current_s_buffer_section)*SECTIONSIZE;a++){
			if(s_buffer[a]==target_char){
				prev_occ_s++;
				if(prev_occ_s==target_num){
					result_occ_s=a+(lower_bound_occ_s-gap_occ_s)*SECTIONSIZE;
					return result_occ_s;
				}
			}
		}
	}else{
		//no,fetch the new s_buffer.
		readSBySection(lower_bound_occ_s);
		for(a=0;a<SECTIONSIZE;a++){
			if(s_buffer[a]==target_char){
				prev_occ_s++;
				if(prev_occ_s==target_num){
					result_occ_s+=a;
					return result_occ_s;
				}
			}
		}

	}
}

void generateBB() {
	initBB();
	rewind(sp);
	fread(s_buffer,MAXSIZE,1,sp);

	//printLengthTableOfS();

	for(i=0;i<CHARSCALE;i++){
		if(lens_table[i]<1){
			continue;
		}
		j=0;
		while(j<lens_table[i]){
			cout<<(char)i<<","<<occS(j,i)<<endl;
			j++;
		}

	}


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
		current_s_buffer_size=(unsigned int)fread(s_buffer,1, MAXSIZE, sp);
		for ( i = 0; i < current_s_buffer_size; i++) {
			lens_table[(int) s_buffer[i]]++;
			//cout<<s_buffer[i]<<lens_table[(int) s_buffer[i]]<<endl;
			if ((i + 1) % SECTIONSIZE == 0) {
				for (j = 0; j < CHARSCALE; j++) {
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
		current_b_buffer_size=(unsigned int)fread(b_buffer, 1,MAXSIZE, bp);
		vector<bool> b_arr;
		getBitArray(b_arr, b_buffer,current_b_buffer_size);
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