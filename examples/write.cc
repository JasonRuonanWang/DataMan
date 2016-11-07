#include <DataManager.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(){

    string sender_address = "tcp://127.0.0.1:12306";
    string receiver_address = "tcp://127.0.0.1:12307";
    string mode = "sender";
    string prefix = "/tmp/";
    int num_pipes = 10;

    cout << "init" << endl;
    dataman_init(sender_address, receiver_address, mode, prefix, num_pipes);


    vector<uint64_t> putshape;
    putshape.push_back(0);
    putshape.push_back(0);
    putshape.push_back(0);

    vector<uint64_t> varshape;
    varshape.push_back(0);
    varshape.push_back(0);
    varshape.push_back(0);

    vector<uint64_t> offset;
    offset.push_back(0);
    offset.push_back(0);
    offset.push_back(0);

    varshape[0] = 10;
    varshape[1] = 20;
    varshape[2] = 30;

    putshape[0] = 1;
    putshape[1] = 20;
    putshape[2] = 30;

    int datasize = 20*30;

    float data[datasize];

    for (int i=0; i<10; i++){
        for (int j=0; j<datasize; j++){
            data[j] = i*1000 + j;
        }
        offset[0] = i;
        dataman_write(data, "aaa", "data", "float", putshape, varshape, offset, 0, 100);
    }

    dataman_flush();


    return 0;
}


