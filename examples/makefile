UNAME_S:=$(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CXX:=clang++
else
	CXX:=g++
endif

CXXFLAGS=-std=c++11 -Wno-return-type-c-linkage
LDFLAGS=-Wl,--no-as-needed -ldl

default:stream

all:zmq mdtm zfp cache dump manager stream

dump:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeDump.cc -o writeDump.exe -ldumpman

zfp:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeZfp.cc -o writeZfp.exe

zmq:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeZmq.cc -o writeZmq.exe -lzmqman -lstreamman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) readZmq.cc -o readZmq.exe -lzmqman -lstreamman

mdtm:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) readMdtm.cc -o readMdtm.exe -lmdtmman -lstreamman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeMdtm.cc -o writeMdtm.exe -lmdtmman -lstreamman

cache:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeCache.cc -o writeCache.exe -lcacheman

manager:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeManager.cc -o writeManager.exe -ldataman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) readManager.cc -o readManager.exe -ldataman

stream:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) writeStreamWorkflow.cc -o writeStreamWorkflow.exe -ldataman


clean:
	rm -rf *.exe *.dSYM




