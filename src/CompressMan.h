#ifndef COMPRESSMAN_H_
#define COMPRESSMAN_H_


#include "DataMan.h"

class CompressMan : public DataMan{
    public:
        CompressMan();
        ~CompressMan();
        virtual string type(){return "Compress";}
};



#endif
