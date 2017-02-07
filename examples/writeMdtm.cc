#include <unistd.h>
#include <MdtmMan.h>
#include <iostream>
#include <vector>

using namespace std;

int main(){

    string sender_address;
    string receiver_address;

    sender_address = "tcp://131.225.2.29:12306";
    receiver_address = "tcp://131.225.2.31:12307";

    sender_address = "tcp://127.0.0.1:12306";
    receiver_address = "tcp://127.0.0.1:12307";

    string mode = "sender";
    string prefix = "/tmp/MdtmManPipes/";
    int num_pipes = 4;

    vector<int> tolerance;
    vector<int> priority;
    tolerance.assign(num_pipes, 0);
    priority.assign(num_pipes, 0);

    MdtmMan mdtmman(sender_address, receiver_address, mode, prefix, num_pipes, tolerance, priority);

    vector<uint64_t> putshape;
    putshape.assign(3,0);

    vector<uint64_t> varshape;
    varshape.push_back(0);
    varshape.push_back(0);
    varshape.push_back(0);

    vector<uint64_t> offset;
    offset.push_back(0);
    offset.push_back(0);
    offset.push_back(0);

    varshape[0] = 4;
    varshape[1] = 6;
    varshape[2] = 10;

    putshape[0] = 1;
    putshape[1] = 2;
    putshape[2] = 5;

    int datasize = 10;

    float data[datasize];

    for (int i=0; i<4; i++){
        for (int j=0; j<3; j++){
            for (int k=0; k<2; k++){
                for (int m=0; m<datasize; m++){
                    data[m] = i*60 + j*10 + m;
                }
                offset[0] = i;
                offset[1] = j*2;
                offset[2] = k*5;
                mdtmman.put(data, "aaa", "data", "float", putshape, varshape, offset, 0, 0, 100);
            }
        }
    }
    mdtmman.flush();
    return 0;
}


