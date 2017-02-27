#ifndef ZMQMAN_H_
#define ZMQMAN_H_

#include"StreamMan.h"

class ZmqMan : public StreamMan{
    public:
        ZmqMan();
        ~ZmqMan();
        ZmqMan(string local_ip,
                string remote_ip,
                int local_port,
                int remote_port,
                int num_channels,
                string mode,
                vector<int> tolerance,
                vector<int> priority);

        void init(
                string local_ip,
                string remote_ip,
                int local_port,
                int remote_port,
                int num_channels,
                string mode,
                vector<int> tolerance,
                vector<int> priority);

        virtual int put(const void *p_data, json p_jmsg);
        virtual int get(void *p_data, json &p_jmsg);


        virtual void on_recv(json j);
        string name(){return "ZmqMan";}

    protected:
        inline string make_address(string ip, int port, string protocol){
            stringstream address;
            address << protocol << "://" << ip << ":" << port;
            return address.str();
        }

    private:
        void *zmq_data = NULL;

        string getmode = "callback";
        string m_local_ip;
        string m_remote_ip;
        int m_local_port;
        int m_remote_port;
        int m_num_channels = 1;

};


extern "C" shared_ptr<DataMan> getMan(){
    return shared_ptr<DataMan>(new ZmqMan);
}



#endif
