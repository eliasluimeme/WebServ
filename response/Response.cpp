#include "Response.hpp"

std::map<std::string , void (Response::*) (Request &, RequestConfiga &)> Response::initMethods()
{
    std::map<std::string, void (Response::*) (Request &, RequestConfiga &)> map;
    map["GET"] = &Response::GetMethod;
    map["POST"] = &Response::PostMethod;
    map["DELETE"] = &Response::DeleteMethod;
    return map;
}

std::map<std::string, void (Response::*) (Request &, RequestConfiga &)> Response::method = Response::initMethods();

void Response::GetMethod(Request & request, RequestConfiga & RequestConfig)
{
    HeaderRes header;
    
    if (RequestConfig.getCgiPass() != "")
    {
        //cgi handler code here
    }
    
    else if (code == 200)
        code = readContent();
    else
        _response = this->readHtml(error_map[code]);
    if (code == 500)
        _response = this->readHtml(error_map[code]);
    _response = header.getHeader(_response.size(), path, code, _type, RequestConfig.getContentLocation(), RequestConfig.getLang()) + "\r\n" + _response;
}

std::string Response::readHtml(const std::string &path)
{
    std::ofstream file;
    std::stringstream buffer;

    if (pathIsFile(path))
    {
        file.open(path.c_str(), std::ifstream::in);
        if (file.is_open() == false)
            return "!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n";
        buffer << file.rdbuf();
        file.close();
        _type = "text/html";
        return buffer.str();
    }
    else
        return "!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n";
}

int Response::pathIsfile(std::string &path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0)
    {
        if (S_ISDIR(s.st_mode))
            return 0;
        else if (S_ISREG(s.st_mode))
            return 1;
    }
    return 0;
}

std::string Response::getPage(const char *path, std::string &host, int port)
{
    std::string dirname(path);
    DIR *dir = opendir(path);
    std::string page =\
    "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
            <title>" + dirname + "</title>\n\
    </head>\n\
    <body>\n\
    <h1>INDEX</h1>\n\
    <p>\n";
    if (dir == NULL)
    {
        std::cerr << RED << "error : could not open ["<< path << "]" << std::endl;
        return "";
    }
    if (dirname[0] != '/')
        dirname = "/" + dirname;
    for (struct dirent * direntry= readdir(dir); direntry; direntry= readdir(dir))
    {
        page += getLink(std::string(direntry->d_name), dirname, host, port);
    }
    page += "\
    </p>\n\
    </body>\n\
    </html>\n";
    closedir(dir);
    return page;
}

std::string Response::getLink(const std::string &direntry, std::string const &dirname, std::string const &host, int port)
{
    std::stringstream ss;
    ss << "\t\t<p><a href=\"http://" + host + ":" <<\
        port << dirname + "/" + direntry + "\">" + direntry + "</a></p>\n"; 
    return ss.str();
}

int Response::readContent()
{
    std::ifstream file;
    std::stringstream buffer;

    _response = "";
    if(pathIsfile(path))
    {
        file.open(path.c_str() , std::ifstream::in);
        if (file.is_open() == false)
        {
            _response = this->readHtml(error_map[403]);
            return 403;
        }
        buffer << file.rdbuf();
        _response = buffer.str();
        file.close();
    }
    else if (_isAutoIndex)
    {
        buffer << getPage(path.c_str(), _hostPort.host,_hostPort.port);
        _response = buffer.str();
        _type = "text/html";
    }
    else
    {
        _response = this->readHtml(error_map[404]);
        return 404;
    }
    return 200;
}
void Response::getLocationMatch(std::string &path, std::map<std::string ,ConfigServer> locations, std::string &location_key)
{
    size_t match = 0;

    for(std::map<std::string , ConfigServer>::const_iterator it = locations.begin(); it != locations.end(); ++it)
    {
        if(path.find(it->first) == 0)
        {
               if( it->first == "/" || path.length() == it->first.length() || path[it->first.length()] == '/')
               {
                    if(it->first.length() > match)
                    {
                        match = it->first.length();
                        location_key = it->first;
                    }
               }
        }
    }
}