#include <DataManager.h>
#include <unistd.h>
#include <iostream>
using namespace std;


int main(){


    dataman_init("ipc:///tmp/sender_remote","ipc:///tmp/sender_local","receiver");


    while (1){
        cout << "1 second" << endl;
        usleep(1000000);
    }

    return 0;
}
