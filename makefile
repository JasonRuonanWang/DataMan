UNAME_S:=$(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CXX:=g++
else
	CXX:=g++
endif

SRCPATH=source/utilities/realtime/dataman
INCPATH=include
CXXFLAGS=-std=c++11 -fPIC -I$(INCPATH)
LDFLAGS=-L. -Wno-return-type-c-linkage

# Setup ZeroMQ dependencies
ZMQ_LDFLAGS=-lzmq
ifdef ZMQ_PREFIX
	ZMQ_CXXFLAGS=-I$(ZMQ_PREFIX)/include
	ZMQ_LDFLAGS+=-L$(ZMQ_PREFIX)/lib
endif
ifdef ZMQ_CXXFLAGS
ifdef ZMQ_LDFLAGS
	ZMQ_TARGETS=mdtmman zmqman
endif
endif

# Setup ZFP dependencies
ZFP_LDFLAGS=-lzfp
ifdef ZFP_PREFIX
	ZFP_CXXFLAGS=-I$(ZFP_PREFIX)/include
	ZFP_LDFLAGS+=-L$(ZFP_PREFIX)/lib64
endif
ifdef ZFP_CXXFLAGS
ifdef ZFP_LDFLAGS
	ZFP_TARGETS=zfpman
endif
endif



default:all

all:manager zfpman zmqman dumpman mdtmman temporalman
	cd examples; make all

dumpman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRCPATH)/DumpMan.cpp $(SRCPATH)/DataManBase.cpp --shared -o libdumpman.so

# streaming methods

mdtmman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(ZMQ_CXXFLAGS) $(ZMQ_LDFLAGS) $(SRCPATH)/MdtmMan.cpp $(SRCPATH)/StreamMan.cpp $(SRCPATH)/CacheMan.cpp $(SRCPATH)/DataManBase.cpp --shared -o libmdtmman.so

zmqman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(ZMQ_CXXFLAGS) $(ZMQ_LDFLAGS) $(SRCPATH)/ZmqMan.cpp $(SRCPATH)/StreamMan.cpp $(SRCPATH)/CacheMan.cpp $(SRCPATH)/DataManBase.cpp --shared -o libzmqman.so

# compression methods

zfpman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(ZFP_CXXFLAGS) $(ZFP_LDFLAGS) $(SRCPATH)/ZfpMan.cpp $(SRCPATH)/DataManBase.cpp --shared -o libzfpman.so

temporalman:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRCPATH)/TemporalMan.cpp $(SRCPATH)/DataManBase.cpp --shared -o libtemporalman.so

# DataManager
manager:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRCPATH)/DataMan.cpp $(SRCPATH)/DataManBase.cpp --shared -o libdataman.so -ldl


clean:
	rm -f src/*.o *.so*
	cd examples; make clean


