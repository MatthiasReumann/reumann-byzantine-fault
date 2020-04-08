#include <iostream>
#include <string>
#include <future>
#include "asio.hpp"
#include "message_utils.h"

using namespace std;
using namespace asio::ip;

class Lieutenant{
    public:
        Lieutenant(unsigned short port, string neighbour_port) : port{port}, neighbour_port{neighbour_port}{}

        void operator()(){
            asio::io_context ctx;
            tcp::endpoint ep{tcp::v4(), this->port};
            tcp::acceptor acceptor{ctx, ep};
            acceptor.listen();

            cout << "waiting for order..." << endl;

            tcp::socket com_sock{ctx};
            acceptor.accept(com_sock);
            string com_order = message_utils::recieve_message(move(com_sock));
            cout << "commanders order is: " << com_order << endl;

            thread t{message_utils::send_message, com_order, this->neighbour_port};
            t.detach();

            tcp::socket l_sock{ctx};
            acceptor.accept(l_sock);
            string l_order = message_utils::recieve_message(move(l_sock));
            cout << "lieutenants order is: " << l_order << endl;
        }
    private:
        unsigned short port;
        string neighbour_port;
};

int main([[maybe_unused]]int argc, char* argv[]){
    unsigned short p = stoi(argv[1]);
    Lieutenant l{p, argv[2]};

    thread treciever{l};

    treciever.join();
}