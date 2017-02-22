CXX=clang++
CXX=g++-6

CXXFLAGS=-std=c++11

default:zmq

all:zmq mdtm zfp cache dump

dump:
	$(CXX) writeDump.cc -o writeDump.e -ldumpman

zfp:
	$(CXX) writeZfp.cc -o writeZfp.e

zmq:
	$(CXX) $(CXXFLAGS) writeZmq.cc -o writeZmq.e -lzmqman -lstreamman
	$(CXX) $(CXXFLAGS) -g readZmq.cc -o readZmq.e -lzmqman -lstreamman

mdtm:
	$(CXX) readMdtm.cc -o readMdtm.e -lmdtmman -lstreamman
	$(CXX) writeMdtm.cc -o writeMdtm.e -lmdtmman -lstreamman

cache:
	$(CXX) writeCache.cc -o writeCache.e -lcacheman


clean:
	rm -rf *.e *.dSYM




