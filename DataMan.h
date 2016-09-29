#include<string>
#include<iostream>
using namespace std;

class DataMan{
    public:
        DataMan();
        virtual ~DataMan();
        virtual int put(void *data, string doid, string var, int *varshape, int *offset, int *putshape) = 0;


};


