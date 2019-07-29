CXXFLAGS = -std=c++14 -pthread -Wall -Wno-dangling-else -O3

CXX := /usr/bin/clang++

all: rlebwt

rlebwt:
	$(CXX) $(CXXFLAGS) -o rlebwt rlebwt.cpp utils.cpp

