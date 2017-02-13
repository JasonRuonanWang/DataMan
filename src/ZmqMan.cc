#include <sys/stat.h>
#include <unistd.h>
#include "ZmqMan.h"
#include "zmq.h"

ZmqMan::ZmqMan()
:StreamMan()
{

}

ZmqMan::ZmqMan(
        string local_ip,
        string remote_ip,
        int local_port,
        int remote_port,
        int num_channels,
        vector<int> tolerance,
        vector<int> priority)
{
    init(
        local_ip,
        remote_ip,
        local_port,
        remote_port,
        num_channels,
        tolerance,
        priority);
}

ZmqMan::~ZmqMan(){
    if(zmq_data_req) zmq_close(zmq_data_req);
    if(zmq_data_rep) zmq_close(zmq_data_rep);
}

void ZmqMan::init(
        string local_ip,
        string remote_ip,
        int local_port,
        int remote_port,
        int num_channels,
        vector<int> tolerance,
        vector<int> priority){
    StreamMan::init(make_address(local_ip, local_port, "tcp"), make_address(remote_ip, remote_port, "tcp"));

    m_local_ip = local_ip;
    m_remote_ip = remote_ip;
    m_local_port = local_port;
    m_remote_port = remote_port;
    m_num_channels = num_channels;

    zmq_data_req = zmq_socket (zmq_context, ZMQ_REQ);
    zmq_data_rep = zmq_socket (zmq_context, ZMQ_REP);

    string local_address = make_address(local_ip, local_port+1, "tcp");
    string remote_address = make_address(remote_ip, remote_port+1, "tcp");

    zmq_connect (zmq_data_req, remote_address.c_str());
    cout << "ZmqMan::init " << remote_address << " connected" << endl;
    zmq_bind (zmq_data_rep, local_address.c_str());
    cout << "ZmqMan::init " << local_address << " bound" << endl;
}

int ZmqMan::put(const void *data,
        string doid,
        string var,
        string dtype,
        vector<uint64_t> putshape,
        vector<uint64_t> varshape,
        vector<uint64_t> offset,
        uint64_t timestep,
        int tolerance,
        int priority)
{
    cout << "ZmqMan::put" << endl;

    json msg;
    uint64_t putsize = product(putshape, dsize(dtype));
    msg["putsize"] = putsize;
    uint64_t varsize = product(varshape, dsize(dtype));
    msg["varsize"] = varsize;
    StreamMan::put(data,doid,var,dtype,putshape,varshape,offset,timestep,tolerance,priority,msg);

    char ret[10];
    zmq_send(zmq_data_req, data, putsize, 0);
    zmq_recv(zmq_data_req, ret, 10, 0);

    return 0;
}


void ZmqMan::on_recv(json j){
    if (j["operation"] == "put"){
        uint64_t putsize = j["putsize"].get<uint64_t>();
        void *data = malloc(putsize);
        int err = zmq_recv (zmq_data_rep, data, putsize, 0);
        zmq_send (zmq_data_rep, "OK", 10, 0);
        m_cache.put(data,
                j["doid"],
                j["var"],
                j["dtype"],
                j["putshape"],
                j["varshape"],
                j["offset"],
                j["timestep"],
                0,
                100);
    }
    else if (j["operation"] == "flush"){
        if(get_callback){
            get_callback(m_cache.get_buffer(j["var"]),
                    j["doid"],
                    j["var"],
                    j["dtype"],
                    j["varshape"].get<vector<uint64_t>>()
                    );
        }
        m_cache.clean(j["var"], "nan");
    }
}

