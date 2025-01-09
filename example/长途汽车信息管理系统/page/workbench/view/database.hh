/// @brief 好TM长的UI代码，算了......这只是大作业
///        好孩子平时写项目决不能这么写的哦

#pragma once

// #include "database/mysql.hh"
// #include <ranges>

#include <creeper-qt/setting/color.hh>
#include <creeper-qt/widget/basic-shape.hh>
#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/line-edit.hh>
#include <creeper-qt/widget/push-button.hh>
#include <qboxlayout.h>
#include <qscrollbar.h>

/// @todo 表格视图，定叫你被全部重构！
///       这简直太疯狂了！
///       使用奇怪渐变和非圆角样式是不符合道义的！
///       使用笨拙滑条是不符合理智的！
#include <qtablewidget.h>

class DataBaseView : public creeper::RoundedRectangle {
public:
    static constexpr auto tableStyle = R"(
    QTableView , QTableWidget{
        selection-background-color: rgb(156, 156, 156);
        background-color: rgb(255, 255, 255);
        color: rgb(97, 97, 97);
    }
    QTableView QTableCornerButton::section {
        background: rgb(204, 204, 204);
        border: 2px solid rgba(0, 0, 0, 0);
    }
    QHeaderView::section{
        background-color: rgba(0, 0, 0, 0);
        border-bottom: 1px solid rgba(0, 0, 0, 0);
        border: 1px solid rgb(204, 204, 204);
        height: 20px;
        color: rgb(131, 131, 131);
    }
    )";

