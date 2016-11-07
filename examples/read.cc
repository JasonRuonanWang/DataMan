#include <DataManager.h>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <numeric>
using namespace std;

void get(void *data,
         string doid,
         string var,
         string dtype,
         vector<uint64_t> putshape,
         vector<uint64_t> varshape,
         vector<uint64_t> offset){

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

    string sender_address = "tcp://127.0.0.1:12306";
    string receiver_address = "tcp://127.0.0.1:12307";
    string mode = "receiver";
    string prefix = "/tmp/";
    int num_pipes = 10;

    dataman_init(receiver_address, sender_address, mode, prefix, num_pipes);
    dataman_reg_cb(get);

    while (1){
        cout << "1 second" << endl;
        usleep(1000000);
    }

    return 0;
}



