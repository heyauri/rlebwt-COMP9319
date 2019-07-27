#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include "sys/types.h"
#include "sys/stat.h"


using namespace std;
// the max size of buffers
#define MAXSIZE 10240
#define BITMAX MAXSIZE*8
int i, j, k;



void printBitArray(vector<bool> &arr) {
	for (i = 0; i < arr.size(); i++)//size()容器中实际数据个数
	{
		cout << arr[i];
		if ((i + 1) % 8 == 0) {
			cout << " ";
		}
	}
	cout << endl;
}

void writeBitToBitArray(vector<bool> &arr, int l){
	arr.push_back(true);
	while (l>0){
		arr.push_back(false);
		l--;
	}
}


void getBitArray(vector<bool> &arr, char *str) {
	for (i = 0; i < strlen(str); i++) {
		for (j = 7; j > -1; j--) {
			arr.push_back(str[i] & (1 << j));
		}
	}
}

void convertBitsToString(vector<bool> &arr,char (&str)[MAXSIZE]){
	printBitArray(arr);
	for ( i = 0; i < arr.size()/8; i++ ){
		str[i]=0;
		for(j=0;j<8;j++){
			//cout<<(arr[8*i+j]<<(7-j))<<" ";
			str[i] |= arr[8*i+j]<<(7-j);
		}
	}
	cout<<endl;
}

void sortFS(char (&fs)[MAXSIZE]){
	int fs_size=(int)strlen(fs);
	int min;
	char tmp;
	for(i=0;i<fs_size;i++){
		min=i;
		for(j=i+1;j<fs_size;j++){
			if(fs[j]<fs[min]){
				min=j;
			}
		}
		tmp=fs[i];
		fs[i]=fs[min];
		fs[min]=tmp;
	}
}

void sortFSAndBB(char (&fs)[MAXSIZE],vector<bool> &bb){
	int fs_size=(int)strlen(fs);
	int b_size=(int)bb.size();
	char tmp;
	int baseline=0,length_prev=0,length_next=0,status=0;
	for(i=0;i<fs_size-1;i++){
		baseline=0;
		for(j=0;j<fs_size-1-i;j++){
			length_next=0;
			length_prev=0;
			for(k=baseline;k<b_size;k++){
				if(bb[k]){
					status++;
				}
				if(status==1){
					length_prev++;
				} else if(status==2){
					length_next++;
				} else if(status==3){
					break;
				}
			}
			status=0;
			if(fs[j]>fs[j+1]){
				//cout<<baseline<<endl;
				//cout<<fs[j]<<length_prev<<","<<fs[j+1]<<length_next<<endl;
				tmp=fs[j+1];
				fs[j+1]=fs[j];
				fs[j]=tmp;
				if(length_next!=length_prev){
					bb[baseline+length_next]=true;
					bb[baseline+length_prev]=false;
				}
				/*
				for(int a=0;a<b_size;a++){
					cout<<bb[a];
					if((a+1)%8==0){
						cout<<" ";
					}
				}
				//cout<<endl;
				 */

				baseline+=length_next;

			}else{
				baseline+=length_prev;
			};

			//cout<<baseline<<endl;
		}
		//printBitArray(bb);
	}
}

void generateBBFile(string &fileName) {
	std::string bbFN, bFN, sFN;
	bbFN = fileName + ".bb";
	sFN = fileName + ".s";
	bFN = fileName + ".b";
	FILE *sp, *bp,*bbp;
	cout << bFN << endl;
	sp = fopen(sFN.c_str(), "r");
	bp = fopen(bFN.c_str(), "r");
	if (!sp || !bp) {
		cout << fileName + ".s/.b not exists!" << endl;
	}
	char s_buffer[MAXSIZE];
	char b_buffer[MAXSIZE];
	char bb_str[MAXSIZE];
	char fs[MAXSIZE];
	vector<bool> b_arr,bb_arr;
	int flag=0;
	while (!feof(sp) || !feof(bp)) {
		if (!feof(sp)) {
			fgets(s_buffer, MAXSIZE, sp);
		}
		if (!feof(bp)) {
			fgets(b_buffer, MAXSIZE, bp);
			getBitArray(b_arr, b_buffer);
			//printBitArray(b_arr);

			while (!flag){
				strcpy(fs,s_buffer);
				//cout<<fs<<endl;
				bb_arr.assign(b_arr.begin(),b_arr.end());
				sortFSAndBB(fs,bb_arr);
				//cout<<fs<<endl;
				//cout<<bb_arr.size()<<endl;
				//printBitArray(bb_arr);
				break;
			}

			convertBitsToString(bb_arr,bb_str);
			bbp=fopen(bbFN.c_str(),"w");
			fwrite(bb_str,strlen(bb_str),1,bbp);
		}


	}


}

int main(int argc, char *argv[]) {
	if (argc < 4) {

	}

	std::string fileName = argv[2];
	FILE *fp;
	std::string bbFN, bFN, sFN;
	bbFN = fileName + ".bb";
	fp = fopen(bbFN.c_str(), "r");
	if (!fp) {
		cout << "generate bb file..." << endl;
		generateBBFile(fileName);
	}

	if (access(argv[3], 00) < 0) {
		mkdir(argv[3], 0777);
	}

	//std::cout <<fileName<<std::endl;
	return 0;
}