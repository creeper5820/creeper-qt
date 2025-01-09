#pragma once

#include <creeper-qt/widget/basic-shape.hh>
#include <creeper-qt/widget/label.hh>
#include <qboxlayout.h>

class AboutUsView : public creeper::RoundedRectangle {
public:
    AboutUsView(QWidget* parent = nullptr)
        : RoundedRectangle(parent) {
        using namespace creeper;
        auto text = new Label(this);
        text->setWordWrap(true);
        text->setAlignment(Qt::AlignCenter);
        text->setFont(QFont("小米兰亭", 10));

        text->setText( //
            "2025年1月 开发\n南京理工大学数据库大作业\n\n"
            "- 选题 -\n长途汽车信息管理系统的设计与实现\n\n"
            "- 作者 - \ncreeper5820");

        auto layout = new QVBoxLayout(this);
        layout->addWidget(text);

        setLayout(layout);
    }
};
