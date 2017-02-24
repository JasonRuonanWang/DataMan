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
        string mode,
        vector<int> tolerance,
        vector<int> priority)
{
    init(
        local_ip,
        remote_ip,
        local_port,
        remote_port,
        num_channels,
        mode,
        tolerance,
        priority);
}

ZmqMan::~ZmqMan(){
    if(zmq_data) zmq_close(zmq_data);
}

void ZmqMan::init(
        string local_ip,
        string remote_ip,
        int local_port,
        int remote_port,
        int num_channels,
        string mode,
        vector<int> tolerance,
        vector<int> priority){
    StreamMan::init(make_address(local_ip, local_port, "tcp"), make_address(remote_ip, remote_port, "tcp"), mode);

    m_local_ip = local_ip;
    m_remote_ip = remote_ip;
    m_local_port = local_port;
    m_remote_port = remote_port;
    m_num_channels = num_channels;

    zmq_data = zmq_socket (zmq_context, ZMQ_PAIR);

    string local_address = make_address(local_ip, local_port+1, "tcp");
    string remote_address = make_address(remote_ip, remote_port+1, "tcp");

    if(mode=="sender"){
        zmq_connect (zmq_data, remote_address.c_str());
        cout << "ZmqMan::init " << remote_address << " connected" << endl;
    }
    else if(mode=="receiver"){
        zmq_bind (zmq_data, local_address.c_str());
        cout << "ZmqMan::init " << local_address << " bound" << endl;
    }
}

int ZmqMan::put(const void *data,
        string doid,
        string var,
        string dtype,
        vector<size_t> putshape,
        vector<size_t> varshape,
        vector<size_t> offset,
        size_t timestep,
        int tolerance,
        int priority)
{
    json msg;
    size_t putsize = product(putshape, dsize(dtype));
    msg["putsize"] = putsize;
    size_t varsize = product(varshape, dsize(dtype));
    msg["varsize"] = varsize;
    StreamMan::put(data,doid,var,dtype,putshape,varshape,offset,timestep,tolerance,priority,msg);
    zmq_send(zmq_data, data, putsize, 0);
    return 0;
}

void ZmqMan::on_recv(json msg){
    if (msg["operation"] == "put"){
        size_t putsize = msg["putsize"].get<size_t>();
        void *data = malloc(putsize);
        int err = zmq_recv (zmq_data, data, putsize, 0);
        m_cache.put(data,
                msg["doid"],
                msg["var"],
                msg["dtype"],
                msg["putshape"],
                msg["varshape"],
                msg["offset"],
                msg["timestep"],
                0,
                100);
        free(data);
    }
    else if (msg["operation"] == "flush"){
        if(get_callback){
            vector<string> do_list = m_cache.get_do_list();
            for(string i : do_list){
                vector<string> var_list = m_cache.get_var_list(i);
                for(string j : var_list){
                    get_callback(m_cache.get_buffer(i,j),
                            i,
                            j,
                            m_cache.get_dtype(i, j),
                            m_cache.get_shape(i, j)
                            );
                }
            }
        }
        m_cache.clean_all("nan");
    }
}

