CXX=g++-6


all:
	$(CXX) writeDump.cc -o writeDump -ldumpman
	$(CXX) writeMdtm.cc -o writeMdtm -lmdtmman -lstreamman
	$(CXX) readMdtm.cc -o readMdtm -lmdtmman -lstreamman
#	$(CXX) -g write.cc -o write -ldataman
#	$(CXX) -g write2.cc -o write2 -ldataman

clean:
	rm -rf write write2 read *.dSYM




