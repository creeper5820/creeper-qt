#include "window.hh"

#include <qcheckbox.h>
#include <qdebug.h>
#include <qevent.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qrandom.h>
#include <qtimer.h>

#include <ranges>

using namespace creeper;

class Ring : public QWidget {
public:
    explicit Ring(QWidget* parent = nullptr)
        : QWidget(parent) { }

    int radius() const { return width() / 2; }
    void setRadius(int radius) { setFixedSize(radius * 2, radius * 2); }

    int ringWidth() const { return ringWidth_; }
    void setRingWidth(int width) { ringWidth_ = width; }

    QColor color() const { return color_; }
    void setColor(QColor color) { color_ = QColor(color); }

    double opacity() const { return opacity_; }
    void setOpacity(double opacity) { opacity_ = opacity; }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setOpacity(opacity_);
        painter.setPen(Qt::NoPen);
        painter.setBrush(color_);

        auto path = QPainterPath {};
        path.addEllipse(ringWidth_, ringWidth_, width() - 2 * ringWidth_, width() - 2 * ringWidth_);
        path.addEllipse(rect());

        painter.drawPath(path);
    }

private:
    int ringWidth_;
    QColor color_;
    double opacity_ = 0;
};

class ColorLabel : public QLabel {
public:
    explicit ColorLabel(QWidget* parent = nullptr)
        : QLabel(parent) { }

    QColor color() const { return color_; }
    void setColor(QColor color) {
        color_ = color, setStyleSheet(QString("color: %1").arg(color.name()));
    }

private:
    QColor color_;
};

struct Window::Impl {
    ColorLabel word[4];
    Ring ring[4];
    double opacity[4];

    ColorLabel countLabel;
    size_t count = 0;

    QTimer timer;

    void randomColor(ColorLabel& label) {
        int hue = QRandomGenerator::global()->bounded(0, 360);
        float saturation = static_cast<float>(QRandomGenerator::global()->bounded(20, 40)) / 100.0;
        float value = static_cast<float>(QRandomGenerator::global()->bounded(70, 90)) / 100.0;
        QColor color;
        color.setHsv(hue, saturation * 255, value * 255);
        label.setStyleSheet(QString("color: %1").arg(color.name()));
        label.setColor(color);
    }

    void randomWord(QLabel& label) {
        constexpr int bornWidth = 760;
        constexpr int fontWidth = 50;
        constexpr int margin = 20;
        bool overlap = true;
        int randomX;
        while (overlap) {
            overlap = false;
            randomX = QRandomGenerator::global()->bounded(margin, bornWidth - fontWidth);
            for (const auto& word : word)
                if (std::abs(randomX - word.pos().x()) < fontWidth) {
                    overlap = true;
                    break;
                }
        }
        label.setText(QString("%1").arg(char(QRandomGenerator::global()->bounded('A', 'Z' + 1))));
        label.move(randomX, -200);
    }
};

Window::Window()
    : pimpl_(new Impl)
    , MainWindow() {

    auto& count = pimpl_->countLabel;
    count.setParent(this);
    count.move(730, 130);
    count.setText("0");
    count.setFont(QFont("Arial", 30, QFont::Bold));

    for (auto& word : pimpl_->word) {
        word.setFont(QFont("Arial", 30, QFont::Bold));
        word.setParent(this);
        pimpl_->randomWord(word);
        pimpl_->randomColor(word);
        word.move(word.pos().x(), word.pos().y() - std::rand() % 200);
    }

    for (auto& ring : pimpl_->ring) {
        ring.setParent(this);
        ring.setColor(Theme::color("primary100"));
        ring.setRadius(10);
        ring.setRingWidth(6);
    }

    connect(&pimpl_->timer, &QTimer::timeout, [this] {
        for (auto& word : pimpl_->word)
            word.move(word.pos().x(), word.pos().y() + 1);

        if (pimpl_->word[0].pos().y() > 200) pimpl_->randomWord(pimpl_->word[0]);
        if (pimpl_->word[1].pos().y() > 200) pimpl_->randomWord(pimpl_->word[1]);
        if (pimpl_->word[2].pos().y() > 200) pimpl_->randomWord(pimpl_->word[2]);
        if (pimpl_->word[3].pos().y() > 200) pimpl_->randomWord(pimpl_->word[3]);

        for (auto& ring : pimpl_->ring) {
            if (ring.opacity() <= 0) continue;
            ring.setOpacity(ring.opacity() - 0.05);
            ring.setRadius(ring.radius() + 10);
            ring.setRingWidth(ring.ringWidth() + 3);
            ring.move(ring.pos().x() - 10, ring.pos().y() - 10);
            ring.update();
        }
    });
    pimpl_->timer.start(10);
}

Window::~Window() { delete pimpl_; }

void Window::keyPressEvent(QKeyEvent* event) {
    const auto key = event->key();
    for (auto&& [word, ring, opacity] :
        std::views::zip(pimpl_->word, pimpl_->ring, pimpl_->opacity)) {
        if (word.text()[0].unicode() != key) continue;
        ring.setRadius(100);
        ring.setRingWidth(10);
        ring.setOpacity(1);
        ring.move(word.pos().x() + word.font().pointSize() - ring.radius(),
            word.pos().y() + word.font().pointSize() - ring.radius() + 200);
        pimpl_->randomWord(word);
        ring.setColor(word.color());
        pimpl_->randomColor(word);

        pimpl_->count++;
        pimpl_->countLabel.setColor(word.color());
        pimpl_->countLabel.setText(QString::number(pimpl_->count));

        break;
    }
}