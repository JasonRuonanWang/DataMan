#include"MdtmMan.h"
#include"json/json.hpp"
#include"zmq.h"
using json = nlohmann::json;


MdtmMan::MdtmMan(string local_ip, string remote_ip, string mode, string prefix, int num_pipes, int *tolerance, int *priority)
    :StreamMan(local_ip, remote_ip)
{
    json msg;
    msg["operation"] = "init";
    msg["mode"] = mode;
    msg["pipe_prefix"] = prefix;
    msg["pipe_names"] = {"red", "yellow", "green"};
    msg["loss_tolerance"] = {0,0,1};
    msg["priority"] = {0,3,10};

    zmq_ipc_req = zmq_socket (zmq_context, ZMQ_REQ);
    zmq_connect (zmq_ipc_req, "ipc:///tmp/ADIOS_MDTM_zmq");

    char buffer_return[10];
    zmq_send (zmq_ipc_req, msg.dump().c_str(), msg.dump().length(), 0);
    zmq_recv (zmq_ipc_req, buffer_return, 10, 0);

}

MdtmMan::~MdtmMan(){
    zmq_close (zmq_ipc_req);
}

int MdtmMan::put(void *data, string doid, string var, int *varshape = 0, int *offset = 0, int *putshape = 0){

    json msg;

    cout << doid << endl;
    cout << var << endl;
    cout << ((float*)data)[0] << endl;
    cout << ((float*)data)[1] << endl;
    cout << ((float*)data)[2] << endl;
    cout << ((float*)data)[3] << endl;
    return 0;
}


