all:
	c++ *.cc --shared -o libDataMan.so -std=c++11 -lzmq

install: all
	cp *.so $(libpath)/DataMan/lib/
	cp api.h $(libpath)/DataMan/include/DataManager.h


clean:
	rm -rf *.o *.so*



