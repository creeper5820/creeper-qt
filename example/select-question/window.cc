#include "window.hh"

#include <creeper-qt/utility/pid.hh>
#include <creeper-qt/widget/basic-shape.hh>
#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/push-button.hh>
#include <qcheckbox.h>
#include <qdebug.h>
#include <qevent.h>
#include <qtimer.h>

using namespace creeper;

class SingleSelectWidget : public RoundedRectangle {
public:
    int index = -1;

    SingleSelectWidget(QWidget* parent = nullptr)
        : RoundedRectangle(parent) {
        setFixedSize(320, 120);

        item0_.setFixedSize(140, 45);
        item0_.setAlignment(Qt::AlignCenter);
        item1_.setFixedSize(140, 45);
        item1_.setAlignment(Qt::AlignCenter);
        item2_.setFixedSize(140, 45);
        item2_.setAlignment(Qt::AlignCenter);
        item3_.setFixedSize(140, 45);
        item3_.setAlignment(Qt::AlignCenter);

        auto grid = new QGridLayout;
        grid->addWidget(&item0_, 0, 0, 1, 1);
        grid->addWidget(&item1_, 0, 1, 1, 1);
        grid->addWidget(&item2_, 1, 0, 1, 1);
        grid->addWidget(&item3_, 1, 1, 1, 1);

        answer_.setFixedSize(140, 45);
        answer_.setVisible(false);
        answer_.setBackground(Theme::color("primary300"));

        setLayout(grid);
        setBackground(Theme::color("primary050"));

        connect(&positionUpdateTimer_, &QTimer::timeout, [this] {
            const auto x = updateWithPid<double>(current_.x(), target_.x(), 0.15);
            const auto y = updateWithPid<double>(current_.y(), target_.y(), 0.15);
            current_ = { x, y };
            answer_.move(current_.x(), current_.y());

            if (std::abs(current_.x() - target_.x()) < 0.01
                && std::abs(current_.y() - target_.y()) < 0.01)
                positionUpdateTimer_.stop();
        });
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() != Qt::LeftButton) return;

        auto x = event->pos().x(), y = event->pos().y();
        if (x > 0 && x < 150 && y > 0 && y < 50) target_ = item0_.pos(), index = 0;
        else if (x > 150 && x < 300 && y > 0 && y < 50) target_ = item1_.pos(), index = 1;
        else if (x > 0 && x < 150 && y > 50 && y < 100) target_ = item2_.pos(), index = 2;
        else if (x > 150 && x < 300 && y > 50 && y < 100) target_ = item3_.pos(), index = 3;

        if (!answer_.isVisible() && target_ != QPoint(0, 0)) answer_.setVisible(true);
        if (!positionUpdateTimer_.isActive()) positionUpdateTimer_.start(1000 / 120);
    }

private:
    Label item0_ { "编程语言" };
    Label item1_ { "程序运行平台" };
    Label item2_ { "操作系统" };
    Label item3_ { "数据库管理系统" };

    RoundedRectangle answer_ { this };
    QPointF target_;
    QPointF current_;
    QTimer positionUpdateTimer_;
};

class MultiSelectWidget : public RoundedRectangle {
public:
    bool theFirstAnswer = false;
    bool theSecondAnswer = false;
    bool theThirdAnswer = false;
    bool theFourthAnswer = false;

