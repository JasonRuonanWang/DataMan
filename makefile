# To build with ZeroMQ support, set:
#   ZMQ_CXXFLAGS - C++ flags to use ZeroMQ
#   ZMQ_LDFLAGS  - Linker flags to use ZeroMQ
# or ZMQ_PREFIX which wil automatically set:
#   ZMQ_CXXFLAGS = -I$(ZMQ_PREFIX)/include
#   ZMQ_LDFLAGS  = -L$(ZMQ_PREFIX)/lib -lzmq

UNAME_S:=$(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CXX:=clang++
else
	CXX:=g++
endif

CXXFLAGS=-std=c++11 -fPIC
LDFLAGS=-L. -Wno-return-type-c-linkage
INSTALL_PREFIX=$(libpath)

# Setup ZeroMQ dependencies
ifdef ZMQ_PREFIX
	ZMQ_CXXFLAGS=-I$(ZMQ_PREFIX)/include
	ZMQ_LDFLAGS=-L$(ZMQ_PREFIX)/lib -lzmq
endif
ifdef ZMQ_CXXFLAGS
ifdef ZMQ_LDFLAGS
	ZMQ_TARGETS=mdtmman zmqman
endif
endif


default:dumpman
	make install

all:manager $(ZMQ_TARGETS) zfpman dumpman
	make install


dumpman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/DumpMan.cc --shared -o libdumpman.so

cacheman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/CacheMan.cc --shared -o libcacheman.so

# streaming methods
streamman:cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(ZMQ_CXXFLAGS) $(ZMQ_LDFLAGS) src/StreamMan.cc --shared -o libstreamman.so -lcacheman

mdtmman:streamman cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(ZMQ_CXXFLAGS) $(ZMQ_LDFLAGS) src/MdtmMan.cc --shared -o libmdtmman.so -lstreamman -lcacheman

zmqman:streamman cacheman
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(ZMQ_CXXFLAGS) $(ZMQ_LDFLAGS) src/ZmqMan.cc --shared -o libzmqman.so -lstreamman -lcacheman

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


