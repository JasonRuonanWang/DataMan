


CC:=gcc
CFLAGS:=-O0 -g -Wall

CXX:=g++
CXXFLAGS:=$(CFLAGS) -std=c++11



all: callback_c callback_cpp

callback_c: glue
	$(CC) main.c glue.o CallBack.o -o callback_c.exe $(CFLAGS) -lstdc++

glue: glue.cpp callback
	$(CXX) glue.cpp -c -o glue.o $(CXXFLAGS)


callback_cpp: callback
	$(CXX) main.cpp CallBack.o -o callback_cpp.exe $(CXXFLAGS)

	
callback: CallBack.cpp
	$(CXX) CallBack.cpp -c -o CallBack.o $(CXXFLAGS)
	
	
clean:
	rm *.exe *.o