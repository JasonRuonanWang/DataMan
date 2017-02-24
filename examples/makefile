CXX=clang++
CXX=g++

CXXFLAGS=-std=c++11

default:zfp

all:zmq mdtm zfp cache dump manager

dump:
	$(CXX) $(CXXFLAGS) writeDump.cc -o writeDump.exe -ldumpman

zfp:
	$(CXX) $(CXXFLAGS) writeZfp.cc -o writeZfp.exe

zmq:
	$(CXX) $(CXXFLAGS) writeZmq.cc -o writeZmq.exe -lzmqman -lstreamman
	$(CXX) $(CXXFLAGS) readZmq.cc -o readZmq.exe -lzmqman -lstreamman

mdtm:
	$(CXX) $(CXXFLAGS) readMdtm.cc -o readMdtm.exe -lmdtmman -lstreamman
	$(CXX) $(CXXFLAGS) writeMdtm.cc -o writeMdtm.exe -lmdtmman -lstreamman

cache:
	$(CXX) $(CXXFLAGS) writeCache.cc -o writeCache.exe -lcacheman

manager:
	$(CXX) $(CXXFLAGS) writeManager.cc -o writeManager.exe -ldataman

clean:
	rm -rf *.exe *.dSYM




