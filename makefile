# To build with ZeroMQ support, set:
#   ZMQ_CXXFLAGS - C++ flags to use ZeroMQ
#   ZMQ_LDFLAGS  - Linker flags to use ZeroMQ
# or ZMQ_PREFIX which wil automatically set:
#   ZMQ_CXXFLAGS = -I$(ZMQ_PREFIX)/include
#   ZMQ_LDFLAGS  = -L$(ZMQ_PREFIX)/lib -lzmq
#
# To build with ZFP support, set:
#   ZFP_CXXFLAGS - C++ flags to use ZFP
#   ZFP_LDFLAGS  - Linker flags to use ZFP
# or ZFP_PREFIX which wil automatically set:
#   ZFP_CXXFLAGS = -I$(ZFP_PREFIX)/include
#   ZFP_LDFLAGS  = -L$(ZFP_PREFIX)/lib -lzfp

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

# Setup ZFP dependencies
ifdef ZFP_PREFIX
        ZFP_CXXFLAGS=-I$(ZFP_PREFIX)/include
        ZFP_LDFLAGS=-L$(ZFP_PREFIX)/lib64 -lzfp
endif
ifdef ZFP_CXXFLAGS
ifdef ZFP_LDFLAGS
        ZFP_TARGETS=zfpman
endif
endif

default:manager
	make install

all:manager $(ZMQ_TARGETS) $(ZFP_TARGETS) dumpman
	make install
	cd examples; make all

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
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(ZFP_CXXFLAGS) $(ZFP_LDFLAGS) src/ZfpMan.cc --shared -o libzfpman.so -lcompressman

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


