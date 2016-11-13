#include <iostream>
#include <unistd.h>
#include <sstream>
#include "StreamMan.h"
#include "zmq.h"

using namespace std;

StreamMan::StreamMan(string local_address, string remote_address)
{
    zmq_context = zmq_ctx_new ();
    zmq_tcp_req = zmq_socket (zmq_context, ZMQ_REQ);
    zmq_tcp_rep = zmq_socket (zmq_context, ZMQ_REP);
    zmq_connect (zmq_tcp_req, remote_address.c_str());
    zmq_bind (zmq_tcp_rep, local_address.c_str());
    zmq_tcp_rep_thread_active = true;
    zmq_tcp_rep_thread = new thread(&StreamMan::zmq_tcp_rep_thread_func, this);
}

StreamMan::~StreamMan(){
    zmq_close (zmq_tcp_req);
    zmq_close (zmq_tcp_rep);
    zmq_ctx_destroy (zmq_context);
    zmq_tcp_rep_thread_active = false;
    zmq_tcp_rep_thread->join();
    if(zmq_tcp_rep_thread) delete zmq_tcp_rep_thread;
    if(cache) free(cache);
}

void StreamMan::flush(){
    json msg;
    msg["operation"] = "flush";
    char ret[10];
    zmq_send(zmq_tcp_req, msg.dump().c_str(), msg.dump().length(), 0);
    zmq_recv(zmq_tcp_req, ret, 10, 0);
}

void StreamMan::cache_it(
        void *data,
        vector<uint64_t> varshape,
        vector<uint64_t> putshape,
        vector<uint64_t> offset){

    uint64_t I = varshape[0];
    uint64_t J = varshape[1];
    uint64_t K = varshape[2];

    uint64_t SI = J*K;
    uint64_t SJ = K;
    uint64_t SK = 1;
    uint64_t s = 0;

    uint64_t putsize = std::accumulate(putshape.begin(), putshape.end(), 1, std::multiplies<uint64_t>());
    uint64_t varsize = std::accumulate(varshape.begin(), varshape.end(), 1, std::multiplies<uint64_t>());

    float *cachef = (float*) cache;
    float *dataf = (float*) data;

    // hardcoded for 3d array
    for(uint64_t i=0; i<putshape[0]; i++){
        for(uint64_t j=0; j<putshape[1]; j++){
            for(uint64_t k=0; k<putshape[2]; k++){
                uint64_t oi=i+offset[0];
                uint64_t oj=j+offset[1];
                uint64_t ok=k+offset[2];
                uint64_t oindex = oi*SI + oj*SJ + ok*SK;
                uint64_t index = i*SI + j*SJ + k*SK;
                cachef[oindex] =dataf[s];
                s++;
            }
        }
    }
    cache_shape = varshape;
}

void StreamMan::zmq_tcp_rep_thread_func(){
    while (zmq_tcp_rep_thread_active){
        char msg[1024]="";
        int err = zmq_recv (zmq_tcp_rep, msg, 1024, ZMQ_NOBLOCK);
        zmq_send (zmq_tcp_rep, "OK", 10, 0);
        if (err>=0){
            cout << "StreamMan::zmq_tcp_rep_thread_func: " << msg << endl;
            json j = json::parse(msg);
            if(getmode == "callback"){
                if(j["operation"] == "put"){
                    if(j["var"] == "data"){
                        if(!cache){
                            uint64_t varsize = j["varsize"].get<uint64_t>();
                            cache = malloc(varsize);
                            for(int i=0; i<varsize/4; i++){
                                ((float*)cache)[i]=numeric_limits<float>::quiet_NaN();
                            }
                        }
                    }
                    get(j);
                }
                if(j["operation"] == "flush"){
                }
            }
        }
        usleep(10000);
    }
}





