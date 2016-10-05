#include<stdio.h>
#include<stdlib.h>
#include"MdtmMan.h"
#include"api.h"



extern "C" void dataman_write_c(void *data){

    DataMan *man = new MdtmMan("tcp://127.0.0.1:12306", "tcp://127.0.0.1:12307", "sender");
    man->put(data, "aaa", "bbb", 0,0,0);
    delete man;
}

extern "C" void dataman_init_c(){

}

extern "C" void dataman_open_c(){
}





