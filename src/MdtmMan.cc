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
        zmq_connect (zmq_ipc_req, "ipc:///tmp/ADIOS_MDTM_pipe");

        char buffer_return[10];
        zmq_send (zmq_ipc_req, pipe_desc.dump().c_str(), pipe_desc.dump().length(), 0);
        zmq_recv (zmq_ipc_req, buffer_return, 10, 0);

    }
    // Pipes
    mkdir(prefix.c_str(), 0755);
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
    if(zmq_ipc_req) zmq_close(zmq_ipc_req);
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
    msg["timestep"] = timestep;

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

    jqueue.push(j);
    bqueue.push(NULL);
    iqueue.push(0);

    // for flush
    /*
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
    */


    // for put
    for(int outloop=0; outloop<10; outloop++){

        // allocate buffer
        if(jqueue.front()["operation"] == "put"){

            uint64_t putsize = jqueue.front()["putsize"].get<uint64_t>();
            if(bqueue.front() == NULL) bqueue.front() = malloc(putsize);

            // determine the pipe for the head request
            json msg = jqueue.front();
            if(msg == nullptr) break;
            int pipeindex=0;
            for(int i=0; i<pipenames.size(); i++){
                if(rmquote(msg["pipe"]) == pipenames[i]){
                    pipeindex=i;
                }
            }

            // read the head request
            int s = iqueue.front();
            putsize = msg["putsize"].get<int>();
            while(s<putsize){
                int ret = read(pipes[pipeindex], ((char*)bqueue.front()) + s, putsize - s);
                if(ret > 0){
                    s += ret;
                }
                else{
                    break;
                }
            }

            cout << "--------------------------" << endl;
            cout << msg << endl;

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
                if(bqueue.front()) free(bqueue.front());
                bqueue.pop();
                iqueue.pop();
                jqueue.pop();
            }
            else{
                iqueue.front()=s;
            }
        }
    }
}


