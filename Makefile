CXXFLAGS =  -Wall -Werror -g -lm

CXX :=  g++

#all: rlebwt rlebwt_test
all: rlebwt

rlebwt:
	$(CXX) $(CXXFLAGS) -o rlebwt rlebwt.cpp

rlebwt_test:
	$(CXX) $(CXXFLAGS) -o rlebwt_test rlebwt_for_test.cpp

