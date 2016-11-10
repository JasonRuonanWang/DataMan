all:
	c++ -fPIC *.cc --shared -o libDataMan.so -std=c++11 -lzmq
	cp libDataMan.so libdataman.so

install: all
	cp *.so $(libpath)/DataMan/lib/
	cp DataManager.h $(libpath)/DataMan/include



clean:
	rm -rf *.o *.so*
	cd examples; make clean



