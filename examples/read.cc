#include <DataManager.h>
#include <unistd.h>
#include <iostream>
using namespace std;


int main(){

    string sender_address = "tcp://127.0.0.1:12306";
    string receiver_address = "tcp://127.0.0.1:12307";

    dataman_init(receiver_address, sender_address, "receiver");


    while (1){
        cout << "1 second" << endl;
        usleep(1000000);
    }

    return 0;
}
