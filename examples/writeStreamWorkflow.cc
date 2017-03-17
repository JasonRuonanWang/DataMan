#include <DataManager.h>

using namespace std;

int main(){

    DataManager man;
    json msg;
    msg["method"] = "dump";
    man.add_stream(msg);

    vector<size_t> varshape = {4,6,10};
    json msg2;
    msg2["varshape"] = varshape;
    msg2["doid"] = "aaa";
    msg2["var"] = "data";
    msg2["dtype"] = "float";

    size_t datasize = 0;
    for(size_t i=0; i<varshape.size(); i++){
        datasize += i;
    }
    vector<float> data(datasize,0);

    man.put(data.data(), msg2);

    man.flush();
    return 0;
}

