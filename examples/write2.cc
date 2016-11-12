#include <DataManager.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(){

    string sender_address = "tcp://127.0.0.1:12306";
    string receiver_address = "tcp://127.0.0.1:12307";
    string mode = "sender";
    string prefix = "/tmp/MdtmManPipes/";
    int num_pipes = 4;

    cout << "init" << endl;
    dataman_init(sender_address, receiver_address, mode, prefix, num_pipes);

    vector<uint64_t> putshape;
    putshape.push_back(1024);
    vector<uint64_t> varshape = putshape;
    vector<uint64_t> offset;

    float **data = new float*[10];
    int datasize = 1024*1024;
    for(int i=0; i<10; i++){
        data[i] = new float[datasize];
    }

    for (int i=0; i<10; i++){
        for (int j=0; j<datasize; j++){
            data[i][j] = i;
        }
    }

    for (int j=0; j<10000; j++){
        for (int i=0; i<10; i++){
            dataman_write(data[i], "aaa", "data", "float", putshape, varshape, offset, 0, 100/(i+1));
        }
    }

    return 0;
}


