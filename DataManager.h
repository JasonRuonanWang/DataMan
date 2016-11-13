#include<string>
#include<vector>
using namespace std;


void dataman_flush();

void dataman_write(const void *data,
        string doid,
        string var,
        string dtype,
        vector<uint64_t> putshape = vector<uint64_t>(),
        vector<uint64_t> varshape = vector<uint64_t>(),
        vector<uint64_t> offset = vector<uint64_t>(),
        int tolerance=0,
        int priority=100);

void dataman_init(string local_address ="tcp://127.0.0.1:12306",
        string remote_address="tcp://127.0.0.1:12307",
//void dataman_init(string local_address ="tcp://131.225.2.31:12306",
//        string remote_address="tcp://131.225.2.29:12307",
        string mode="sender",
        string prefix="/tmp/MdtmManPipes/",
        int num_pipes=1,
        vector<int> tolerance=vector<int>(),
        vector<int> priority=vector<int>());

void dataman_reg_cb(void (*cb)
        (void *data,
         string doid,
         string var,
         string dtype,
         vector<uint64_t> putshape,
         vector<uint64_t> varshape,
         vector<uint64_t> offset)
        );

