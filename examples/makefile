CXX=clang++
CXX=g++-6

CXXFLAGS=-std=c++11

all:
	$(CXX) writeDump.cc -o writeDump.e -ldumpman

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




