#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/widget.hh>

class Login : public creeper::Extension<QWidget> {
    Q_OBJECT
    CREEPER_WIDGET_PIMPL_DEFINTION(Login)
public:
    void setButtonText(const QString& text);
    void reloadTheme() override;

signals:
    void sendLoginRequest(const QString& username, const QString& password);
};