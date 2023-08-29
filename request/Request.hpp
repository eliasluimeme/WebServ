#pragma once


#include "../conf/webserv.hpp"

class Request
{
private:
    std::string _method;
    std::string _version;
    std::map<std::string, std::string>  _header;
    std::map<std::string, std::string>  _envCgi;
    int ret;
    std::string body;
    int _port;
    std::string path;
    std::string _query;
    std::list<std::pair<std::string, float> > _lang;
    const std::string& _raw;

    //parsing
    int readFirstLine(const std::string & str);
    int readPath(const std::string line, size_t i);
    int readVersion(const std::string& line, size_t i);
    int checkMethod();
    int Checkport();
    void findQuery();
    std::string formatheaderforCgi(std::string &key);
    std::string nextLine(const std::string &str, size_t& i);
    void setLang();

    static std::vector<std::string> methods;
    
public:
    Request(const std::string& str);
    ~Request();
    Request& operator=(const Request& rhs);

    //getters
    const std::string & getMethod() const;
    const std::string & getversion()const;
    const std::map <std::string, std::string > &get_header() const;
    const std::map <std::string, std::string> &get_env() const;
    int get_ret() const;
    const std::string &get_body() const;
    int get_port() const;
    const std::string &get_path() const;
    const std::string &get_query() const;
    const std::list<std::pair<std::string, float> > &get_lang() const;
    const std::string &get_raw() const; 
    //setters
    void setBody(const std::string &str);
    void setRet(int);
    void setMethod(const std::string &method);
    //utisls
    int parse(const std::string &str);
    void resetHeader();
    void stripAll();
    void displayHeader() const;

    static std::vector<std::string>  init_Methods();
};

std::ostream& operator<<(std::ostream& os, const Request& re);