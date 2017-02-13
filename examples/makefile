CXX=g++-6

all:
	$(CXX) writeDump.cc -o writeDump.e -ldumpman

zmq:
	$(CXX) writeZmq.cc -o writeZmq.e -lzmqman -lstreamman
	$(CXX) readZmq.cc -o readZmq.e -lzmqman -lstreamman

mdtm:
	$(CXX) readMdtm.cc -o readMdtm.e -lmdtmman -lstreamman
	$(CXX) writeMdtm.cc -o writeMdtm.e -lmdtmman -lstreamman



clean:
	rm -rf write write2 read *.dSYM




