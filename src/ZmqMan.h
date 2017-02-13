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
                vector<int> tolerance,
                vector<int> priority);

        void init(
                string local_ip,
                string remote_ip,
                int local_port,
                int remote_port,
                int num_channels,
                vector<int> tolerance,
                vector<int> priority);

        virtual int put(const void *data,
                string doid,
                string var,
                string dtype,
                vector<uint64_t> putshape,
                vector<uint64_t> varshape,
                vector<uint64_t> offset,
                uint64_t timestep,
                int tolerance,
                int priority);

        int get(void *data,
                string doid,
                string var,
                string &dtype,
                vector<uint64_t> &getshape,
                vector<uint64_t> &varshape,
                vector<uint64_t> &offset,
                uint64_t &timestep,
                int &tolerance,
                int &priority){}

        virtual int get(void *data,
                string doid,
                string var,
                string &dtype,
                vector<uint64_t> &varshape,
                uint64_t &timestep
                ){}

        virtual void on_recv(json j);

    protected:
        inline string make_address(string ip, int port, string protocol){
            stringstream address;
            address << protocol << "://" << ip << ":" << port;
            return address.str();
        }

    private:
        void *zmq_data_req = NULL;
        void *zmq_data_rep = NULL;

        string getmode = "callback";
        string m_local_ip;
        string m_remote_ip;
        int m_local_port;
        int m_remote_port;
        int m_num_channels = 1;

};


std::unique_ptr<ZmqMan> getZmqMan(){
    std::unique_ptr<ZmqMan> p(new ZmqMan);
    return std::move(p);
}



