#if !defined(CONFIG_HPP)
#define CONFIG_HPP

#include "../Includes/Includes.hpp"
#include "webserv.hpp"
#include "StructData.hpp"
#include "HelperFunc.hpp"
#include "ReaderConf.hpp"
#include "ConfigServer.hpp"
// #include "../request/Request.hpp"
#include "RequestConfig.hpp"

class RequestConfiga;
class Config
{
private:
    bool    getServerforRequest(ConfigServer &conf, t_listen const address, std::string const hostname)const;
    std::vector<ConfigServer> _server; 
public:
    Config(){}
    Config(std::string defaultserverPath);
    virtual ~Config(); 
    int parse(const char* filename);
    std::vector<ConfigServer> getServer() const;
    // RequestConfiga   getConfigForRequest(t_listen const adress, std::string const uri, std::string hostname, const std::string &methodt) const;
    // RequestConfiga messi();
    std::vector<t_listen> getAllListen() const;
};

#endif