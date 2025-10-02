#include "creeper-qt/utility/painter/helper.hh"
#include <qdebug.h>

namespace creeper::util {

constexpr auto enable_print_paint_event_count = bool { false };

auto print_paint_event_count() noexcept -> void {
    if constexpr (enable_print_paint_event_count) {
        static auto count = std::size_t { 0 };
        qDebug() << "[PainterHelper] Paint Event:" << count++;
    }
}

}
