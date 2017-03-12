#include <unistd.h>
#include <MdtmMan.h>
#include <iostream>
#include <vector>

using namespace std;

int main(){

    MdtmMan man;
    json msg;
    msg["local_ip"] = "127.0.0.1";
    msg["remote_ip"] = "127.0.0.1";
    msg["local_port"] = 12306;
    msg["remote_port"] = 12307;
    msg["stream_mode"] = "sender";
    msg["pipe_prefix"] = "/tmp/MdtmManPipes/";

    msg["num_channels"] = 4;
    msg["tolerance"] = {0,0,0,0};
    msg["priority"] = {100,100,1,1};

    man.init(msg);

    json msg2;
    msg2["putshape"] = {1,2,5};
    msg2["varshape"] = {4,6,10};
    msg2["doid"] = "aaa";
    msg2["var"] = "data";
    msg2["dtype"] = "float";

    int datasize = 10;
    float data[datasize];
    for(int loop=0; loop<100; loop++){
        for (int i=0; i<4; i++){
            for (int j=0; j<3; j++){
                for (int k=0; k<2; k++){
                    for (int m=0; m<datasize; m++){
                        data[m] = i*10000 + j*100 + k*10 + m;
                    }
                    msg2["offset"] = {i,j*2,k*5};
                    man.put(data, msg2, 0);
                    for (int i=0; i<10; i++)
                        cout << ((float*)data)[i] << " ";
                    cout << endl;
                }
            }
        }
        man.flush();
        cout << "Time Step " << loop << endl;
    }
    return 0;
}


