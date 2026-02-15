#pragma once
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper::widget::internal {

class Widget : public QWidget { };

}
namespace creeper {
using Widget = Declarative<widget::internal::Widget, widget::pro::checker>;
}
