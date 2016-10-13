#include<string>
using namespace std;

void dataman_write(void *data,
        string doid,
        string var,
        string dtype,
        unsigned int *putshape,
        unsigned int *varshape,
        unsigned int *offset,
        int priority);

void dataman_init(string local_address ="tcp://127.0.0.1:12306",
        string remote_address="tcp://127.0.0.1:12307",
        string mode="sender",
        string prefix="/tmp/",
        int num_pipes=3,
        int *tolerance=NULL,
        int *priority=NULL);



