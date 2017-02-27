#include <sys/stat.h>
#include <unistd.h>
#include "MdtmMan.h"
#include "zmq.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

MdtmMan::MdtmMan(){

}

MdtmMan::MdtmMan(string local_address,
        string remote_address,
        string mode,
        string prefix,
        int num_pipes,
        vector<int> tolerance,
        vector<int> priority)
    :StreamMan(local_address, remote_address, mode)
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


int MdtmMan::put(const void *p_data, json p_jmsg){

    int priority = p_jmsg["priority"].get<int>();
    vector<size_t> putshape = p_jmsg["putshape"].get<vector<size_t>>();
    vector<size_t> varshape = p_jmsg["varshape"].get<vector<size_t>>();
    string dtype = p_jmsg["dtype"];

    int index = closest(priority, pipe_desc["priority"], true);
    p_jmsg["pipe"] = pipe_desc["pipe_names"][index];
    size_t putbytes = product(putshape, dsize(dtype));
    p_jmsg["putbytes"] = putbytes;
    size_t varbytes = product(varshape, dsize(dtype));
    p_jmsg["varbytes"] = varbytes;

    StreamMan::put(p_data, p_jmsg);

    index=0;
    for(int i=0; i<pipenames.size(); i++){
        if(rmquote(p_jmsg["pipe"].dump()) == pipenames[i]){
            index=i;
        }
    }
    string pipename = rmquote(pipe_desc["pipe_prefix"].dump()) + rmquote(p_jmsg["pipe"].dump());
    write(pipes[index], p_data, putbytes);
    return 0;
}

int MdtmMan::get(void *p_data, json &p_jmsg){
    return 0;
}

void MdtmMan::on_recv(json j){

    // push new request

    jqueue.push(j);
    bqueue.push(NULL);
    iqueue.push(0);

    // for flush
    if(jqueue.front()["operation"] == "flush"){
        callback();
        m_cache.clean_all("nan");
        bqueue.pop();
        iqueue.pop();
        jqueue.pop();
    }

    // for put
    for(int outloop=0; outloop<10; outloop++){

        // allocate buffer
        if(jqueue.front()["operation"] == "put"){

            size_t putbytes = jqueue.front()["putbytes"].get<size_t>();
            if(bqueue.front() == NULL) bqueue.front() = malloc(putbytes);

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
            putbytes = msg["putbytes"].get<int>();
            while(s<putbytes){
                int ret = read(pipes[pipeindex], ((char*)bqueue.front()) + s, putbytes - s);
                if(ret > 0){
                    s += ret;
                }
                else{
                    break;
                }
            }

            cout << "--------------------------" << endl;
            cout << msg << endl;

            if(s == putbytes){
                m_cache.put(bqueue.front(),msg);
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


