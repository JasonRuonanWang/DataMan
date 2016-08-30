all:
	cc *.c -c
	c++ *.cc -c
	c++ *.o --shared -o libDataMan.so

install: all
	cp *.so $(libpath)/UniMan/lib/


clean:
	rm -rf *.o *.so



