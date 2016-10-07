#include<string>
using namespace std;

void dataman_write(void *data,
        string doid,
        string var,
        string dtype,
        unsigned int *putshape,
        unsigned int *varshape,
        unsigned int *offset);


void dataman_init(string local_ip,
        string remote_ip,
        string mode,
        string prefix,
        int num_pipes,
        int *tolerance,
        int *priority);


