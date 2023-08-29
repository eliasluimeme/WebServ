#include "Request.hpp"

std::vector<std::string> Request::init_Methods()
{
    
    std::vector<std::string> methods;
    methods.push_back("GET");
    methods.push_back("POST");
    methods.push_back("DELETE");
    return methods;
}

std::vector<std::string>    Request::methods = Request::init_Methods();


Request::Request(const std::string& str):_method(""), _version(""),ret(200),body(""), _port(80),path(""),_query(""),_raw(str)
{
    this->resetHeader();
    this->_envCgi.clear();
    this->parse(str);
    if (ret != 200)\
        std::cerr << "parse Error: " << this->ret << '\n';
}

Request::~Request()
{

}

Request& Request::operator=(const Request& rhs)
{
    this->_method = rhs.getMethod();
    this->_version = rhs.getversion();
    this->ret = rhs.get_ret();
    this->body = rhs.get_body();
    this->_port = rhs.get_port();
    this->path = rhs.get_path();
    this->_query = rhs.get_query();

    return *this;
}

const std::string & Request::getMethod() const
{
    return this->_method;
}
const std::string & Request::getversion() const
{
    return this->_version;
}

const std::map<std::string , std::string> & Request::get_header() const
{
    return this->_header;
}
const std::map<std::string, std::string> & Request::get_env() const
{
    return this->_envCgi;
}

int Request::get_ret() const
{
    return this->ret;
}

int Request::get_port() const
{
    return this->_port;
}
const std::string& Request::get_body() const
{
    return this->body;
}

const std::string& Request::get_path() const 
{
    return this->path;
}

const std::string& Request::get_query() const 
{
    return this->_query;
}

const std::list<std::pair <std::string, float> >& Request::get_lang() const
{
    return this->_lang;
}

const std::string & Request::get_raw() const
{
    return this->_raw;
}


void Request::setRet(int ret)
{
    this->ret = ret;
}

void Request::setMethod(const std::string &method)
{
    this->_method = method;
}

void Request::setBody(const std::string &str)
{
    char	strip[] = {'\n', '\r'};

	this->body.assign(str);
	for (int i = 0; i < 4; i++)
		if (this->body.size() > 0 && this->body[this->body.size() - 1] == strip[i % 2])
			pop(this->body);
		else
			break ;
}

