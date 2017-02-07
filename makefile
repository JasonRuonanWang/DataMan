CXX=g++-6
CXX_FLAGS=-std=c++11
LINK_LIBS=-lzmq
INSTALL_PREFIX=$(libpath)

all:mdtmman

dataman:
	$(CXX) $(CXX_FLAGS) $(LINK_LIBS) src/DataMan.cc --shared -o lib/libdataman.so

dumpman:
	$(CXX) $(CXX_FLAGS) $(LINK_LIBS) src/DumpMan.cc --shared -o lib/libdumpman.so lib/libdataman.so

streamman:dataman cacheman
	$(CXX) $(CXX_FLAGS) $(LINK_LIBS) src/StreamMan.cc --shared -o lib/libstreamman.so lib/libdataman.so lib/libcacheman.so

cacheman:
	$(CXX) $(CXX_FLAGS) $(LINK_LIBS) src/CacheMan.cc --shared -o lib/libcacheman.so lib/libdataman.so

mdtmman:dataman streamman cacheman
	$(CXX) $(CXX_FLAGS) $(LINK_LIBS) src/MdtmMan.cc --shared -o lib/libmdtmman.so lib/libstreamman.so lib/libdataman.so lib/libcacheman.so

install:
	cp lib/*.so $(INSTALL_PREFIX)/DataMan/lib/
	cp src/*.h src/*.hpp $(INSTALL_PREFIX)/DataMan/include

clean:
	rm -rf *.o *.so*
	cd examples; make clean


