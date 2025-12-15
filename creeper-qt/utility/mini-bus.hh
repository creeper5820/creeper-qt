#pragma once
#include "creeper-qt/utility/wrapper/common.hh"

#include <qwidget.h>

namespace creeper {

template <typename MessageT>
class MiniBus {
public:
    template <class T>
    static auto append_receiver(T* receiver) noexcept {

        static_assert(
            requires { receiver->receive(std::declval<MessageT>()); },
            "Receiver should has function: receive(const MessageT&)");

        static_assert(std::derived_from<T, QObject>, "Receiver should be derived from QObject");

        constexpr auto notify_function = [](QObject* receiver, const MessageT& msg) {
            static_cast<T*>(receiver)->receive(msg);
        };
        actions.emplace_back(Action {
            .receiver = receiver,
            .notify   = notify_function,
        });

        QObject::connect(receiver, &QObject::destroyed, [key = receiver] {
            std::erase_if(actions, [=](const Action& action) { return action.receiver == key; });
        });
    }

    static auto receiver_count() noexcept { return actions.size(); }

    static auto broadcast(const MessageT& msg) noexcept {
        std::ranges::for_each(
            actions, [&](const Action& action) { action.notify(action.receiver, msg); });
    }

private:
    struct Action {
        QObject* receiver;
        auto (*notify)(QObject*, const MessageT&) -> void;
    };
    static inline std::vector<Action> actions {};
};

template <typename MessageT>
inline auto mini_bus = MiniBus<MessageT> {};

}
namespace creeper::bus::pro {

using Token = common::Token<MiniBus<void>>;

template <typename MessageT>
struct Subscribe : Token {
    static auto apply(auto& self) noexcept {
        // ...
        mini_bus<MessageT>.append_receiver(&self);
    }
};

template <class T>
concept trait = std::derived_from<T, Token>;
CREEPER_DEFINE_CHECKER(trait)

}
