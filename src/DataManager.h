#include"DataMan.h"
#include<string>
#include<vector>
using namespace std;


class DataManager: public DataMan{
    public:
        DataManager(
                string p_local_ip,
                string p_remote_ip,
                int p_local_port,
                int p_remote_port,
                int p_num_channels,
                vector<int> p_tolerance,
                vector<int> p_priority
                );
        ~DataManager();

        int put(void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<uint64_t> p_putshape,
                vector<uint64_t> p_varshape,
                vector<uint64_t> p_offset,
                uint64_t p_timestep,
                int p_tolerance,
                int p_priority
                );

        int get(void *p_data,
                string p_doid,
                string p_var,
                string p_dtype,
                vector<uint64_t> p_getshape,
                vector<uint64_t> p_varshape,
                vector<uint64_t> p_offset,
                uint64_t p_timestep
                );

        int get(void *p_data,
                string p_doid,
                string p_var,
                string &p_dtype,
                vector<uint64_t> &p_varshape,
                uint64_t &p_timestep
                );
        void flush();

    private:
        void init();

        string m_local_ip="";
        string m_remote_ip="";
        int m_local_port=0;
        int m_remote_port=0;
        int m_num_channels=0;
        vector<int> m_tolerance;
        vector<int> m_priority;
        string m_filename="";


};


