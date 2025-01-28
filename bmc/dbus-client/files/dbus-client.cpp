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
#include <iostream> 

constexpr char const *BusName = "xyz.openbmc_project.DemoDbus";
constexpr char const *BusPath = "/xyz/openbmc_project/DemoDbus";
constexpr char const *PropertyInterface = "org.freedesktop.DBus.Properties";
constexpr char const *PropertyName = "Value1";  // 要讀取的屬性名稱
constexpr char const *AddInterface = "xyz.openbmc_project.Add";  // 屬性所在的介面名稱

int main() {
    // int value;
    std::variant<int> value;
    boost::asio::io_service io;

    // 創建 D-Bus 連接
    auto connection = std::make_shared<sdbusplus::asio::connection>(io);

    // 創建方法調用，目標為 `org.freedesktop.DBus.Introspectable` 接口中的 `Introspect` 方法
    auto method = connection->new_method_call(
        "xyz.openbmc_project.DemoDbus",  // 目標服務名稱
        "/xyz/openbmc_project/DemoDbus",  // 目標物件路徑
        "org.freedesktop.DBus.Introspectable",  // 目標接口名稱
        "Introspect"  // 方法名稱
    );

    // 發送方法調用並接收回應
    auto reply = connection->call(method);

    // 讀取回應中的 XML 描述
    std::string xml;
    reply.read(xml);

    // 輸出 introspection 結果
    std::cout << "Introspection XML:\n" << xml << std::endl;

    // 創建方法調用，目標為 `org.freedesktop.DBus.Properties` 接口中的 `Get` 方法
    method = connection->new_method_call(
        "xyz.openbmc_project.DemoDbus",  // 目標服務名稱
        "/xyz/openbmc_project/DemoDbus",  // 目標物件路徑
        "org.freedesktop.DBus.Properties",  // 目標接口名稱
        "Get"  // 方法名稱
    );

    // 追加接口名稱和屬性名稱
    method.append("xyz.openbmc_project.Add", "Value1");
    reply = connection->call(method);
    try {
        std::cout << "Reading Value..." << std::endl;
        // 發送方法調用並接收回應
        

        // 讀取回應中的屬性值
        
        reply.read(value);  // 讀取屬性值
        std::cout << "Received Value1: " << std::get<int>(value) << std::endl;  
        
        // std::cout << "Received Value1: " << value << std::endl;

        // 執行事件循環
        

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    io.run();
    return 0;
}
