#include <iostream>
#include <unistd.h>
#include <sstream>
#include "StreamMan.h"
#include "zmq.h"

using namespace std;

StreamMan::StreamMan(string local_address, string remote_address)
{
    zmq_context = zmq_ctx_new ();
    //zmq_tcp_req = zmq_socket (zmq_context, ZMQ_REQ);
    //zmq_tcp_rep = zmq_socket (zmq_context, ZMQ_REP);
    zmq_tcp_req = zmq_socket (zmq_context, ZMQ_PAIR);
    zmq_tcp_rep = zmq_socket (zmq_context, ZMQ_PAIR);
    zmq_connect (zmq_tcp_req, remote_address.c_str());
    zmq_bind (zmq_tcp_rep, local_address.c_str());
    zmq_tcp_rep_thread_active = true;
    zmq_tcp_rep_thread = new thread(&StreamMan::zmq_tcp_rep_thread_func, this);
    cout << "StreamMan Create!" << endl;
}

StreamMan::~StreamMan(){
    zmq_tcp_rep_thread_active = false;
    zmq_tcp_rep_thread->join();
    if(zmq_tcp_rep_thread) delete zmq_tcp_rep_thread;
    //if(cache) free(cache);
    zmq_close (zmq_tcp_req);
    zmq_close (zmq_tcp_rep);
    zmq_ctx_destroy (zmq_context);
}

void StreamMan::flush(){
    json msg;
    msg["operation"] = "flush";
    char ret[10];
    zmq_send(zmq_tcp_req, msg.dump().c_str(), msg.dump().length(), 0);
    //zmq_recv(zmq_tcp_req, ret, 10, 0);
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

    for(uint64_t i=0; i<putshape[0]; i++){
        for(uint64_t j=0; j<putshape[1]; j++){
            for(uint64_t k=0; k<putshape[2]; k++){
                uint64_t oi=i+offset[0];
                uint64_t oj=j+offset[1];
                uint64_t ok=k+offset[2];
                uint64_t oindex = oi*SI + oj*SJ + ok*SK;
                uint64_t index = i*SI + j*SJ + k*SK;
		if(oindex > varsize - 1) { 
			//cout << "oindex too big: " << oindex << " varsize=" << varsize << endl; 
			oindex = varsize - 1; 
		}
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
        //zmq_send (zmq_tcp_rep, "OK", 10, 0);
        if (err>=0){
            cout << "StreamMan::zmq_tcp_rep_thread_func: " << msg << endl;
            json j = json::parse(msg);
            if(getmode == "callback"){
                if(j["operation"] == "put"){
                    uint64_t putsize = j["putsize"].get<uint64_t>();
                    void *data = malloc(putsize);
                    if(get(data, j)>= 0){
                    	if(j["var"] == "data"){
                        	uint64_t varsize = j["varsize"].get<uint64_t>();
                        	if(!cache){
                            		cache = malloc(varsize);
                            		for(int i=0; i<varsize/4; i++){
                                		((float*)cache)[i]=numeric_limits<float>::quiet_NaN();
                            		}
                        	}
                        	cache_it(data, j["varshape"].get<vector<uint64_t>>(), j["putshape"].get<vector<uint64_t>>(), j["offset"].get<vector<uint64_t>>());
                    	}
		    }
                    free(data);
                }
                if(j["operation"] == "flush"){
                    if(get_callback){
                        get_callback(cache,
                                "aaa",
                                "data",
                                "",
                                vector<uint64_t>(),
                                cache_shape,
                                vector<uint64_t>());
                    }
		    if(cache) {
                    	uint64_t varsize = accumulate(cache_shape.begin(), cache_shape.end(), 1, multiplies<uint64_t>());
                    	for(int i=0; i<varsize; i++){
                        	((float*)cache)[i]=numeric_limits<float>::quiet_NaN();
                    	}
		    }
                }
            }
        }
	else {
	  if( errno != EAGAIN ) {
          	cout << "zmq_recv failed: " << strerror(errno) << endl;
		switch(errno) {
			case EAGAIN:
				cout << "EAGAIN" << endl;
				break;
			case ENOTSUP:
				cout << "ENOTSUP" << endl;
				break;
			case EFSM:
				cout << "EFSM" << endl;
				break;
			case ETERM:
				cout << "ETERM" << endl;
				break;
			case ENOTSOCK:
				cout << "ENOTSOCK" << endl;
				break;
			case EINTR:
				cout << "EINTR" << endl;
				break;
		}
	  }
	}

        usleep(10000);
    }
    if(cache) free(cache);
}





