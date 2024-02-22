#include <drogon/drogon.h>
#include "./controllers/ApiController.hpp"
int main() {
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 9999);
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().setThreadNum( std::thread::hardware_concurrency() * 2)
      .setClientMaxBodySize(1 * 1024 * 1024 * 1024).run();
    return 0;
}
