#include <thread>
#include "message_utils.h"

using namespace std;

int main() {
    thread l1{message_utils::send_message, message::ATTACK, "10000"};
    thread l2{message_utils::send_message, message::RETREAT, "10001"};

    l1.join(),
    l2.join();
}
