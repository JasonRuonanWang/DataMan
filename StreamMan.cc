#include<iostream>
#include<sstream>
#include"StreamMan.h"
#include"zmq.h"

using namespace std;


StreamMan::StreamMan(string local_ip, string remote_ip)
{
    zmq_context = zmq_ctx_new ();
    zmq_tcp_req = zmq_socket (zmq_context, ZMQ_REQ);
    zmq_tcp_rep = zmq_socket (zmq_context, ZMQ_REP);

    stringstream local, remote;
    local << "tcp://" << local_ip << ":12306";
    remote << "tcp://" << remote_ip << ":12307";

    zmq_connect (zmq_tcp_req, remote.str().c_str());
    zmq_bind (zmq_tcp_rep, local.str().c_str());

    zmq_tcp_rep_thread_active = true;
    zmq_tcp_rep_thread = new thread(&StreamMan::zmq_tcp_rep_thread_func, this);
}


StreamMan::~StreamMan(){


    zmq_tcp_rep_thread_active = false;
    zmq_tcp_rep_thread->join();
    delete zmq_tcp_rep_thread;

    zmq_close (zmq_tcp_req);
    zmq_close (zmq_tcp_rep);
    zmq_ctx_destroy (zmq_context);
}


void StreamMan::zmq_tcp_rep_thread_func(){
    while (zmq_tcp_rep_thread_active){
        char msg[1024];
        zmq_recv (zmq_tcp_rep, msg, 1024, 0);
        zmq_send (zmq_tcp_rep, "OK", 10, 0);
    }
}


