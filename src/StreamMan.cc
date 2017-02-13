#include <iostream>
#include <unistd.h>
#include <sstream>
#include "StreamMan.h"
#include "zmq.h"

using namespace std;

StreamMan::StreamMan()
    :DataMan()
{
}

StreamMan::StreamMan(string local_address, string remote_address)
    :DataMan()
{
    init(local_address, remote_address);
}

StreamMan::~StreamMan(){
    if(zmq_meta_req) zmq_close(zmq_meta_req);
    if(zmq_meta_rep) zmq_close(zmq_meta_rep);
    if(zmq_context) zmq_ctx_destroy(zmq_context);
    zmq_meta_rep_thread_active = false;
    if(zmq_meta_rep_thread){
        if(zmq_meta_rep_thread->joinable())
            zmq_meta_rep_thread->join();
        delete zmq_meta_rep_thread;
    }
}

void StreamMan::init(string local_address, string remote_address){
    if(!zmq_context){
        zmq_context = zmq_ctx_new ();
        zmq_meta_req = zmq_socket (zmq_context, ZMQ_REQ);
        zmq_meta_rep = zmq_socket (zmq_context, ZMQ_REP);
        zmq_connect (zmq_meta_req, remote_address.c_str());
        cout << "StreamMan::init " << remote_address << " connected" << endl;
        zmq_bind (zmq_meta_rep, local_address.c_str());
        cout << "StreamMan::init " << local_address << " bound" << endl;
        zmq_meta_rep_thread_active = true;
        zmq_meta_rep_thread = new thread(&StreamMan::zmq_meta_rep_thread_func, this);
    }
}

void StreamMan::flush(){
    json msg;
    msg["operation"] = "flush";
    char ret[10];
    zmq_send(zmq_meta_req, msg.dump().c_str(), msg.dump().length(), 0);
    zmq_recv(zmq_meta_req, ret, 10, 0);
}

void StreamMan::zmq_meta_rep_thread_func(){
    while (zmq_meta_rep_thread_active){
        char msg[1024]="";
        int err = zmq_recv (zmq_meta_rep, msg, 1024, ZMQ_NOBLOCK);
        zmq_send (zmq_meta_rep, "OK", 10, 0);
        if (err>=0){
            cout << "StreamMan::zmq_meta_rep_thread_func: " << msg << endl;
            json j = json::parse(msg);
            if(getmode == "callback"){
                on_recv(j);
            }
        }
        usleep(10);
    }
}

int StreamMan::put(const void *data,
        string doid,
        string var,
        string dtype,
        vector<uint64_t> putshape,
        vector<uint64_t> varshape,
        vector<uint64_t> offset,
        uint64_t timestep,
        int tolerance,
        int priority,
        json msg)
{

    msg["operation"] = "put";
    msg["doid"] = doid;
    msg["var"] = var;
    msg["dtype"] = dtype;
    msg["putshape"] = putshape;
    msg["varshape"] = varshape;
    msg["offset"] = offset;
    msg["timestep"] = timestep;

    char ret[10];
    zmq_send(zmq_meta_req, msg.dump().c_str(), msg.dump().length(), 0);
    zmq_recv(zmq_meta_req, ret, 10, 0);
}



