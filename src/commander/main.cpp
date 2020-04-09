#include <thread>
#include "CLI11.hpp"
#include "spdlog/spdlog.h"
#include "message_utils.h"

using namespace std;
using namespace CLI;

constexpr char L1_NAME[]{"L1"};
constexpr char L2_NAME[]{"L2"};

void send_order(string order, string lname, string port){
    spdlog::info("Sending order '{}' to {}", order, lname);
    message_utils::send_message(order, port);
}

int main(int argc, char* argv[]) {
    bool isTraitor{false};
    string l1_port, l2_port;

    App app{"Byzantine Fault"};
    app.add_flag("-t, --traitor", isTraitor, "Is traitor");
    app.add_option("-1", l1_port, "Is traitor")->required();
    app.add_option("-2", l2_port, "Is traitor")->required();
    CLI11_PARSE(app, argc, argv);

    spdlog::info("Sending orders to L1 at :{} and L2 at :{}", l1_port, l2_port);

    if(isTraitor){
        spdlog::info("Commander is a traitor");
    }

    thread l1{::send_order, message::ATTACK, L1_NAME, l1_port};
    thread l2{::send_order, isTraitor ? message::RETREAT : message::ATTACK, L2_NAME, l2_port};

    l1.join(),
    l2.join();
}
