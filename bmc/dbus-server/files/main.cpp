// #include "include/dbus_include.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/container/flat_set.hpp>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <systemd/sd-journal.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <boost/asio/io_service.hpp>
#include <vector>

// Define extern variables
int32_t Value1 = 0;
int32_t Value2 = 0;

constexpr char const *createDbusService = "xyz.openbmc_project.DemoDbus";
constexpr char const *createDbusObjPath = "/xyz/openbmc_project/DemoDbus";
constexpr char const *createDbusInf = "xyz.openbmc_project.Add";

std::shared_ptr<sdbusplus::asio::dbus_interface> demoBusAddInterface;

static int32_t value_add(int32_t v1, int32_t v2)
{
    return v1 + v2;
}

int main(int argc, char *argv[])
{
    printf("hello! from dbus-serve\n");
    boost::asio::io_service io;
    
    auto systemBus = std::make_shared<sdbusplus::asio::connection>(io);
    sdbusplus::asio::object_server objectServer(systemBus);
    objectServer.add_manager("/xyz/openbmc_project/DemoDbus");

    systemBus->request_name(createDbusService);

    demoBusAddInterface = objectServer.add_interface(createDbusObjPath, createDbusInf);
    demoBusAddInterface->register_property("Value1", Value1, sdbusplus::asio::PropertyPermission::readWrite);
    demoBusAddInterface->register_property("Value2", Value2, sdbusplus::asio::PropertyPermission::readWrite);
    
    demoBusAddInterface->register_method(
        "Add",
        [](int32_t v1, int32_t v2) -> int32_t {
            return value_add(v1, v2);
        });
    
    demoBusAddInterface->initialize();


    io.run();

    return 0;
}