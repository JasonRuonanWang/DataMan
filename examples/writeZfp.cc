#include <utilities/realtime/dataman/DataMan.h>
#include <dlfcn.h>

using namespace std;
using namespace adios::realtime;

int main(){

    void *so = NULL;
    so = dlopen("libzfpman.so",RTLD_NOW);
    if (so == NULL){
        std::cout << "so=NULL\n";
    }
    std::unique_ptr<DataMan> (*func)() = NULL;
    func = (std::unique_ptr<DataMan>(*)()) dlsym(so,"getMan");
    if (func == NULL){
        std::cout << "func=NULL\n";
    }

    std::unique_ptr<DataMan> man = func();

    vector<size_t> putshape;
    putshape.assign(3,0);

    vector<size_t> varshape;
    varshape.push_back(0);
    varshape.push_back(0);
    varshape.push_back(0);

    vector<size_t> offset;
    offset.push_back(0);
    offset.push_back(0);
    offset.push_back(0);

    varshape[0] = 2;
    varshape[1] = 4;
    varshape[2] = 60;

    putshape=varshape;

    int datasize = varshape[0] * varshape[1] * varshape[2];

    float data[datasize];

    for (int k=0; k<datasize; k++){
        data[k] = exp(-(0.01*k));
    }
    man->put(data, "aaa", "data", "float", putshape, varshape, offset, 0, 0, 100);
    man->flush();
    return 0;
}


