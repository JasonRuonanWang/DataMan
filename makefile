all:
	c++ *.cc --shared -o libDataMan.so -std=c++11 -lzmq

install: all
	cp *.so $(libpath)/DataMan/lib/


clean:
	rm -rf *.o *.so*



