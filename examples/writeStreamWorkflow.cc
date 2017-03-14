#include <DataManager.h>

using namespace std;

int main(){

    DataManager man;
    json msg;
    msg["method"] = "dump";
    man.add_stream(msg);

    json msg2;
    msg2["putshape"] = {1,2,5};
    msg2["varshape"] = {4,6,10};
    msg2["doid"] = "aaa";
    msg2["var"] = "data";
    msg2["dtype"] = "float";

    int datasize = 4*6*10;
    float data[datasize];
    for (int i=0; i<4; i++){
        for (int j=0; j<3; j++){
            for (int k=0; k<2; k++){
                for (int m=0; m<datasize; m++){
                    data[m] = i*60 + j*10 + m;
                }
                msg2["offset"] = {i,j*2,k*5};
                man.put(data, msg2);
            }
        }
    }
    man.flush();
    return 0;
}


