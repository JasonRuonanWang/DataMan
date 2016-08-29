all:
	cc *.c -c
	c++ *.cc -c
	c++ *.o --shared -o libUniMan.so

install: all
	cp libUniMan.so $(libpath)/UniMan/lib/


clean:
	rm -rf *.o *.so



