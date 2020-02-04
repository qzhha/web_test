#include <iostream>
#include "net/socket_ops.h"
using namespace std;

class Test  
{
public:
    
    void test_func()
    {
        cout<<"test"<<endl;
    }

};

int main()
{
    int myfd = sockets::createNonblocking(AF_INET);

    std::cout << "Hello world" << std::endl;
    return 0;
}

