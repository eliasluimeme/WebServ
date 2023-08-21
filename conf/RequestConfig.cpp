

#include "RequestConfig.hpp"

RequestConfiga::RequestConfiga()
{
    return;
}

RequestConfiga::RequestConfiga(ConfigServer &config, Request &request,const std::string &path,  const std::string &method, std::string &locationName):
_error_page(config.get_errorPage()),
_client_body_size(config.getclientbodyBuffersize()),
_cgi_param(config.getCgiParms()),
_allowed_method(config.getAllowedmethod()),
_cgi_pass(config.getCgipass()),
_lang(""),
autoindex(config.getAutoindex())
{

    std::string alias = config.getalias();
    std::string root = config.get_root();
    std::string ret;

    std::vector<std::string> conf_index = config.getIndex();
    for (std::vector<std::string>::iterator it = conf_index.begin(); it != conf_index.end() ; it++)
    {
        std::vector<std::string>::iterator it2 = _index.begin();
        for ( it2 = _index.begin(); it2 < _index.end(); it2++)
        {
            if (*it == *it2)
                break;
        }
        if (it2 == _index.end())
            _index.push_back(*it);   
    }
    _cgi_param = request.get_env();
    if (locationName[0] != '*' && config.getaliasSet())
    {
        ret = root + alias + path.substr(locationName.length());
        this->_contentlocation = alias + removeSlaches(path.substr(locationName.length()));
    }
    else
    {
        ret = root + path;
        this->_contentlocation = removeSlaches(path);
    }
    this->_path = removeSlaches(ret);
    std::string indexpath;
    if (!pathIsFile(this->_path) && method == "GET")
    {
        if ((indexpath = this->addindex(request)) != "")
        {
            config = config.getlocationforRequest(indexpath, locationName);
            this->_cgi_pass = config.getCgipass();
            this->_cgi_param = config.getCgiParms();
        }
    }
}

RequestConfiga::~RequestConfiga()
{
    return;
}

const std::string & RequestConfiga::getContentLocation() const 
{
    return this->_contentlocation;
}

const std::string  RequestConfiga::getPath() const 
{
    return this->_path;
}

const std::map<int, std::string> &RequestConfiga::getErrorPage() const 
{
    return this->_error_page;
}

const unsigned long &RequestConfiga::getClientBodyBufferSize() const 
{
    return this->_client_body_size;
}

const std::map<std::string, std::string> &RequestConfiga::getCgiParam() const
{
    return this->_cgi_param;
}
const std::string & RequestConfiga::getCgiPass() const
{
    return this->_cgi_pass;
}
const std::string &RequestConfiga::getLang() const
{
    return this->_lang;
}
const std::set<std::string> &RequestConfiga::getAllowedMethods() const
{
    return this->_allowed_method;
}

const t_listen &RequestConfiga::getHostPort() const
{
    return this->_hostport;
}

bool  RequestConfiga::getAutoIndex() const
{
    return this->autoindex;
}

void  RequestConfiga::setPath(int code)
{
    this->_path = "./location/error/error/";
    this->_path += to_string(code);
}

void RequestConfiga::setPath(const std::string &path)
{
    this->_path = path;
}

void RequestConfiga::setContentLocation(const std::string & path) 
{
    this->_contentlocation = path;
}

void RequestConfiga::setHostPort(const t_listen hostport)
{
    this->_hostport.port = hostport.port;
    this->_hostport.host = hostport.host;
}

std::string RequestConfiga::addindex(Request& request)
{
    std::vector<std::string>::iterator it;
    std::list<std::pair<std::string, float >>::const_iterator lang;
    std::string path;

    it = this->_index.begin();
    while (it != this->_index.end())
    {
         for (lang = request.get_lang().begin(); lang != request.get_lang().end(); lang++)
        {
            path = this->_path;
            if (path[path.size() - 1] != '/')
                path += "/";
            if ((*it).find('.') != (*it).npos)
                path += (*it).substr(0, (*it).find_last_of('.') + 1) + lang->first + (*it).substr((*it).find_last_of('.'));
            if(pathIsFile(path))
            {
                this->_path = path;
                if (this->_contentlocation.size() && this->_contentlocation[this->_contentlocation.size() - 1] != '/')
                    this->_contentlocation += "/";
                if ((*it).find('.') != (*it).npos)
                    this->_contentlocation = (*it).substr(0, (*it).find_last_of('.') + 1) + lang->first + (*it).substr((*it).find_last_of('.'));
                return this->_path;
            }
        }
        it++;
    }
    it = this->_index.begin();
    while (it != this->_index.end())
    {
        path = this->_path;
        if (path[path.size() - 1] != '/')
            path += "/";
        path += *it;
        if (pathIsFile(path))
        {
            this->_path = path;
            if (this->_contentlocation[this->_contentlocation.size() - 1] != '/')
                this->_contentlocation += "/";
            this->_contentlocation += *it;
            return this->_path;
        }
        it++;
    }
    return "";
}