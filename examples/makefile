UNAME_S:=$(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CXX:=g++
else
	CXX:=g++
endif

CXXFLAGS=-std=c++11 -Wno-return-type-c-linkage
LDFLAGS=-ldl

default:stream

all:zmq mdtm zfp dump manager stream

dump:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeDump.cc -o writeDump.exe -ldataman

zfp:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeZfp.cc -o writeZfp.exe -ldataman

zmq:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -lzmqman -lzmq writeZmq.cc -o writeZmq.exe
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -lzmqman -lzmq readZmq.cc -o readZmq.exe

mdtm:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) readMdtm.cc -o readMdtm.exe -lmdtmman -lzmq
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeMdtm.cc -o writeMdtm.exe -lmdtmman -lzmq

manager:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeManager.cc -o writeManager.exe -ldataman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) readManager.cc -o readManager.exe -ldataman

stream:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeStreamWorkflow.cc -o writeStreamWorkflow.exe -ldataman

clean:
	rm -rf *.exe *.dSYM




