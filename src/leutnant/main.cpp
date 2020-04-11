#include <iostream>
#include <string>
#include <vector>
#include "spdlog/spdlog.h"
#include "asio.hpp"
#include "CLI11.hpp"
#include "message_utils.h"

using namespace std;
using namespace asio::ip;
using namespace CLI;

class Lieutenant {
    public:
        Lieutenant(unsigned short port, string neighbour_port, bool isTraitor) : port{port}, 
            neighbour_port{neighbour_port}, isTraitor{isTraitor} {}

        void start() {
            asio::io_context ctx;
            tcp::endpoint ep{tcp::v4(), this->port};
            tcp::acceptor acceptor{ctx, ep};
            acceptor.listen();
            spdlog::info("Waiting for order...");

            tcp::socket com_sock{ctx};
            acceptor.accept(com_sock);
            string com_order = message_utils::recieve_message(move(com_sock));

            this->votes.push_back(com_order);

            spdlog::info("Commander sent '{}' order", com_order);

            if(isTraitor){
                if(com_order == message::ATTACK){
                    com_order = message::RETREAT;
                }else{
                    com_order = message::ATTACK;
                }
            }
            
            spdlog::info("Sending '{}' order to neighbour", com_order);
            thread t{message_utils::send_message, com_order, this->neighbour_port};
            t.detach();

            spdlog::info("Waiting for neigbour's order...");

            tcp::socket l_sock{ctx};
            acceptor.accept(l_sock);
            string l_order = message_utils::recieve_message(move(l_sock));

            this->votes.push_back(l_order);

            spdlog::info("Neighbours order is '{}'", l_order);

            this->majority();
        }

    private:
        void majority() {
            if(isTraitor) {
                spdlog::info("Lieutnant is a traitor. The majority doesn't matter.");
            }else{
                spdlog::info("Picking the majority of:");
                for(auto vote : votes) {
                    spdlog::info(vote);
                }
            }
        }
        
        vector<string> votes;
        unsigned short port;
        string neighbour_port;
        bool isTraitor;
};

int main(int argc, char* argv[]) {
    bool isTraitor{false};
    unsigned short port;
    string neighbour_port;

    App app{"Byzantine Fault"};
    app.add_flag("-t, --traitor", isTraitor, "Is traitor");
    app.add_option("-p, --port", port, "Lieutnant port")->required();
    app.add_option("-n, --neighbour", neighbour_port, "Neighbour lieutnant port")->required();

    CLI11_PARSE(app, argc, argv);

    Lieutenant l{port, neighbour_port, isTraitor};
    l.start();
}