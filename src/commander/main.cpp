#include <iostream>
#include <thread>
#include <string>
#include "asio.hpp"

using namespace std;
using namespace asio::ip;

void send_message(string port){
    tcp::iostream strm{"localhost", port};
    string data;
    if(strm){
        strm << "ANGRIFF" << endl;
        strm.close();
    }else{
        cerr << "no connection" << endl;
    }
    cout << port << endl;
}

int main() {
    thread l1{send_message, "10000"};
    thread l2{send_message, "10001"};
    thread l3{send_message, "10002"};

    l1.join(),
    l2.join();
    l3.join();
}
