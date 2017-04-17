#ifndef COMPRESSMAN_H_
#define COMPRESSMAN_H_


#include "DataMan.h"

using namespace std;

namespace adios
{
namespace realtime
{

class CompressMan : public DataManBase{
    public:
        CompressMan() = default;
        ~CompressMan() = default;
        virtual string type(){return "Compress";}
};

} // end namespace realtime
} // end namespace adios


#endif
