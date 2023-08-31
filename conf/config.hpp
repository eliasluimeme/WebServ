#if !defined(CONFIG_HPP)
#define CONFIG_HPP

#include "webserv.hpp"
#include "StructData.hpp"
#include "HelperFunc.hpp"
#include "ReaderConf.hpp"
#include "ConfigServer.hpp"
#include "../Includes/Includes.hpp"
// #include "../request/Request.hpp"
// #include "RequestConfig.hpp"
class confData;
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
    int parse(const char* filename, confData &);
    std::vector<ConfigServer> getServer() const;
    bool checkfile(filevector file);
    // RequestConfiga   getConfigForRequest(t_listen const adress, std::string const uri, std::string hostname, const std::string &methodt) const;
    // RequestConfiga messi();
    std::vector<t_listen> getAllListen() const;
};
 
#endif