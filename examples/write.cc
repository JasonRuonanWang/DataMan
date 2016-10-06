#include <DataManager.h>
#include <unistd.h>


int main(){

    float data[1024];

    unsigned int shape[3] = {2,256,4};

    for (int i=0; i<10; i++){
        dataman_write(data, "aaa", "bbb", "float", shape, 0, 0);
        usleep(1000);
    }

    return 0;
}
