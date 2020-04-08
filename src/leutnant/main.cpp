#include <iostream>
#include <string>
#include <future>
#include "asio.hpp"

using namespace std;
using namespace asio::ip;

void sender(promise<string>& p){
    cout << "sender starting" << endl;
    string order = p.get_future().get();
    cout << "starting to send: " << order << endl;
}

void reciever(unsigned short port, promise<string>& p){
    asio::io_context ctx;
    tcp::endpoint ep{tcp::v4(), port};
    tcp::acceptor acceptor{ctx, ep};
    acceptor.listen();

    cout << "waiting for order..." << endl;

    tcp::socket sock{ctx};
    acceptor.accept(sock);
    tcp::iostream strm{move(sock)};

    string data;
    strm >> data;
    cout << "order recieved: " << data << endl;
    p.set_value(data);
    strm.close();
}

int main(int argc, char* argv[]){
    if(argc >= 2){
        promise<string> order;
        unsigned short port = stoi(argv[1]);

        thread tsender{sender, ref(order)};
        thread treciever{reciever, port, ref(order)};

        tsender.detach();
        treciever.join();
    }
}