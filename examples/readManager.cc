#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <numeric>
#include <utilities/realtime/dataman/DataManager.h>
using namespace std;

void getcb(const void *data,
         string doid,
         string var,
         string dtype,
         vector<size_t> varshape
         ){
    cout << "data object ID = " << doid << endl;
    cout << "variable name = " << var << endl;
    cout << "data type = " << dtype << endl;
    float *dataf = (float*)data;
    size_t varsize = std::accumulate(varshape.begin(), varshape.end(), 1, std::multiplies<size_t>());
    for (int i=0; i<varsize; i++)
        cout << ((float*)data)[i] << " ";
    cout << endl;
}

int main(){
    adios::realtime::DataManager man;
    json msg;
    msg["method"] = "zmq";
    msg["stream_mode"] = "receiver";
    msg["local_ip"] = "127.0.0.1";
    msg["remote_ip"] = "127.0.0.1";
    msg["local_port"] = 12307;
    msg["remote_port"] = 12306;
    msg["num_channels"] = 1;
    msg["stream_mode"] = "receiver";
    msg["tolerance"] = {0};
    msg["priority"] = {100};
    man.add_stream(msg);
    man.reg_callback(getcb);

    for (int i=0; i<100000; i++){
        cout << "1 second" << endl;
        usleep(1000000);
    }
    return 0;
}



