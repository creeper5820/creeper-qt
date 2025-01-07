#include "mysql.hh"

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>
#include <yaml-cpp/yaml.h>

namespace mysql {
sql_create_5(VehicleInformation, 1, 5, //
    mysqlpp::sql_char, Identity, //
    mysqlpp::sql_char, Status, //
    mysqlpp::sql_char, DriveRecord, //
    mysqlpp::sql_char, DriveMileage, //
    mysqlpp::sql_char, RepairStatus //
);
sql_create_4(RouteDetail, 1, 4, //
    mysqlpp::sql_char, VehicleIdentity, //
    mysqlpp::sql_char, RouteName, //
    mysqlpp::sql_char, Status, //
    mysqlpp::sql_date, BeginTime //
);
sql_create_4(RoutesInformation, 1, 4, //
    mysqlpp::sql_char, RouteName, //
    mysqlpp::sql_int, VehicleCount, //
    mysqlpp::sql_char, EndPoint, //
    mysqlpp::sql_char, Distance //
);
}

VehicleInformation::VehicleInformation(const mysql::VehicleInformation& instance)
    : Identity(instance.Identity)
    , Status(instance.Status)
    , DriveRecord(instance.DriveRecord)
    , DriveMileage(instance.DriveMileage)
    , RepairStatus(instance.RepairStatus) { }
RouteDetail::RouteDetail(const mysql::RouteDetail& instance)
    : VehicleIdentity(instance.VehicleIdentity)
    , RouteName(instance.RouteName)
    , Status(instance.Status)
    , BeginTime(instance.BeginTime) { }
RoutesInformation::RoutesInformation(const mysql::RoutesInformation& instance)
    : RouteName(instance.RouteName)
    , VehicleCount(instance.VehicleCount)
    , EndPoint(instance.EndPoint)
    , Distance(instance.Distance) { }

Database::Database(const std::string& config) {
    tryToConnectSqlServer(config); //
}

bool Database::tryToConnectSqlServer(const std::string& path) {
    const auto yaml = YAML::LoadFile(path);

    /// @note 大作业什么的，就不做异常处理了
    const auto database = yaml["database"].as<std::string>();
    const auto username = yaml["username"].as<std::string>();
    const auto password = yaml["password"].as<std::string>();
    const auto remote = yaml["remote"].as<std::string>();
    const auto port = yaml["port"].as<int>();

    try { // connect to the mysql server
        connection.connect(
            database.c_str(), remote.c_str(), username.c_str(), password.c_str(), port);
    } catch (const mysqlpp::Exception& e) {
        std::cerr << "Something wrong happened in mysql++: " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Something wrong happened in else module" << std::endl;
        return false;
    }

    std::string info
        = "connect status: " + std::string(connection.connected() ? "success" : "failed") + "\n";
    info += "server status: " + connection.server_status() + "\n";
    info += "server version: " + connection.server_version() + "\n";
    info += "client version: " + connection.client_version() + "\n";

    std::cout << info << std::endl;

    return true;
}

void Database::createTables() {
    auto query = connection.query();

    query << R"(
            create table if not exists VehicleInformation (
                Identity varchar(20) primary key,
                Status varchar(50) not null,
                DriveRecord varchar(50) not null,
                DriveMileage varchar(50) not null,
                RepairStatus varchar(20) not null 
            );
        )";
    query.execute();

    query << R"(
            create table if not exists RoutesInformation (
                RouteName varchar(20) primary key,
                VehicleCount int not null,
                EndPoint varchar(30) not null,
                Distance varchar(30) not null
            );
        )";
    query.execute();

    query << R"(
            create table if not exists RouteDetail (
                VehicleIdentity varchar(20) not null,
                RouteName varchar(20) not null,
                Status varchar(50) not null,
                BeginTime datetime not null,
                primary key (VehicleIdentity, BeginTime),
                foreign key (VehicleIdentity) references VehicleInformation (Identity),
                foreign key (RouteName) references RoutesInformation (RouteName)
            );
        )";
    query.execute();
}

bool Database::insert(const VehicleInformation& instance) {
    auto query = connection.query();
    try {
        auto mysqlInstance = mysql::VehicleInformation(instance.Identity, instance.Status,
            instance.DriveRecord, instance.DriveMileage, instance.RepairStatus);
        query.insert(mysqlInstance).execute();
        std::cout << "VehicleInformation data inserted successfully.\n" << std::endl;
        return true;
    } catch (const mysqlpp::BadQuery& e) {
        std::cerr << "Error inserting VehicleInformation: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred while inserting VehicleInformation: " << e.what()
                  << std::endl;
        return false;
    }
}
bool Database::insert(const RouteDetail& instance) {
    auto query = connection.query();
    try {
        auto mysqlInstance = mysql::RouteDetail(
            instance.VehicleIdentity, instance.RouteName, instance.Status, instance.BeginTime);
        query.insert(mysqlInstance).execute();
        std::cout << "RouteDetail data inserted successfully." << std::endl;
        return true;
    } catch (const mysqlpp::BadQuery& e) {
        std::cerr << "Error inserting RouteDetail: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred while inserting RouteDetail: " << e.what()
                  << std::endl;
        return false;
    }
}
bool Database::insert(const RoutesInformation& instance) {
    auto query = connection.query();
    try {
        auto mysqlInstance = mysql::RoutesInformation(
            instance.RouteName, instance.VehicleCount, instance.EndPoint, instance.Distance);
        query.insert(mysqlInstance).execute();
        std::cout << "RoutesInformation data inserted successfully." << std::endl;
        return true;
    } catch (const mysqlpp::BadQuery& e) {
        std::cerr << "Error inserting RoutesInformation: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred while inserting RoutesInformation: " << e.what()
                  << std::endl;
        return false;
    }
}

