#include <MdtmMan.h>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <numeric>
using namespace std;

void get(const void *data,
         string doid,
         string var,
         string dtype,
         vector<uint64_t> varshape
         ){

    cout << doid << endl;
    cout << var << endl;
    cout << dtype << endl;
    float *dataf = (float*)data;

    uint64_t varsize = std::accumulate(varshape.begin(), varshape.end(), 1, std::multiplies<uint64_t>());

    for (int i=0; i<varsize; i++)
        cout << ((float*)data)[i] << " ";
    cout << endl;

}


int main(){

    string sender_address;
    string receiver_address;

    sender_address = "tcp://131.225.2.29:12306";
    receiver_address = "tcp://131.225.2.31:12307";

    sender_address = "tcp://127.0.0.1:12306";
    receiver_address = "tcp://127.0.0.1:12307";

    string mode = "receiver";
    string prefix = "/tmp/MdtmManPipes/";
    int num_pipes = 4;

    vector<int> tolerance;
    vector<int> priority;
    tolerance.assign(num_pipes, 0);
    priority.assign(num_pipes, 0);

    MdtmMan mdtmman(receiver_address, sender_address, mode, prefix, num_pipes, tolerance, priority);
    mdtmman.reg_callback(get);


    for (int i=0; i<100; i++){
        cout << "1 second" << endl;
        usleep(1000000);
    }


    return 0;
}



