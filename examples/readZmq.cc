#include <ZmqMan.h>
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

    string local_ip, remote_ip;
    int local_port, remote_port;

    local_ip = "131.225.2.29";
    remote_ip = "131.225.2.31";

    local_ip = "127.0.0.1";
    remote_ip = "127.0.0.1";

    local_port = 12307;
    remote_port = 12306;

    int num_channels = 1;

    vector<int> tolerance;
    vector<int> priority;
    tolerance.assign(num_channels, 0);
    priority.assign(num_channels, 0);

    ZmqMan man(local_ip, remote_ip, local_port, remote_port, num_channels, tolerance, priority);
    man.get_callback = get;

    for (int i=0; i<10; i++){
        cout << "1 second" << endl;
        usleep(1000000);
    }

    return 0;
}