public:
    explicit DataBaseView() {
        using namespace creeper;
        auto fuckTableWidgetStyle = [this](QTableWidget& table, const QStringList& title) {
            table.setColumnCount(title.size());
            table.setHorizontalHeaderLabels(title);
            table.setFont(QFont("JetBrains Mono", 8));
            table.setStyleSheet(tableStyle);
            table.setHorizontalScrollMode(QTableView::ScrollMode::ScrollPerPixel);
            table.setVerticalScrollMode(QTableView::ScrollMode::ScrollPerPixel);
            table.horizontalScrollBar()->setSingleStep(10);
            table.verticalScrollBar()->setSingleStep(10);
        };

        fuckTableWidgetStyle(vehicleInformationsTable, vehicleInformationTitle);
        fuckTableWidgetStyle(vehicleStatusesTable, vehicleStatuseTitle);
        fuckTableWidgetStyle(routesInformationsTable, routesInformationTitle);

        updateFromDatabase();

        auto layout = new QHBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(10);
        layout->setAlignment(Qt::AlignLeft);
        layout->addWidget(&vehicleInformationsTable);
        layout->addWidget(&vehicleStatusesTable);
        layout->addWidget(&routesInformationsTable);

        const auto buttonFont = QFont("Nowar Warcraft Sans CN", 8);
        const auto buttonSize = QSize(80, 30);
        PushButton::Style {
            .text = "插入数据",
            .size = buttonSize,
            .font = buttonFont,
            .radiusRatio = 0.2,
            .background = false,
        }(insertButton);
        PushButton::Style {
            .text = "查询数据",
            .size = buttonSize,
            .font = buttonFont,
            .radiusRatio = 0.2,
            .background = false,
        }(searchButton);
        PushButton::Style {
            .text = "删除数据",
            .size = buttonSize,
            .font = buttonFont,
            .radiusRatio = 0.2,
            .background = false,
        }(deleteButton);

        searchLineEdit.setPlaceholderText("输入搜索名称");
        searchLineEdit.setFixedSize(200, 30);
        searchLineEdit.setIcon(QPixmap(":/theme/icon/normal/search.png"));
        searchLineEdit.setFont(buttonFont);

        auto buttonLayout = new QHBoxLayout;
        buttonLayout->setMargin(10);
        buttonLayout->setSpacing(10);
        buttonLayout->setAlignment(Qt::AlignLeft);
        buttonLayout->addSpacing(15);
        buttonLayout->addWidget(&insertButton);
        buttonLayout->addWidget(&searchButton);
        buttonLayout->addWidget(&deleteButton);
        buttonLayout->addWidget(&searchLineEdit);

        auto topBackground = new RoundedRectangle;
        topBackground->setBackground(color::grey100);
        topBackground->setFixedHeight(50);
        topBackground->setRadius(5);
        topBackground->setLayout(buttonLayout);

        auto mainVerticalLayout = new QVBoxLayout;
        mainVerticalLayout->setMargin(10);
        mainVerticalLayout->setSpacing(10);
        mainVerticalLayout->setAlignment(Qt::AlignTop);
        mainVerticalLayout->addWidget(topBackground);
        mainVerticalLayout->addLayout(layout);
        setLayout(mainVerticalLayout);

        auto vehicleEditTextOnInsertPanel = new Label("车辆名称");
        auto vehicleEditOnInsertPanel = new LineEdit;
        auto statusTextOnInsertPanel = new Label("车辆状态");
        auto statusEditOnInsertPanel = new LineEdit;
        auto driveRecordTextOnInsertPanel = new Label("行驶记录");
        auto driveRecordEditOnInsertPanel = new LineEdit;
        auto driveMileageTextOnInsertPanel = new Label("行驶里程");
        auto driveMileageEditOnInsertPanel = new LineEdit;
        auto repairStatusTextOnInsertPanel = new Label("维修状态");
        auto repairStatusEditOnInsertPanel = new LineEdit;

        auto panelEnsureButton = PushButton::Style {
            .text = "确定操作",
            .size = QSize(80, 30),
            .font = buttonFont,
            .radiusRatio = 0.2,
        }();

        auto panelCancelButton = PushButton::Style {
            .text = "清空输入",
            .size = QSize(80, 30),
            .font = buttonFont,
            .radiusRatio = 0.2,
        }();

        auto panelFont = QFont("JetBrains Mono", 8);
        auto panelTextConfig = [panelFont](Label& label) {
            label.setFont(panelFont);
            label.setAlignment(Qt::AlignCenter);
            label.setFixedSize(60, 30);
        };
        auto lineEditConfig = [panelFont](LineEdit& edit) {
            edit.setFixedSize(150, 30);
            edit.setFont(panelFont);
            edit.setPlaceholderText("请输入数据");
        };

        panelTextConfig(*vehicleEditTextOnInsertPanel);
        panelTextConfig(*statusTextOnInsertPanel);
        panelTextConfig(*driveRecordTextOnInsertPanel);
        panelTextConfig(*driveMileageTextOnInsertPanel);
        panelTextConfig(*repairStatusTextOnInsertPanel);

        auto panelTextVerticalLayout = new QVBoxLayout;
        panelTextVerticalLayout->setMargin(10);
        panelTextVerticalLayout->setSpacing(10);
        panelTextVerticalLayout->setAlignment(Qt::AlignTop);
        panelTextVerticalLayout->addWidget(vehicleEditTextOnInsertPanel);
        panelTextVerticalLayout->addWidget(statusTextOnInsertPanel);
        panelTextVerticalLayout->addWidget(driveRecordTextOnInsertPanel);
        panelTextVerticalLayout->addWidget(driveMileageTextOnInsertPanel);
        panelTextVerticalLayout->addWidget(repairStatusTextOnInsertPanel);

        lineEditConfig(*vehicleEditOnInsertPanel);
        lineEditConfig(*statusEditOnInsertPanel);
        lineEditConfig(*driveRecordEditOnInsertPanel);
        lineEditConfig(*driveMileageEditOnInsertPanel);
        lineEditConfig(*repairStatusEditOnInsertPanel);

        auto panelEditVerticalLayout = new QVBoxLayout;
        panelEditVerticalLayout->setMargin(10);
        panelEditVerticalLayout->setSpacing(10);
        panelEditVerticalLayout->setAlignment(Qt::AlignTop);
        panelEditVerticalLayout->addWidget(vehicleEditOnInsertPanel);
        panelEditVerticalLayout->addWidget(statusEditOnInsertPanel);
        panelEditVerticalLayout->addWidget(driveRecordEditOnInsertPanel);
        panelEditVerticalLayout->addWidget(driveMileageEditOnInsertPanel);
        panelEditVerticalLayout->addWidget(repairStatusEditOnInsertPanel);

        auto panelInputHorizonLayout = new QHBoxLayout;
        panelInputHorizonLayout->setMargin(10);
        panelInputHorizonLayout->setAlignment(Qt::AlignCenter);
        panelInputHorizonLayout->addLayout(panelTextVerticalLayout);
        panelInputHorizonLayout->addLayout(panelEditVerticalLayout);

        auto panelButtonHorizonLayout = new QHBoxLayout;
        panelButtonHorizonLayout->setMargin(10);
        panelButtonHorizonLayout->setSpacing(10);
        panelButtonHorizonLayout->setAlignment(Qt::AlignCenter);
        panelButtonHorizonLayout->addWidget(panelEnsureButton);
        panelButtonHorizonLayout->addWidget(panelCancelButton);

        auto panelLayout = new QVBoxLayout;
        panelLayout->setMargin(10);
        panelLayout->addLayout(panelInputHorizonLayout);
        panelLayout->addSpacing(10);
        panelLayout->addLayout(panelButtonHorizonLayout);

        insertPanel.setLayout(panelLayout);

        connect(panelEnsureButton, &QPushButton::clicked,
            [this, vehicleEditOnInsertPanel, statusEditOnInsertPanel, driveRecordEditOnInsertPanel,
                driveMileageEditOnInsertPanel, repairStatusEditOnInsertPanel, panelEnsureButton] {
                const auto vehicleIdentity = vehicleEditOnInsertPanel->text();
                const auto status = statusEditOnInsertPanel->text();
                const auto driveRecord = driveRecordEditOnInsertPanel->text();
                const auto driveMileage = driveMileageEditOnInsertPanel->text();
                const auto repairStatus = repairStatusEditOnInsertPanel->text();

                if (vehicleIdentity.isEmpty() || status.isEmpty() || driveRecord.isEmpty()
                    || driveMileage.isEmpty() || repairStatus.isEmpty()) {
                    panelEnsureButton->setText("请输入完整");
                } else {
                    panelEnsureButton->setText("确定操作");
                    // auto data = VehicleInformation {};
                    // data.Identity = vehicleIdentity.toStdString();
                    // data.Status = status.toStdString();
                    // data.DriveRecord = driveRecord.toStdString();
                    // data.DriveMileage = driveMileage.toStdString();
                    // data.RepairStatus = repairStatus.toStdString();
                    // client.insert(data);
                    updateFromDatabase();
                }
            });

        connect(panelCancelButton, &QPushButton::clicked,
            [this, vehicleEditOnInsertPanel, statusEditOnInsertPanel, driveRecordEditOnInsertPanel,
                driveMileageEditOnInsertPanel, repairStatusEditOnInsertPanel] {
                vehicleEditOnInsertPanel->clear();
                statusEditOnInsertPanel->clear();
                driveRecordEditOnInsertPanel->clear();
                driveMileageEditOnInsertPanel->clear();
                repairStatusEditOnInsertPanel->clear();
            });

        connect(&insertButton, &QPushButton::clicked, [this, panelEnsureButton] {
            auto target = insertButton.mapToGlobal(QPoint(0, 0))
                + QPoint(insertButton.width() / 2 - insertPanel.width() / 2, insertButton.height());
            panelEnsureButton->setText("确定操作");
            insertPanel.setParent(this);
            insertPanel.move(target);
            insertPanel.show();
            searchPanel.hide();
            deletePanel.hide();
        });

        connect(&searchButton, &QPushButton::clicked, [this] {
            auto target = searchButton.mapToGlobal(QPoint(0, 0))
                + QPoint(searchButton.width() / 2 - searchPanel.width() / 2, searchButton.height());
            searchPanel.setParent(this);
            searchPanel.move(target);
            searchPanel.show();
            insertPanel.hide();
            deletePanel.hide();
        });

        connect(&deleteButton, &QPushButton::clicked, [this] {
            auto target = deleteButton.mapToGlobal(QPoint(0, 0))
                + QPoint(deleteButton.width() / 2 - deletePanel.width() / 2, deleteButton.height());
            deletePanel.setParent(this);
            deletePanel.move(target);
            deletePanel.show();
            insertPanel.hide();
            searchPanel.hide();
        });
    }

