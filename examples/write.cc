#include <DataManager.h>
#include <unistd.h>


int main(){

    string sender_address = "tcp://127.0.0.1:12306";
    string receiver_address = "tcp://127.0.0.1:12307";

    dataman_init(sender_address, receiver_address);

    float *data = new float[1024*1024*1024];
    unsigned int shape[3] = {2,1024*1024,1024};

    for (int i=0; i<1024*1024*1024; i++){
        data[i] = i;
    }

    for (int i=0; i<10; i++){
        dataman_write(data, "aaa", "bbb", "float", shape, 0, 0, 0, 100);
        usleep(1000);
    }

    return 0;
}
