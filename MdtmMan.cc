#include <sys/stat.h>
#include <unistd.h>
#include"MdtmMan.h"
#include"zmq.h"
#include <fcntl.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

MdtmMan::MdtmMan(string local_address,
        string remote_address,
        string mode,
        string prefix,
        int num_pipes,
        vector<int> tolerance,
        vector<int> priority)
    :StreamMan(local_address, remote_address)
{
    pipe_desc["operation"] = "init";
    pipe_desc["mode"] = mode;
    pipe_desc["pipe_prefix"] = prefix;

    string pipename_prefix = "MdtmManPipe";
    for(int i=0; i<num_pipes; i++){
        stringstream pipename;
        pipename << pipename_prefix << i;
        if(i==0){
            pipe_desc["pipe_names"] = {pipename.str()};
            pipe_desc["loss_tolerance"] = {tolerance[i]};
            pipe_desc["priority"] = {priority[i]};
        }
        else{
            pipe_desc["pipe_names"].insert(pipe_desc["pipe_names"].end(), pipename.str());
            pipe_desc["loss_tolerance"].insert(pipe_desc["loss_tolerance"].end(), tolerance[i]);
            pipe_desc["priority"].insert(pipe_desc["priority"].end(), priority[i]);
        }
    }

    // ZMQ_DataMan_MDTM

    if(mode=="sender"){
        zmq_ipc_req = zmq_socket (zmq_context, ZMQ_REQ);
        zmq_ipc_rep = zmq_socket (zmq_context, ZMQ_REP);
        zmq_connect (zmq_ipc_req, "ipc:///tmp/ADIOS_MDTM_pipe");
        zmq_bind (zmq_ipc_rep, "ipc:///tmp/MDTM_ADIOS_pipe");

        char buffer_return[10];
        zmq_send (zmq_ipc_req, pipe_desc.dump().c_str(), pipe_desc.dump().length(), 0);
        zmq_recv (zmq_ipc_req, buffer_return, 10, 0);

        zmq_ipc_rep_thread_active = true;
        zmq_ipc_rep_thread = new thread(&MdtmMan::zmq_ipc_rep_thread_func, this);
    }
    // Pipes
    for (int i=0; i<pipe_desc["pipe_names"].size(); i++){
        string filename = prefix + rmquote(pipe_desc["pipe_names"][i].dump());
        mkfifo(filename.c_str(), 0666);
    }

    for(int i=0; i<num_pipes; i++){
        stringstream pipename;
        pipename << pipename_prefix << i;
        string fullpipename = prefix + pipename.str();
        cout << fullpipename << endl;
        if (mode == "sender"){
            int fp = open(fullpipename.c_str(), O_WRONLY);
            pipes.push_back(fp);
        }
        if (mode == "receiver"){
            int fp = open(fullpipename.c_str(), O_RDONLY | O_NONBLOCK);
            pipes.push_back(fp);
        }
        printf("pipe pointer %d ------------------- \n", pipes[i]);
        pipenames.push_back(pipename.str());
    }




}

MdtmMan::~MdtmMan(){
    if(zmq_ipc_rep) zmq_close (zmq_ipc_rep);
    if(zmq_ipc_req) zmq_close (zmq_ipc_req);
    zmq_ipc_rep_thread_active = false;
    zmq_ipc_rep_thread->join();
    delete zmq_ipc_rep_thread;
}

int MdtmMan::put(const void *data,
        string doid,
        string var,
        string dtype,
        vector<uint64_t> putshape,
        vector<uint64_t> varshape,
        vector<uint64_t> offset,
        int tolerance,
        int priority)
{
    json msg;
    msg["operation"] = "put";
    msg["doid"] = doid;
    msg["var"] = var;
    msg["dtype"] = dtype;
    msg["putshape"] = putshape;
    msg["varshape"] = varshape;
    msg["offset"] = offset;

    int index = closest(priority, pipe_desc["priority"], true);
    msg["pipe"] = pipe_desc["pipe_names"][index];

    uint64_t putsize = std::accumulate(putshape.begin(), putshape.end(), dsize(dtype), std::multiplies<uint64_t>());
    msg["putsize"] = putsize;
    uint64_t varsize = std::accumulate(varshape.begin(), varshape.end(), dsize(dtype), std::multiplies<uint64_t>());
    msg["varsize"] = varsize;

    cout << msg << endl;

    char ret[10];
    zmq_send(zmq_tcp_req, msg.dump().c_str(), msg.dump().length(), 0);
    zmq_recv(zmq_tcp_req, ret, 10, 0);

    index=0;
    for(int i=0; i<pipenames.size(); i++){
        if(rmquote(msg["pipe"].dump()) == pipenames[i]){
            index=i;
        }
    }
    string pipename = rmquote(pipe_desc["pipe_prefix"].dump()) + rmquote(msg["pipe"].dump());

    write(pipes[index], data, putsize);

    return 0;
}


void MdtmMan::get(json j){

    // push new request
    void *buf=0;
    uint64_t putsize;
    if(j["operation"] == "put"){
        putsize = j["putsize"].get<uint64_t>();
        buf = malloc(putsize);
    }
    jqueue.push(j);
    bqueue.push(buf);
    iqueue.push(0);

    if(jqueue.front()["operation"] == "flush"){
        if(get_callback){
            get_callback(cache,
                    "aaa",
                    "data",
                    "",
                    vector<uint64_t>(),
                    cache_shape,
                    vector<uint64_t>());
        }
        uint64_t varsize = accumulate(cache_shape.begin(), cache_shape.end(), 1, multiplies<uint64_t>());
        for(int i=0; i<varsize; i++){
            ((float*)cache)[i]=numeric_limits<float>::quiet_NaN();
        }
        bqueue.pop();
        iqueue.pop();
        jqueue.pop();
    }

    for(int outloop=0; outloop<10; outloop++){
        // determine the pipe for the head request
        json msg = jqueue.front();
        int index=0;
        for(int i=0; i<pipenames.size(); i++){
            if(rmquote(msg["pipe"]) == pipenames[i]){
                index=i;
            }
        }

        // read the head request
        int s = iqueue.front();
        putsize = msg["putsize"].get<int>();
        while(s<putsize){
            int ret = read(pipes[index], ((char*)bqueue.front()) + s, putsize - s);
            if(ret > 0){
                s += ret;
            }
            else{
                //                usleep(100);
                break;
            }
        }

        if(s == putsize){


            cache_it(bqueue.front(),
                    msg["varshape"].get<vector<uint64_t>>(),
                    msg["putshape"].get<vector<uint64_t>>(),
                    msg["offset"].get<vector<uint64_t>>());
            free(bqueue.front());
            bqueue.pop();
            iqueue.pop();
            jqueue.pop();
        }
        else{
            iqueue.front()=s;
        }

    }



}

int MdtmMan::get(void *data, json j){
    return 0;
}

void MdtmMan::zmq_ipc_rep_thread_func(){
    while (zmq_ipc_rep_thread_active){
        char msg[1024];
        zmq_recv (zmq_ipc_rep, msg, 1024, ZMQ_NOBLOCK);
        zmq_send (zmq_ipc_rep, "OK", 10, 0);
        usleep(10000);
    }
}


