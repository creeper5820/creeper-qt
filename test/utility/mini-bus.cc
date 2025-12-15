#include <creeper-qt/core/application.hh>
#include <creeper-qt/utility/mini-bus.hh>
#include <print>

struct ReceiverMock : QObject {
    std::string name;

    ReceiverMock(std::string name)
        : name { name } { }

    auto receive(int num) noexcept -> void {
        // ...
        std::println("[{}] Received {} and then update", name, num);
    }
};

auto main(int argc, char* argv[]) -> int {
    using namespace creeper;

    app::init {
        app::pro::Complete { argc, argv },
    };

    auto mock0 = ReceiverMock { "0" };
    mini_bus<int>.append_receiver(&mock0);

    std::println("[Broadcast] 0");
    mini_bus<int>.broadcast(0);

    {
        auto mock1 = ReceiverMock { "1" };
        mini_bus<int>.append_receiver(&mock1);

        std::println("[Broadcast] 1");
        mini_bus<int>.broadcast(1);
    }

    std::println("[Broadcast] 2");
    mini_bus<int>.broadcast(2);

    app::quit();
}
