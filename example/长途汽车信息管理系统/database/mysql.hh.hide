#pragma once

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>
#include <yaml-cpp/yaml.h>

namespace mysql {
struct VehicleInformation;
struct RouteDetail;
struct RoutesInformation;
}

struct VehicleInformation {
    VehicleInformation(const mysql::VehicleInformation&);
    VehicleInformation() = default;
    mysqlpp::sql_char Identity;
    mysqlpp::sql_char Status;
    mysqlpp::sql_char DriveRecord;
    mysqlpp::sql_char DriveMileage;
    mysqlpp::sql_char RepairStatus;
};

struct RouteDetail {
    RouteDetail(const mysql::RouteDetail&);
    RouteDetail() = default;
    mysqlpp::sql_char VehicleIdentity;
    mysqlpp::sql_char RouteName;
    mysqlpp::sql_char Status;
    mysqlpp::sql_date BeginTime;
};

struct RoutesInformation {
    RoutesInformation(const mysql::RoutesInformation&);
    RoutesInformation() = default;
    mysqlpp::sql_char RouteName;
    mysqlpp::sql_int VehicleCount;
    mysqlpp::sql_char EndPoint;
    mysqlpp::sql_char Distance;
};

class Database {
public:
    explicit Database(const std::string& config);

    bool tryToConnectSqlServer(const std::string& path);

    void createTables();

    bool insert(const VehicleInformation& instance);
    bool insert(const RouteDetail& instance);
    bool insert(const RoutesInformation& instance);

    bool update(const auto& _old, const auto& _new);
    bool remove(const auto& instance);

    std::vector<VehicleInformation> getVehicleInformations();
    std::vector<RouteDetail> getRouteDetails();
    std::vector<RoutesInformation> getRoutesInformations();

    std::optional<VehicleInformation> getVehicleInformation(const std::string& key);
    std::optional<RouteDetail> getRouteDetail(const std::string& key);
    std::optional<RoutesInformation> getRoutesInformation(const std::string& key);

    void generateSomeDataAndInsert();

    bool connected() const noexcept;

private:
    mysqlpp::Connection connection;

private:
    bool insert(const std::string& type, const auto& instance);
};
