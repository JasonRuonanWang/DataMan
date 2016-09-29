#include<stdio.h>
#include<stdlib.h>
#include"MdtmMan.h"


extern "C" void dataman_write_c(void *data){

    DataMan *man = new MdtmMan("127.0.0.1", "127.0.0.1", "send");
    man->put(data, "aaa", "bbb", 0,0,0);
    delete man;
}

extern "C" void dataman_init_c(){



}

extern "C" void dataman_open_c(){
}





