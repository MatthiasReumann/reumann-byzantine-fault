#include <thread>
#include "CLI11.hpp"
#include "message_utils.h"

using namespace std;
using namespace CLI;

int main(int argc, char* argv[]) {
    bool isTraitor{false};

    App app{"Byzantine Fault"};
    app.add_flag("-t, --traitor", isTraitor, "Is traitor");
    CLI11_PARSE(app, argc, argv);

    thread l1{message_utils::send_message, message::ATTACK, "10000"};
    thread l2{message_utils::send_message, isTraitor ? message::RETREAT : message::ATTACK, "10001"};

    l1.join(),
    l2.join();

}