bool Database::insert(const std::string& type, const auto& instance) {
    auto query = connection.query();
    try {
        auto mysqlInstances = std::vector<mysql::RoutesInformation> {};
        query.insert(instance.begin(), instance.end()).execute();
        std::cout << type + " data inserted successfully." << std::endl;
    } catch (const mysqlpp::BadQuery& e) {
        std::cerr << "Error inserting " + type + ": " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred while inserting " + type + ": " << e.what()
                  << std::endl;
        return false;
    }
    return true;
}

bool Database::update(const auto& _old, const auto& _new) {
    auto query = connection.query();
    const std::string type = typeid(_old).name();
    try {
        query.update(_old, _new).execute();
        std::cout << type + "data updated successfully." << std::endl;
        return true;
    } catch (const mysqlpp::BadQuery& e) {
        std::cerr << "Error updating " + type + ": " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred while updating " + type + ": " << e.what()
                  << std::endl;
        return false;
    }
}

bool Database::remove(const auto& instance) { return {}; }

std::vector<VehicleInformation> Database::getVehicleInformations() {
    auto query = connection.query("select * from VehicleInformation");
    auto data = std::vector<VehicleInformation> {};
    query.storein(data);
    return data;
}

std::vector<RouteDetail> Database::getRouteDetails() {
    auto query = connection.query("select * from RouteDetail");
    auto data = std::vector<RouteDetail> {};
    query.storein(data);
    return data;
}

std::vector<RoutesInformation> Database::getRoutesInformations() {
    auto query = connection.query("select * from RoutesInformation");
    auto data = std::vector<RoutesInformation> {};
    query.storein(data);
    return data;
}

std::optional<VehicleInformation> Database::getVehicleInformation(const std::string& key) {
    return {};
}

std::optional<RouteDetail> Database::getRouteDetail(const std::string& key) { //
    return {};
}

std::optional<RoutesInformation> Database::getRoutesInformation(const std::string& key) {
    return {};
}

void Database::generateSomeDataAndInsert() {
    auto query = connection.query();

    auto vehicleInformationArray = std::array { //
        mysql::VehicleInformation { "粤A12345", "正常", "无故障", "20000 km", "无维修" },
        mysql::VehicleInformation { "京B67890", "维修中", "发动机故障", "30000 km", "需要维修" },
        mysql::VehicleInformation { "沪C23456", "正常", "无故障", "15000 km", "无维修" },
        mysql::VehicleInformation { "津D98765", "正常", "无故障", "5000 km", "无维修" },
        mysql::VehicleInformation { "川E54321", "保养中", "定期保养", "25000 km", "定期维修" },
        mysql::VehicleInformation { "渝F11223", "正常", "无故障", "40000 km", "无维修" },
        mysql::VehicleInformation { "鲁G33445", "正常", "无故障", "12000 km", "无维修" },
        mysql::VehicleInformation { "豫H55667", "维修中", "刹车系统故障", "22000 km", "需要维修" },
        mysql::VehicleInformation { "湘J77889", "正常", "无故障", "8000 km", "无维修" },
        mysql::VehicleInformation { "赣K99000", "保养中", "机油更换", "18000 km", "定期维修" }
    };

    auto routeDetailArray = std::array { //
        mysql::RouteDetail { "粤A12345", "路线A", "正常", mysqlpp::sql_date("2024-01-01") },
        mysql::RouteDetail { "京B67890", "路线B", "拥堵", mysqlpp::sql_date("2024-01-02") },
        mysql::RouteDetail { "沪C23456", "路线A", "正常", mysqlpp::sql_date("2024-01-03") },
        mysql::RouteDetail { "津D98765", "路线C", "正常", mysqlpp::sql_date("2024-01-04") },
        mysql::RouteDetail { "川E54321", "路线B", "延误", mysqlpp::sql_date("2024-01-05") },
        mysql::RouteDetail { "渝F11223", "路线A", "正常", mysqlpp::sql_date("2024-01-06") },
        mysql::RouteDetail { "鲁G33445", "路线C", "正常", mysqlpp::sql_date("2024-01-07") },
        mysql::RouteDetail { "豫H55667", "路线A", "堵车", mysqlpp::sql_date("2024-01-08") },
        mysql::RouteDetail { "湘J77889", "路线B", "正常", mysqlpp::sql_date("2024-01-09") },
        mysql::RouteDetail { "赣K99000", "路线D", "维护", mysqlpp::sql_date("2024-01-10") }
    };

    auto routesInformationArray = std::array { //
        mysql::RoutesInformation { "路线A", 3, "上海", "50 km" },
        mysql::RoutesInformation { "路线B", 2, "北京", "70 km" },
        mysql::RoutesInformation { "路线C", 1, "广州", "90 km" },
        mysql::RoutesInformation { "路线D", 4, "深圳", "120 km" },
        mysql::RoutesInformation { "路线E", 5, "成都", "150 km" }
    };

    insert("RoutesInformationTest", routesInformationArray);
    insert("VehicleInformationTest", vehicleInformationArray);
    insert("RouteDetailTest", routeDetailArray);
}

bool Database::connected() const noexcept { return connection.connected(); }
