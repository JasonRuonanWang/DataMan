CXX=clang++
CXX=g++
CXXFLAGS=-std=c++11 -fPIC
LDFLAGS=-L. -Wno-return-type-c-linkage
INSTALL_PREFIX=$(libpath)


default:zmqman install

all:manager zmqman mdtmman zfpman dumpman install


dumpman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/DumpMan.cc --shared -o libdumpman.so

cacheman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/CacheMan.cc --shared -o libcacheman.so

# streaming methods
streamman:cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/StreamMan.cc --shared -o libstreamman.so -lcacheman -lzmq

mdtmman:streamman cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/MdtmMan.cc --shared -o libmdtmman.so -lstreamman -lcacheman -lzmq

zmqman:streamman cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/ZmqMan.cc --shared -o libzmqman.so -lstreamman -lcacheman -lzmq

# compression methods
compressman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/CompressMan.cc --shared -o libcompressman.so

zfpman:compressman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/ZfpMan.cc --shared -o libzfpman.so -lcompressman -lzfp

manager:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/DataManager.cc --shared -o libdataman.so -ldl


install:
	@( mkdir -p $(INSTALL_PREFIX)/DataMan/lib/);
	@( mkdir -p $(INSTALL_PREFIX)/DataMan/include/);
	cp *.so $(INSTALL_PREFIX)/DataMan/lib/
	cp src/*.h src/*.hpp $(INSTALL_PREFIX)/DataMan/include

clean:
	rm -f src/*.o *.so*
	cd examples; make clean


