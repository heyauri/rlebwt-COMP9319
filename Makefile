CXXFLAGS = -std=c++14 -pthread -Wall -Wno-dangling-else -O3

CXX := /usr/bin/clang++

#all: rlebwt rlebwt_test
all: rlebwt

rlebwt:
	$(CXX) $(CXXFLAGS) -o rlebwt rlebwt.cpp

rlebwt_test:
	$(CXX) $(CXXFLAGS) -o rlebwt_test rlebwt_for_test.cpp

