#include <sys/stat.h>
#include <unistd.h>
#include "ZmqMan.h"
#include "zmq.h"

ZmqMan::ZmqMan()
    :StreamMan()
{
}

ZmqMan::~ZmqMan(){
    if(zmq_data) zmq_close(zmq_data);
}

int ZmqMan::init(json p_jmsg){
    StreamMan::init(p_jmsg);
    zmq_data = zmq_socket (zmq_context, ZMQ_PAIR);
    string local_address = make_address(m_local_ip, m_local_port+1, "tcp");
    string remote_address = make_address(m_remote_ip, m_remote_port+1, "tcp");
    if(m_stream_mode=="sender"){
        zmq_connect (zmq_data, remote_address.c_str());
        cout << "ZmqMan::init " << remote_address << " connected" << endl;
    }
    else if(m_stream_mode=="receiver"){
        zmq_bind (zmq_data, local_address.c_str());
        cout << "ZmqMan::init " << local_address << " bound" << endl;
    }
    return 0;
}

int ZmqMan::put(const void *p_data, json p_jmsg, int p_flag){
    put_begin(p_data, p_jmsg);
    StreamMan::put(p_data, p_jmsg);
    zmq_send(zmq_data, p_data, p_jmsg["putbytes"], 0);
    put_end(p_data, p_jmsg);
    return 0;
}

int ZmqMan::get(void *p_data, json &p_jmsg){
    return 0;
}

void ZmqMan::on_recv(json msg){
    if (msg["operation"] == "put"){
        cout << "ZmqMan::on_recv 1" << endl;
        size_t putbytes = msg["putbytes"].get<size_t>();
        void *data = malloc(putbytes);
        int err = zmq_recv (zmq_data, data, putbytes, 0);
        m_cache.put(data, msg);
        free(data);
        cout << "ZmqMan::on_recv 2" << endl;
    }
    else if (msg["operation"] == "flush"){
        callback();
        m_cache.clean_all("nan");
    }
}

