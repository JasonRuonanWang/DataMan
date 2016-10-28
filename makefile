all:
	c++ -fPIC *.cc --shared -o libDataMan.so -std=c++11 -lzmq

install: all
	cp *.so $(libpath)/DataMan/lib/
	cp DataManager.h $(libpath)/DataMan/include



clean:
	rm -rf *.o *.so*
	cd examples; make clean



