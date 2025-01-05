#include "login.hh"

#include <creeper-qt/widget/basic-shape.hh>
#include <creeper-qt/widget/image.hh>
#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/line-edit.hh>
#include <creeper-qt/widget/push-button.hh>

#include <creeper-qt/module/round-icon-button.hh>

using namespace creeper;

struct Login::Impl {
public:
    Impl() {
        loginButton.setText("登陆");
        loginButton.setFont(QFont(Theme::font("text"), 10));
        loginButton.setRadiusRatio(0.5);
        loginButton.setFixedSize(300, 40);

        welcome.setText("嗨! 别来无恙");
        welcome.setAlignment(Qt::AlignCenter);
        welcome.setFont(QFont(Theme::font("text"), 18));
        welcome.setFixedSize(300, 40);

        username.setPlaceholderText("请输入用户名");
        username.setFont(QFont(Theme::font("text"), 10));
        username.setFixedSize(300, 40);
        username.setIcon(QIcon(":image/username.png"));

        password.setPlaceholderText("请输入密码");
        password.setFont(QFont(Theme::font("text"), 10));
        password.setFixedSize(300, 40);
        password.setEchoMode(QLineEdit::Password);
        password.setIcon(QIcon(":/image/password.png"));

        details.setAlignment(Qt::AlignCenter);
        details.setText(
            "Welcome to the car management system for the long-distance transportation. ");
        details.setFont(QFont("JetBrains Mono", 8));
        details.setFixedSize(300, 60);
        details.setWordWrap(true);
        details.setStyleSheet("QLabel { color:rgb(94, 94, 94); }");

        forgetPassword.setText("Forget password?");
        forgetPassword.setFont(QFont("JetBrains Mono", 8));
        forgetPassword.setStyleSheet("QLabel { color:rgb(94, 94, 94); }");
        forgetPassword.setFixedSize(160, 30);
        forgetPassword.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        forgetPassword.disableBackground();

        divider.setText("——————— other methods ———————");
        divider.setAlignment(Qt::AlignCenter);
        divider.setFont(QFont("JetBrains Mono", 8));
        divider.setStyleSheet("QLabel { color:rgb(94, 94, 94); }");
        divider.setFixedSize(300, 30);

        googleLogin.setRadius(20);
        googleLogin.setIconRatio(1.2);
        googleLogin.setIcon(QIcon(":/image/google.png"));

        githubLogin.setRadius(20);
        githubLogin.setIconRatio(1.2);
        githubLogin.setIcon(QIcon(":/image/github.png"));

        schoolLogin.setRadius(20);
        schoolLogin.setIconRatio(1.2);
        schoolLogin.setIcon(QIcon(":/image/school.png"));

        auto otherMethods = new QHBoxLayout;
        otherMethods->setAlignment(Qt::AlignRight);
        otherMethods->addWidget(&googleLogin);
        otherMethods->addWidget(&githubLogin);
        otherMethods->addWidget(&schoolLogin);

        auto layout = new QGridLayout;
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(15);
        layout->addWidget(&welcome, 0, 0, 1, 6);
        layout->addWidget(&details, 1, 0, 1, 6);
        layout->addWidget(&username, 2, 0, 1, 6);
        layout->addWidget(&password, 3, 0, 1, 6);
        layout->addWidget(&forgetPassword, 4, 2, 1, 4);
        layout->addLayout(loginButton.horizontalWithSelf(), 5, 0, 1, 6);
        layout->addWidget(&divider, 6, 0, 1, 6);
        layout->addLayout(otherMethods, 7, 1, 1, 4);

        background.setRadius(10);
        background.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        background.setLayout(layout);
    }

    RoundedRectangle background;
    PushButton loginButton;
    LineEdit username;
    LineEdit password;
    Image image;

private:
    PushButton forgetPassword;

    Label welcome;
    Label details;
    Label divider;

    RoundIconButton googleLogin;
    RoundIconButton githubLogin;
    RoundIconButton schoolLogin;
};

Login::Login(QWidget* parent)
    : pimpl_(new Impl) {
    pimpl_->image.setPixmap(":/image/雪原城堡.png");
    pimpl_->image.setFitness(ImageFitness::Cover);
    pimpl_->image.setRadius(10);
    pimpl_->image.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(30);
    layout->addWidget(&pimpl_->image, 0, 0, 1, 5);
    layout->addWidget(&pimpl_->background, 0, 5, 1, 4);

    connect(&pimpl_->loginButton, &PushButton::clicked,
        [this] { emit sendLoginRequest(pimpl_->username.text(), pimpl_->password.text()); });

    setLayout(layout);
}

Login::~Login() { delete pimpl_; }

void Login::reloadTheme() { }