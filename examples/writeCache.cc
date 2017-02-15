#include <unistd.h>
#include <iostream>
#include <vector>

#include <CacheMan.h>

using namespace std;

int main(){

    CacheMan man;

    vector<uint64_t> putshape;
    putshape.assign(3,0);

    vector<uint64_t> varshape;
    varshape.push_back(0);
    varshape.push_back(0);
    varshape.push_back(0);

    vector<uint64_t> offset;
    offset.push_back(0);
    offset.push_back(0);
    offset.push_back(0);

    varshape[0] = 4;
    varshape[1] = 6;
    varshape[2] = 10;

    putshape[0] = 1;
    putshape[1] = 2;
    putshape[2] = 5;

    int datasize = 10;

    float data[datasize];

    for (int i=0; i<4; i++){
        for (int j=0; j<3; j++){
            for (int k=0; k<2; k++){
                for (int m=0; m<datasize; m++){
                    data[m] = i*10000 + j*100 + k*10 + m;
                }
                offset[0] = i;
                offset[1] = j*2;
                offset[2] = k*5;
                man.put(data, "aaa", "data", "float", putshape, varshape, offset, 0, 0, 100);

                for (int i=0; i<10; i++)
                    cout << ((float*)data)[i] << " ";
                cout << endl;
            }
        }
    }
    man.flush();
    return 0;
}

