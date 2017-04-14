#include <utilities/realtime/dataman/ZmqMan.h>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <numeric>
using namespace std;
using namespace adios::realtime;

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
    adios::realtime::ZmqMan man;
    json j;
    j["local_ip"] = "127.0.0.1";
    j["remote_ip"] = "127.0.0.1";
    j["local_port"] = 12307;
    j["remote_port"] = 12306;
    j["num_channels"] = 1;
    j["stream_mode"] = "receiver";
    j["tolerance"] = {0};
    j["priority"] = {100};
    man.init(j);
    man.reg_callback(getcb);
    for (int i=0; i<100000; i++){
        cout << "1 second" << endl;
        usleep(1000000);
    }
    return 0;
}



