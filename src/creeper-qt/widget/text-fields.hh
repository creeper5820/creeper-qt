#pragma once

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <qlineedit.h>

namespace creeper {

class FilledTextField;
class OutlinedTextField;

class BasicTextField : public QLineEdit, public DSL {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(BasicTextField);

    friend FilledTextField;
    friend OutlinedTextField;

public:
    struct ColorSpecs {
        struct Tokens {
            QColor container;
            QColor caret;
            QColor active_indicator;

            QColor input_text;
            QColor label_text;
            QColor supporting_text;

            QColor leading_icon;
            QColor trailing_icon;

            QColor outline;
        };

        Tokens enabled;
        Tokens disabled;
        Tokens focused;
        Tokens error;

        QColor state_layer;
        QColor selection_container;
    };

    struct Measurements {
        int container_height = 56;

        int icon_rect_size  = 24;
        int input_rect_size = 24;
        int label_rect_size = 16;

        int standard_font_height = 16;

        int col_padding                      = 8;
        int row_padding_without_icons        = 16;
        int row_padding_with_icons           = 12;
        int row_padding_populated_label_text = 4;

        int padding_icons_text = 16;

        int supporting_text_and_character_counter_top_padding = 4;
        int supporting_text_and_character_counter_row_padding = 16;

        auto iconSize() const { return QSize { icon_rect_size, icon_rect_size }; }

        friend auto dsl_invoke(BasicTextField& self, const Measurements& measurements) -> void {
            self.setMeasurements(measurements);
        }
    };

    explicit BasicTextField(auto&&... props)
        : BasicTextField {} {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    void loadColorScheme(const ColorScheme&);

    void bindThemeManager(ThemeManager&);

    void setLabelText(const QString&);

    void setHintText(const QString&);

    void setSupportingText(const QString&);

    void setLeadingIcon(const QIcon&);

    void setLeadingIcon(const QString& code, const QString& font);

    void setTrailingIcon(const QIcon&);

    void setTrailingIcon(const QString& code, const QString& font);

    auto setMeasurements(const Measurements& measurements) noexcept -> void;

Q_SIGNALS:
    auto pressed() -> void;

protected:
    void resizeEvent(QResizeEvent*) override;

    void enterEvent(qt::EnterEvent*) override;
    void leaveEvent(QEvent*) override;

    void focusInEvent(QFocusEvent*) override;
    void focusOutEvent(QFocusEvent*) override;

    void mousePressEvent(QMouseEvent*) override;
};

class FilledTextField : public BasicTextField {
public:
    explicit FilledTextField(auto&&... props)
        : BasicTextField {} {
        construct_with(std::forward<decltype(props)>(props)...);
    }

protected:
    void paintEvent(QPaintEvent*) override;
};

class OutlinedTextField : public BasicTextField {
public:
    explicit OutlinedTextField(auto&&... props)
        : BasicTextField {} {
        construct_with(std::forward<decltype(props)>(props)...);
    }

protected:
    void paintEvent(QPaintEvent*) override;
};

}
namespace creeper::text_field::pro {

using namespace common::pro;
using namespace widget::pro;
using namespace theme::pro;

using ClearButton = ForwardProp<&QLineEdit::setClearButtonEnabled>;

using Measurements = ForwardProp<&BasicTextField::setMeasurements>;

using LabelText =
    common::pro::String<[](auto& self, const auto& text) { self.setLabelText(text); }>;

using Text = common::pro::Text;

using ReadOnly = ForwardProp<&QLineEdit::setReadOnly>;

struct LeadingIcon {
    QString code;
    QString font;
    explicit LeadingIcon(const QString& code, const QString& font)
        : code { code }
        , font { font } { }
    friend auto dsl_invoke(BasicTextField& self, const LeadingIcon& prop) -> void {
        self.setLeadingIcon(prop.code, prop.font);
    }
};

template <typename F>
using OnTextChanged = common::pro::SignalInjection<F, &BasicTextField::textChanged>;

template <typename F>
using OnEditingFinished = common::pro::SignalInjection<F, &BasicTextField::editingFinished>;

template <typename F>
using OnChanged = OnTextChanged<F>;

template <typename F>
using OnPressed = common::pro::SignalInjection<F, &BasicTextField::pressed>;

}
