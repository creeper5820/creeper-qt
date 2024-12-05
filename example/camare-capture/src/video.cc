#include "video.hh"
#include <thread>

#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>

#include <qpainter.h>
#include <qpainterpath.h>
#include <qtimer.h>

using namespace creeper;

struct Video::Impl {
public:
    Impl() {
        spdlog::info("starting camera thread");
        thread = std::make_unique<std::thread>([this] {
            while (!exitRequest) {
                update();
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(16.666ms);
            }
        });
    }

    ~Impl() {
        exitRequest = true;
        if (thread->joinable()) thread->join();
    }

    void sendConnectRequest() { connectRequest = true; }

    void update() {
        if (connectRequest && !connected) connect();
        if (!connected) return;

        auto frame = cv::Mat {};
        const auto available = camera->read(frame);
        if (!available) {
            spdlog::error("Error reading frame");
            connected = false;
            return;
        }

        writeIndex = (writeIndex + 1) % framebuffers.size();
        framebuffers[writeIndex]
            = QImage(frame.data, frame.cols, frame.rows, frame.step1(), QImage::Format_BGR888);
    }

public:
    QImage image;
    QTimer timer;

    std::array<QImage, 5> framebuffers;
    int readIndex = 0;
    int writeIndex = 0;

    std::unique_ptr<cv::VideoCapture> camera;
    std::unique_ptr<std::thread> thread;

    std::atomic<bool> connected = false;
    std::atomic<bool> exitRequest = false;

private:
    static void setCameraOption(cv::VideoCapture& camera) {
        spdlog::info("all camera options are set");
    }

    static void setCameraOption(
        cv::VideoCapture& camera, int property, double value, auto... args) {
        camera.set(property, value);
        setCameraOption(camera, args...);
    }

    void connect() {
        spdlog::info("Connecting to camera");
        camera = std::make_unique<cv::VideoCapture>(2, cv::CAP_V4L2);
        if (!camera->isOpened()) {
            spdlog::error("Error opening video stream or file");
            return;
        }
        // for standard usb 2.0 v4l2 camera
        setCameraOption(*camera, //
            cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), //
            cv::CAP_PROP_FPS, 60, //
            cv::CAP_PROP_FRAME_WIDTH, 1280, //
            cv::CAP_PROP_FRAME_HEIGHT, 720, //
            cv::CAP_PROP_AUTO_EXPOSURE, 1.0, //
            cv::CAP_PROP_EXPOSURE, 200, //
            cv::CAP_PROP_BRIGHTNESS, 0 //
        );
        spdlog::info("camera connected");
        connected = true;
        connectRequest = false;
    }

private:
    std::atomic<bool> connectRequest = false;
};

Video::Video(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl) {

    setFixedSize(1280 / 2, 720 / 2);

    connect(&pimpl_->timer, &QTimer::timeout, [this] {
        if (pimpl_->writeIndex == pimpl_->readIndex) return;
        pimpl_->readIndex = (pimpl_->readIndex + 1) % 5;
        pimpl_->image = pimpl_->framebuffers[pimpl_->readIndex];
        update();
    });
    pimpl_->timer.start(10);
}

Video::~Video() { delete pimpl_; }

void Video::paintEvent(QPaintEvent* event) {
    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    auto path = QPainterPath();
    path.addRoundedRect(0, 0, width(), height(), 10, 10);
    painter.setClipPath(path);
    painter.setBrush({ Theme::color("primary050") });
    painter.setPen(QPen { Theme::color("primary050") });
    painter.drawRoundedRect(0, 0, width(), height(), 10, 10);

    if (!pimpl_->connected) return;
    painter.drawImage(0, 0, pimpl_->image.scaled(width(), height(), Qt::KeepAspectRatio));
}

void Video::connectCamera(int index) { pimpl_->sendConnectRequest(); }

bool Video::connected() const { return pimpl_->connected; }

void Video::reloadTheme() { }