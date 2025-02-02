#pragma once

#include <qboxlayout.h>
#include <qfile.h>
#include <qwidget.h>
#include <type_traits>

#include "creeper-qt/setting/theme.hh"

/// @brief 一切都是为了链式调用!
#define CREEPER_WIDGET_OVERRIDE_INVOKE_METHOD(FinalWidget, OriginWidget)                           \
public:                                                                                            \
    template <typename... Args> static FinalWidget& create(Args... args) {                         \
        return *(new FinalWidget(std::forward<Args>(args)...));                                    \
    }                                                                                              \
    [[nodiscard]] FinalWidget* build() { return this; }                                            \
    FinalWidget& setMinimumSize(const QSize& size) {                                               \
        OriginWidget::setMinimumSize(size);                                                        \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setMinimumSize(int w, int h) {                                                    \
        OriginWidget::setMinimumSize(w, h);                                                        \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setMaximumSize(const QSize& size) {                                               \
        OriginWidget::setMaximumSize(size);                                                        \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setMaximumSize(int w, int h) {                                                    \
        OriginWidget::setMaximumSize(w, h);                                                        \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setMinimumWidth(int w) {                                                          \
        OriginWidget::setMinimumWidth(w);                                                          \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setMinimumHeight(int h) {                                                         \
        OriginWidget::setMinimumHeight(h);                                                         \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setMaximumWidth(int w) {                                                          \
        OriginWidget::setMaximumWidth(w);                                                          \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setSizeIncrement(const QSize& size) {                                             \
        OriginWidget::setSizeIncrement(size);                                                      \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setSizeIncrement(int w, int h) {                                                  \
        OriginWidget::setSizeIncrement(w, h);                                                      \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setBaseSize(const QSize& size) {                                                  \
        OriginWidget::setBaseSize(size);                                                           \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setBaseSize(int w, int h) {                                                       \
        OriginWidget::setBaseSize(w, h);                                                           \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setFixedSize(const QSize& size) {                                                 \
        OriginWidget::setFixedSize(size);                                                          \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setFixedSize(int w, int h) {                                                      \
        OriginWidget::setFixedSize(w, h);                                                          \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setFixedWidth(int w) {                                                            \
        OriginWidget::setFixedWidth(w);                                                            \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setFixedHeight(int h) {                                                           \
        OriginWidget::setFixedHeight(h);                                                           \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setBackgroundRole(QPalette::ColorRole role) {                                     \
        OriginWidget::setBackgroundRole(role);                                                     \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setForegroundRole(QPalette::ColorRole role) {                                     \
        OriginWidget::setForegroundRole(role);                                                     \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setFont(const QFont& font) {                                                      \
        OriginWidget::setFont(font);                                                               \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setMask(const QBitmap& bitmap) {                                                  \
        OriginWidget::setMask(bitmap);                                                             \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setMask(const QRegion& region) {                                                  \
        OriginWidget::setMask(region);                                                             \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& clearMask() {                                                                     \
        OriginWidget::clearMask();                                                                 \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setWindowIcon(const QIcon& icon) {                                                \
        OriginWidget::setWindowIcon(icon);                                                         \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setWindowIconText(const QString& text) {                                          \
        OriginWidget::setWindowIconText(text);                                                     \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setWindowRole(const QString& role) {                                              \
        OriginWidget::setWindowRole(role);                                                         \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setWindowFilePath(const QString& filePath) {                                      \
        OriginWidget::setWindowFilePath(filePath);                                                 \
        return *this;                                                                              \
    }                                                                                              \
    FinalWidget& setWindowOpacity(qreal level) {                                                   \
        OriginWidget::setWindowOpacity(level);                                                     \
        return *this;                                                                              \
    }

namespace creeper {

template <class Widget>
    requires std::is_convertible_v<Widget*, QWidget*>
class Extension : public Widget {
public:
    template <typename... Args>
    explicit Extension(Args... args)
        : Widget(std::forward<Args>(args)...) {
        Theme::addReloadThemeHandler(this, [this] {
            if (autoReloadTheme_) reloadTheme();
        });
    }

    ~Extension() { Theme::removeReloadThemeHandler(this); }

    void moveCenter() {
        if (widget_->parentWidget() == nullptr) return;

        const auto parentCenter = widget_->parentWidget()->geometry().center();
        const auto frameCenter = widget_->frameGeometry().center();

        widget_->move(parentCenter - frameCenter);
    }

    void loadStyleFromFile(const QString& file) {
        QFile styleFile(file);
        styleFile.open(QFile::ReadOnly | QFile::Text);

        const auto style = styleFile.readAll();
        widget_->setStyleSheet(style);

        widget_->ensurePolished();
    }

    auto verticalWithSelf() {
        if (vertical_ == nullptr) {
            vertical_ = new QVBoxLayout;
            vertical_->setAlignment(Qt::AlignCenter);
            vertical_->addWidget(widget_);
        }
        return vertical_;
    }

    auto horizontalWithSelf() {
        if (horizontal_ == nullptr) {
            horizontal_ = new QHBoxLayout;
            horizontal_->setAlignment(Qt::AlignCenter);
            horizontal_->addWidget(widget_);
        }
        return horizontal_;
    }

    void setRefreshIntervalMs(int ms) { refreshIntervalMs_ = ms; }

    virtual void reloadTheme() = 0;
    void disableAutoTheme() { autoReloadTheme_ = false; }
    void enableAutoTheme() { autoReloadTheme_ = true; }

protected:
    int refreshIntervalMs_ = 10;
    bool autoReloadTheme_ = true;

private:
    QWidget* widget_ = static_cast<QWidget*>(this);

    QVBoxLayout* vertical_ = nullptr;
    QHBoxLayout* horizontal_ = nullptr;
};

/// @brief 快速重载 reloadTheme 的辅助类，适用于没有继承 Extension 的类
template <typename Widget> class QuickAutoTheme : public Extension<Widget> {
    CREEPER_WIDGET_OVERRIDE_INVOKE_METHOD(QuickAutoTheme, Extension<Widget>);

public:
    template <typename... Args>
    explicit QuickAutoTheme(std::function<void(QuickAutoTheme&)> handler, Args&&... args)
        : Extension<Widget>(std::forward<Args>(args)...)
        , handler_(handler) {
        reloadTheme();
    }
    void reloadTheme() override { handler_(*this); }

private:
    std::function<void(QuickAutoTheme&)> handler_;
};
}
