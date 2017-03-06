CXX=clang++
CXX=g++

CXXFLAGS=-std=c++11 -Wno-return-type-c-linkage


default:mdtm

all:zmq mdtm zfp cache dump manager stream

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
	$(CXX) $(CXXFLAGS) readManager.cc -o readManager.exe -ldataman

stream:
	$(CXX) $(CXXFLAGS) writeStreamWorkflow.cc -o writeStreamWorkflow.exe -ldataman


clean:
	rm -rf *.exe *.dSYM




