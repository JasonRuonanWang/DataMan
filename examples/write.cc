#include <DataManager.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(){

    string sender_address;
    string receiver_address;


    sender_address = "tcp://127.0.0.1:12306";
    receiver_address = "tcp://127.0.0.1:12307";

    sender_address = "tcp://131.225.2.31:12306";
    receiver_address = "tcp://131.225.2.29:12310";

    string mode = "sender";
    string prefix = "/tmp/MdtmManPipes/";
    int num_pipes = 1;

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

    varshape[0] = 4;
    varshape[1] = 6;
    varshape[2] = 10;

    putshape[0] = 1;
    putshape[1] = 2;
    putshape[2] = 5;

    int datasize = 10;

    float data[datasize];

	cout << "writing data ... " << endl;

    for (int i=0; i<4; i++){
        for (int j=0; j<3; j++){
            for (int k=0; k<2; k++){
                for (int m=0; m<datasize; m++){
                    data[m] = 'A' + 1;
cout << data[m] << "  ";
                }
                offset[0] = i;
                offset[1] = j*2;
                offset[2] = k*5;
                dataman_write(data, "aaa", "data", "float", putshape, varshape, offset, 0, 100);
            }
        }

cout << endl;


    }

    dataman_flush();


    return 0;
}


