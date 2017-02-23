CXX=clang++
CXX=g++

CXXFLAGS=-std=c++11

default:manager

all:zmq mdtm zfp cache dump manager

dump:
	$(CXX) writeDump.cc -o writeDump.exe -ldumpman

zfp:
	$(CXX) writeZfp.cc -o writeZfp.exe

zmq:
	$(CXX) $(CXXFLAGS) writeZmq.cc -o writeZmq.exe -lzmqman -lstreamman
	$(CXX) $(CXXFLAGS) readZmq.cc -o readZmq.exe -lzmqman -lstreamman

mdtm:
	$(CXX) readMdtm.cc -o readMdtm.exe -lmdtmman -lstreamman
	$(CXX) writeMdtm.cc -o writeMdtm.exe -lmdtmman -lstreamman

cache:
	$(CXX) writeCache.cc -o writeCache.exe -lcacheman

manager:
	$(CXX) writeManager.cc -o writeManager.exe -ldataman

clean:
	rm -rf *.exe *.dSYM