    MultiSelectWidget(QWidget* parent = nullptr)
        : RoundedRectangle(parent) {
        setFixedSize(320, 120);

        item0_.setFixedSize(140, 45);
        item0_.setAlignment(Qt::AlignCenter);
        item1_.setFixedSize(140, 45);
        item1_.setAlignment(Qt::AlignCenter);
        item2_.setFixedSize(140, 45);
        item2_.setAlignment(Qt::AlignCenter);
        item3_.setFixedSize(140, 45);
        item3_.setAlignment(Qt::AlignCenter);

        auto grid = new QGridLayout;
        grid->addWidget(&item0_, 0, 0, 1, 1);
        grid->addWidget(&item1_, 0, 1, 1, 1);
        grid->addWidget(&item2_, 1, 0, 1, 1);
        grid->addWidget(&item3_, 1, 1, 1, 1);

        answer0_.setFixedSize(140, 45);
        answer0_.setVisible(false);
        answer0_.setBackground(Theme::color("primary300"));

        answer1_.setFixedSize(140, 45);
        answer1_.setVisible(false);
        answer1_.setBackground(Theme::color("primary300"));

        answer2_.setFixedSize(140, 45);
        answer2_.setVisible(false);
        answer2_.setBackground(Theme::color("primary300"));

        answer3_.setFixedSize(140, 45);
        answer3_.setVisible(false);
        answer3_.setBackground(Theme::color("primary300"));

        setLayout(grid);
        setBackground(Theme::color("primary050"));
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        auto x = event->pos().x(), y = event->pos().y();
        if (event->button() == Qt::LeftButton) {
            if (x > 0 && x < 150 && y > 0 && y < 50)
                answer0_.setVisible(true), theFirstAnswer = true;
            else if (x > 150 && x < 300 && y > 0 && y < 50)
                answer1_.setVisible(true), theSecondAnswer = true;
            else if (x > 0 && x < 150 && y > 50 && y < 100)
                answer2_.setVisible(true), theThirdAnswer = true;
            else if (x > 150 && x < 300 && y > 50 && y < 100)
                answer3_.setVisible(true), theFourthAnswer = true;
        } else if (event->button() == Qt::RightButton) {
            if (x > 0 && x < 150 && y > 0 && y < 50)
                answer0_.setVisible(false), theFirstAnswer = false;
            else if (x > 150 && x < 300 && y > 0 && y < 50)
                answer1_.setVisible(false), theSecondAnswer = false;
            else if (x > 0 && x < 150 && y > 50 && y < 100)
                answer2_.setVisible(false), theThirdAnswer = false;
            else if (x > 150 && x < 300 && y > 50 && y < 100)
                answer3_.setVisible(false), theFourthAnswer = false;
        }

        answer0_.move(item0_.pos());
        answer1_.move(item1_.pos());
        answer2_.move(item2_.pos());
        answer3_.move(item3_.pos());
    }

private:
    Label item0_ { "Click" };
    Label item1_ { "DoubleClick" };
    Label item2_ { "Enter" };
    Label item3_ { "Leave" };

    RoundedRectangle answer0_ { this };
    RoundedRectangle answer1_ { this };
    RoundedRectangle answer2_ { this };
    RoundedRectangle answer3_ { this };
};

struct Window::Impl { };

Window::Window()
    : pimpl_(new Impl)
    , MainWindow() {
    auto singleSelectWidget = new SingleSelectWidget;
    auto multiSelectWidget = new MultiSelectWidget;

    auto label0 = new Label { "Qt Framework 是一种" };
    label0->setAlignment(Qt::AlignCenter);
    auto label1 = new Label { "Button控件的事件有" };
    label1->setAlignment(Qt::AlignCenter);

    auto vertical0 = new QVBoxLayout;
    vertical0->addWidget(label0);
    vertical0->addWidget(singleSelectWidget);
    vertical0->addWidget(label1);
    vertical0->addWidget(multiSelectWidget);

    auto label2 = new Label { "您的答案是" };
    label2->setAlignment(Qt::AlignCenter);

    auto yourAnswer = new Label;
    yourAnswer->setWordWrap(true);
    yourAnswer->setAlignment(Qt::AlignTop);

    auto youAnswerArea = new QHBoxLayout;
    youAnswerArea->addWidget(yourAnswer);

    auto yourAnswerRect = new RoundedRectangle;
    yourAnswerRect->setLayout(youAnswerArea);
    yourAnswerRect->setFixedSize(380, 300);
    yourAnswerRect->setBackground(Theme::color("primary050"));

    auto vertical1 = new QVBoxLayout;
    vertical1->addWidget(label2);
    vertical1->addWidget(yourAnswerRect);

    auto horizontal = new QHBoxLayout;
    horizontal->addLayout(vertical0);
    horizontal->addLayout(vertical1);

    auto button = new PushButton;
    button->setText("确定");
    button->setFixedSize(80, 30);
    connect(
        button, &PushButton::clicked, [this, singleSelectWidget, multiSelectWidget, yourAnswer] {
            auto first = QString {};
            switch (singleSelectWidget->index) {
            case 0:
                first = "编程语言";
                break;
            case 1:
                first = "程序运行平台";
                break;
            case 2:
                first = "操作系统";
                break;
            case 3:
                first = "数据库管理系统";
                break;
            }

            auto second = QString {};
            if (multiSelectWidget->theFirstAnswer) second += "Click";
            if (multiSelectWidget->theSecondAnswer) second += ", DoubleClick";
            if (multiSelectWidget->theThirdAnswer) second += ", Enter";
            if (multiSelectWidget->theFourthAnswer) second += ", Leave";

            yourAnswer->setText(
                "   Qt Framework 是一种" + first + "。Button控件的事件有" + second + "。");
        });

    auto vertical2 = new QVBoxLayout;
    vertical2->addLayout(horizontal);
    vertical2->addLayout(button->horizontalWithSelf());

    auto main = new QWidget;
    main->setLayout(vertical2);
    setCentralWidget(main);
}

Window::~Window() { delete pimpl_; }