#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include "sys/types.h"
#include "sys/stat.h"



using namespace std;
#define MAXSIZE 20480


void generateBBFile(string &fileName){
	std::string bbFN,bFN,sFN;
	bbFN=fileName+".bb";
	sFN=fileName+".s";
	bFN=fileName+".b";
	FILE *sp,*bp;
	sp=fopen(sFN.c_str(),"r");
	bp=fopen(bFN.c_str(),"r");

	if(!sp||!bp){
		cout<<fileName+".s/.b not exists!"<<endl;
	}
	char s_buffer[MAXSIZE];
	char b_buffer[MAXSIZE];

	while (!feof(sp)||!feof(bp)){
		if(!feof(sp)){
			fgets(s_buffer,MAXSIZE,sp);
		}
		if(!feof(bp)){
			fgets(b_buffer,MAXSIZE,bp);
		}
	}


}

int main(int argc,char *argv[]) {
	if(argc<4){

	}

	std::string fileName=argv[2];
	FILE *fp;
	std::string bbFN,bFN,sFN;
	bbFN=fileName+".bb";
	fp=fopen(bbFN.c_str(),"r");
	if(!fp){
		generateBBFile(fileName);
	}

	if(access(argv[3],00)<0){
		mkdir(argv[3],0777);
	}

	std::cout <<fileName<<std::endl;
	return 0;
}