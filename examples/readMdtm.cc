#include <MdtmMan.h>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <numeric>
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
    size_t printsize = std::accumulate(varshape.begin(), varshape.end(), 1, std::multiplies<size_t>());
    if(printsize > 1000) printsize = 1000;
    for (int i=0; i<printsize; i++)
        cout << ((float*)data)[i] << " ";
    cout << endl;
}

int main(){
    MdtmMan man;
    json j;
    j["local_ip"] = "127.0.0.1";
    j["remote_ip"] = "127.0.0.1";
    j["local_port"] = 12307;
    j["remote_port"] = 12306;
    j["stream_mode"] = "receiver";
    j["pipe_prefix"] = "/tmp/MdtmManPipes/";

    j["num_channels"] = 4;
    j["tolerance"] = {0,0,0,0};
    j["priority"] = {100,100,1,1};

    man.init(j);
    man.reg_callback(getcb);
    for (int i=0; i<100000; i++){
        cout << "1 second" << endl;
        usleep(1000000);
    }
    return 0;
}



