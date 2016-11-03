#include <DataManager.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main(){

    cout << sizeof(unsigned long) << "   " << sizeof(unsigned long long) << "   " << sizeof(uint64_t) << endl;

    string sender_address = "tcp://127.0.0.1:12306";
    string receiver_address = "tcp://127.0.0.1:12307";
    string mode = "receiver";
    string prefix = "/tmp/";
    int num_pipes = 10;

    dataman_init(receiver_address, sender_address, mode, prefix, num_pipes);

    while (1){
        cout << "1 second" << endl;
        usleep(1000000);
    }

    return 0;
}



