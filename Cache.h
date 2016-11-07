#include <vector>
using namespace std;

class Cache{

    public:
        Cache();
        ~Cache();

    private:
        vector<void *> buffer;


};
