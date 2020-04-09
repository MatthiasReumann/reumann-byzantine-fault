#pragma once 
#include <iostream>
#include <string>
#include "spdlog/spdlog.h"
#include "asio.hpp"

typedef std::string Order;

namespace message_utils{
    void send_message(std::string msg, std::string port){
        asio::ip::tcp::iostream strm{"localhost", port};
        if(strm){
            strm << msg << std::endl;
            strm.close();
        }else{
            std::cerr << "no connection" << std::endl;
        }
    }

    std::string recieve_message(asio::ip::tcp::socket sock){
        asio::ip::tcp::iostream strm{std::move(sock)};
        std::string data;
        strm >> data;
        strm.close();
        return data;
    }
}

namespace message{
    constexpr char ATTACK[]{"ANGRIFF"};
    constexpr char RETREAT[]{"RÜCKZUG"};
}