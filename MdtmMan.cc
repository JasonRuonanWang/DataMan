#include <sys/stat.h>
#include <unistd.h>
#include "MdtmMan.h"
#include "zmq.h"
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
    cout << "~MdtmMan" << endl;
    if(zmq_ipc_rep) zmq_close (zmq_ipc_rep);
    if(zmq_ipc_req) zmq_close (zmq_ipc_req);
    zmq_ipc_rep_thread_active = false;
    zmq_ipc_rep_thread->join();
    delete zmq_ipc_rep_thread;
}

int MdtmMan::get(void *data,
        string doid,
        string var,
        string &dtype,
        vector<uint64_t> &getshape,
        vector<uint64_t> &varshape,
        vector<uint64_t> &offset,
        uint64_t &timestep,
        int &tolerance,
        int &priority){

    return 0;
}

int MdtmMan::get(void *data,
        string doid,
        string var,
        string &dtype,
        vector<uint64_t> &varshape,
        uint64_t &timestep){

    return 0;
}

int MdtmMan::put(const void *data,
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

    uint64_t putsize = product(putshape, dsize(dtype));
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


void MdtmMan::on_recv(json j){

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
            get_callback(m_cache.get_buffer(jqueue.front()["var"]),
                    jqueue.front()["doid"],
                    jqueue.front()["var"],
                    jqueue.front()["dtype"],
                    jqueue.front()["varshape"].get<vector<uint64_t>>()
                    );
        }
        m_cache.clean(jqueue.front()["var"], "nan");
        bqueue.pop();
        iqueue.pop();
        jqueue.pop();
    }

    for(int outloop=0; outloop<10; outloop++){
        // determine the pipe for the head request
        json msg = jqueue.front();
        if(msg == nullptr) break;
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
                break;
            }
        }
        if(s == putsize){
            m_cache.put(bqueue.front(),
                    msg["doid"],
                    msg["var"],
                    msg["dtype"],
                    msg["putshape"],
                    msg["varshape"],
                    msg["offset"],
                    msg["timestep"],
                    0,
                    100);

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


void MdtmMan::zmq_ipc_rep_thread_func(){
    while (zmq_ipc_rep_thread_active){
        char msg[1024];
        zmq_recv (zmq_ipc_rep, msg, 1024, ZMQ_NOBLOCK);
        zmq_send (zmq_ipc_rep, "OK", 10, 0);
        usleep(10000);
    }
}


