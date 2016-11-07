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

void StreamMan::cache_it(void *cache,
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

    for(uint64_t i=0; i<putshape[0]; i++){
        for(uint64_t j=0; j<putshape[1]; j++){
            for(uint64_t k=0; k<putshape[2]; k++){
                uint64_t oi=i+offset[0];
                uint64_t oj=j+offset[1];
                uint64_t ok=k+offset[2];
                uint64_t oindex = oi*SI + oj*SJ + ok*SK;
                uint64_t index = i*SI + j*SJ + k*SK;
                ((float*)cache)[oindex] = ((float*)data)[index];
            }
        }
    }

    cache_shape = varshape;

    /*
    for (int i=0; i<I*J*K; i++)
        cout << ((float*)cache)[i] << " ";
    cout << endl;
    */

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
                    int varsize = j["varsize"].get<int>();
                    if(!cache) cache = malloc(varsize);
                    int putsize = j["putsize"].get<int>();
                    void *data = malloc(putsize);
                    get(data, j);
                    if(j["var"] == "data")
                        cache_it(cache, data, j["varshape"].get<vector<uint64_t>>(), j["putshape"].get<vector<uint64_t>>(), j["offset"].get<vector<uint64_t>>());
                    /*
                    for (int i=0; i<putsize/4; i++)
                        cout << ((float*)data)[i] << " ";
                    cout << endl;
                    */
                    free(data);
                }
                if(j["operation"] == "flush"){
                    get_callback(cache,
                            "aaa",
                            "data",
                            "",
                            vector<uint64_t>(),
                            cache_shape,
                            vector<uint64_t>());
                }
            }
        }
        usleep(10000);
    }
}





