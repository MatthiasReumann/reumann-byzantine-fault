#include <thread>
#include "CLI11.hpp"
#include "spdlog/spdlog.h"
#include "message_utils.h"
#include "generals.h"

using namespace std;
using namespace CLI;

void send_order(Order order, string lname, string port){
    spdlog::info("Sending order '{}' to {}", order, lname);
    message_utils::send_message(order, port);
}

int main(int argc, char* argv[]) {
    bool isTraitor{false};

    App app{"Byzantine Fault"};
    app.add_flag("-t, --traitor", isTraitor, "Is traitor");
    CLI11_PARSE(app, argc, argv);

    spdlog::info("Sending orders to L1 at :{} and L2 at :{}", L1_PORT, L2_PORT);

    if(isTraitor){
        spdlog::info("Commander is a traitor");
    }

    thread l1{::send_order, message::ATTACK, L1_NAME, L1_PORT};
    thread l2{::send_order, isTraitor ? message::RETREAT : message::ATTACK, L1_NAME, L2_PORT};

    l1.join(),
    l2.join();
}
