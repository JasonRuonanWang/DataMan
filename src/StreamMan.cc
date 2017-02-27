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

StreamMan::StreamMan(string local_address, string remote_address, string mode)
    :DataMan()
{
    init(local_address, remote_address, mode);
}

StreamMan::~StreamMan(){
    if(zmq_meta) zmq_close(zmq_meta);
    if(zmq_context) zmq_ctx_destroy(zmq_context);
    zmq_meta_rep_thread_active = false;
    if(zmq_meta_rep_thread){
        if(zmq_meta_rep_thread->joinable())
            zmq_meta_rep_thread->join();
        delete zmq_meta_rep_thread;
    }
}

void StreamMan::init(string local_address, string remote_address, string mode){
    if(!zmq_context){
        zmq_context = zmq_ctx_new ();
        zmq_meta = zmq_socket (zmq_context, ZMQ_PAIR);
        if(mode == "sender"){
            zmq_connect (zmq_meta, remote_address.c_str());
            cout << "StreamMan::init " << remote_address << " connected" << endl;
        }
        else if(mode == "receiver"){
            zmq_bind (zmq_meta, local_address.c_str());
            cout << "StreamMan::init " << local_address << " bound" << endl;
        }
        zmq_meta_rep_thread_active = true;
        zmq_meta_rep_thread = new thread(&StreamMan::zmq_meta_rep_thread_func, this);
    }
}

void StreamMan::flush(){
    json msg;
    msg["operation"] = "flush";
    zmq_send(zmq_meta, msg.dump().c_str(), msg.dump().length(), 0);
}

void StreamMan::zmq_meta_rep_thread_func(){
    while (zmq_meta_rep_thread_active){
        char msg[1024]="";
        int err = zmq_recv (zmq_meta, msg, 1024, ZMQ_NOBLOCK);
        if (err>=0){
            cout << "StreamMan::zmq_meta_rep_thread_func: " << msg << endl;
            json j = json::parse(msg);
            if(m_getmode == "callback"){
                on_recv(j);
            }
        }
        usleep(10);
    }
}

int StreamMan::put(const void *p_data, json p_jmsg){
    zmq_send(zmq_meta, p_jmsg.dump().c_str(), p_jmsg.dump().length(), 0);
    return 0;
}



