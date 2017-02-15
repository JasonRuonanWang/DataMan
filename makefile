CXX=clang++
CXX=g++-6
CXXFLAGS=-std=c++11 -fPIC
LINK_LIBS=-lzmq
LDFLAGS=-L./lib
INSTALL_PREFIX=$(libpath)

all:mdtmman dumpman zmqman

dumpman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LINK_LIBS) src/DumpMan.cc --shared -o lib/libdumpman.so

streamman:cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LINK_LIBS) src/StreamMan.cc --shared -o lib/libstreamman.so -lcacheman

cacheman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LINK_LIBS) src/CacheMan.cc --shared -o lib/libcacheman.so

mdtmman:streamman cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LINK_LIBS) src/MdtmMan.cc --shared -o lib/libmdtmman.so -lstreamman -lcacheman

zmqman:streamman cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LINK_LIBS) src/ZmqMan.cc --shared -o lib/libzmqman.so -lstreamman -lcacheman

install:
	cp lib/*.so $(INSTALL_PREFIX)/DataMan/lib/
	cp src/*.h src/*.hpp $(INSTALL_PREFIX)/DataMan/include

clean:
	rm -rf *.o *.so*
	cd examples; make clean


