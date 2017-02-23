CXX=clang++
CXX=g++-6
CXXFLAGS=-std=c++11 -fPIC
LDFLAGS=-L./lib
INSTALL_PREFIX=$(libpath)


default:libdir manager zmqman install

all: libdir manager zmqman mdtmman zfpman dumpman install


dumpman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/DumpMan.cc --shared -o lib/libdumpman.so

cacheman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/CacheMan.cc --shared -o lib/libcacheman.so

# streaming methods
streamman:cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/StreamMan.cc --shared -o lib/libstreamman.so -lcacheman -lzmq

mdtmman:streamman cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/MdtmMan.cc --shared -o lib/libmdtmman.so -lstreamman -lcacheman -lzmq

zmqman:streamman cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/ZmqMan.cc --shared -o lib/libzmqman.so -lstreamman -lcacheman -lzmq

# compression methods
compressman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/CompressMan.cc --shared -o lib/libcompressman.so

zfpman:compressman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/ZfpMan.cc --shared -o lib/libzfpman.so -lcompressman -lzfp

manager:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/DataManager.cc --shared -o lib/libdataman.so

libdir:
	@( mkdir -p ./lib )

install:
	@( mkdir -p $(INSTALL_PREFIX)/DataMan/lib/);
	@( mkdir -p $(INSTALL_PREFIX)/DataMan/include/);
	cp lib/*.so $(INSTALL_PREFIX)/DataMan/lib/
	cp src/*.h src/*.hpp $(INSTALL_PREFIX)/DataMan/include

clean:
	rm -rf lib/*.so*
	cd examples; make clean


