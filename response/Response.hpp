#include "../conf/webserv.hpp"
#include "../request/Request.hpp"
#include "../conf/RequestConfig.hpp"
#include "../conf/ConfigServer.hpp"
#include "HeaderRes.hpp"

class Response
{
private:
    std::string _response;
    std::string path;
    int code;
    std::string _type;
    bool _isAutoIndex;
    t_listen _hostPort;
    std::map<int , std::string> error_map;
public:
    Response();
    ~Response();
    
    std::string  getResponse();

    void  call(Request & Request, RequestConfiga & requestConfig);
    static void    getLocationMatch(std::string &path, std::map<std::string ,ConfigServer> locations, std::string &location_key);
    void GetMethod(Request & request, RequestConfiga & RequestConfig);
    void PostMethod(Request & request, RequestConfiga & RequestConfig);
    void DeleteMethod(Request & request, RequestConfiga & RequestConfig);

    int readContent();
    int writeContent();
    int fileExist();
    std::string readHtml(const std::string & path);
    int pathIsfile(std::string &path);
    std::string getPage(const char * path, std::string &host, int port);
    std::string getLink(const std::string &direntry, std::string const &dirname, std::string const &host, int port);

    static std::map<std::string, void (Response::*)(Request &, RequestConfiga &)> method;
    static std::map<std::string, void (Response::*) (Request &, RequestConfiga &)> initMethods();
};