private:
    struct FuckQtPanel : public RoundedRectangle {
        FuckQtPanel(QWidget* parent, creeper::PushButton* button)
            : RoundedRectangle(parent) {
            setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
            setFixedSize(300, 300);
            setBorderWidth(3);
            setBorderColor(creeper::color::grey400);
        }
        void leaveEvent(QEvent* event) override { hide(); }
    } insertPanel { this, &insertButton }, searchPanel { this, &searchButton },
        deletePanel { this, &deleteButton };

    QTableWidget vehicleInformationsTable;
    QTableWidget vehicleStatusesTable;
    QTableWidget routesInformationsTable;

    creeper::LineEdit searchLineEdit;
    creeper::PushButton insertButton;
    creeper::PushButton searchButton;
    creeper::PushButton deleteButton;

    /// @note 既然是大作业，那么写绝对路径也是一件能被原谅的事情吧
    /// @note 二编：算了，还是加一个宏定义
    // Database client { std::string(PROJECT_SOURCE_DIR) + "/database.yaml" };

    // std::vector<VehicleInformation> vehicleInformations;
    // std::vector<RouteDetail> vehicleStatuses;
    // std::vector<RoutesInformation> routesInformations;

    QStringList vehicleInformationTitle { //
        "车牌号", "状态", "行驶记录", "行驶里程", "维修状态"
    };
    QStringList vehicleStatuseTitle { //
        "车牌号", "路线名称", "路线状态", "启程时间"
    };
    QStringList routesInformationTitle { //
        "路线名称", "车辆数量", "终点", "总距离"
    };

    // only for test without mysql
    void updateFromDatabase() {
        vehicleInformationsTable.setRowCount(3);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 5; j++)
                vehicleInformationsTable.setItem(i, j, new QTableWidgetItem("数据"));

        vehicleStatusesTable.setRowCount(3);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
                vehicleStatusesTable.setItem(i, j, new QTableWidgetItem("数据"));

        routesInformationsTable.setRowCount(3);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
                routesInformationsTable.setItem(i, j, new QTableWidgetItem("数据"));
    }

    // void updateFromDatabase() {
    // vehicleInformations = client.getVehicleInformations();
    // vehicleInformationsTable.setRowCount(vehicleInformations.size());
    // for (const auto [i, item] : std::views::enumerate(vehicleInformations)) {
    //     vehicleInformationsTable.setItem(i, 0, //
    //         new QTableWidgetItem(item.Identity.c_str()));
    //     vehicleInformationsTable.setItem(i, 1, //
    //         new QTableWidgetItem(item.Status.c_str()));
    //     vehicleInformationsTable.setItem(i, 2, //
    //         new QTableWidgetItem(item.DriveRecord.c_str()));
    //     vehicleInformationsTable.setItem(i, 3, //
    //         new QTableWidgetItem(item.DriveMileage.c_str()));
    //     vehicleInformationsTable.setItem(i, 4, //
    //         new QTableWidgetItem(item.RepairStatus.c_str()));
    // }
    //
    // vehicleStatuses = client.getRouteDetails();
    // vehicleStatusesTable.setRowCount(vehicleStatuses.size());
    // for (const auto [i, item] : std::views::enumerate(vehicleStatuses)) {
    // vehicleStatusesTable.setItem(i, 0, //
    // new QTableWidgetItem(item.VehicleIdentity.c_str()));
    // vehicleStatusesTable.setItem(i, 1, //
    // new QTableWidgetItem(item.RouteName.c_str()));
    // vehicleStatusesTable.setItem(i, 2, //
    // new QTableWidgetItem(item.Status.c_str()));
    // vehicleStatusesTable.setItem(i, 3, //
    // new QTableWidgetItem(item.BeginTime.str().c_str()));
    // }
    //
    // routesInformations = client.getRoutesInformations();
    // routesInformationsTable.setRowCount(routesInformations.size());
    // for (const auto [i, item] : std::views::enumerate(routesInformations)) {
    // routesInformationsTable.setItem(i, 0, //
    // new QTableWidgetItem(item.RouteName.c_str()));
    // routesInformationsTable.setItem(i, 1, //
    // new QTableWidgetItem(QString::number(item.VehicleCount)));
    // routesInformationsTable.setItem(i, 2, //
    // new QTableWidgetItem(item.EndPoint.c_str()));
    // routesInformationsTable.setItem(i, 3, //
    // new QTableWidgetItem(item.Distance.c_str()));
    // }
    //
    // std::cout << "update data from database: \n";
    // std::cout << "vehicleInformations: " << vehicleInformations.size() << "\n";
    // std::cout << "vehicleStatuses: " << vehicleStatuses.size() << '\n';
    // std::cout << "routesInformations: " << routesInformations.size() << std::endl;
    // }
};
