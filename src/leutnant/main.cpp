#include <iostream>
#include <string>
#include <future>
#include "asio.hpp"
#include "CLI11.hpp"
#include "message_utils.h"

using namespace std;
using namespace asio::ip;
using namespace CLI;

class Lieutenant{
    public:
        Lieutenant(unsigned short port, string neighbour_port, bool isTraitor) : port{port}, neighbour_port{neighbour_port}, isTraitor{isTraitor}{}

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

            if(isTraitor){
                if(com_order == message::ATTACK){
                    com_order = message::RETREAT;
                }else{
                    com_order = message::ATTACK;
                }
            }

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
        bool isTraitor;
};

int main(int argc, char* argv[]){
    bool isTraitor{false};
    unsigned short port;
    string neighbour_port;

    App app{"Byzantine Fault"};
    app.add_flag("-t, --traitor", isTraitor, "Is traitor");
    app.add_option("-p, --port", port, "Lieutnant port");
    app.add_option("-n, --neighbour", neighbour_port, "Neighbour lieutnant port");

    CLI11_PARSE(app, argc, argv);

    Lieutenant l{port, neighbour_port, isTraitor};

    thread treciever{l}; //thread is basically useless here

    treciever.join();
}