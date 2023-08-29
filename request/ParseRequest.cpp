#include "Request.hpp"

void Request::displayHeader() const
{

}

std::ostream& operator<<(std::ostream& os, const Request& re)
{
    std::map<std::string, std::string>::const_iterator it;
    os << "Method : " << re.getMethod() << "\t HTTP version : ";
    os << re.getversion() << std::endl;
    os << "Port :" << re.get_port()<< '\n';
    os << "path :" << re.get_path() << '\n';
    for (it = re.get_header().begin(); it != re.get_header().end(); it++)
        os << it->first << ":" << it->second << '\n';
    os << "body :" << re.get_body() << std::endl;
    return os;
}

void    Request::resetHeader()
{
    this->_header.clear();

    this->_header["Accept-Charsets"] = "";
    this->_header["Accept-Language"] = "";
    this->_header["Allow"] = "";
    this->_header["Auth-Scheme"] = "";
    this->_header["Authorization"] = "";
    this->_header["Content-Language"] = "";
    this->_header["Content-Type"] = "";
    this->_header["Content-Length"] = "";
    this->_header["Content-Location"] = "";
    this->_header["Location"] = "";
    this->_header["Host"] = "";
    this->_header["Date"] = "";
    this->_header["User-Agent"] = "";
    this->_header["Www-Authenticate"] = "";
    this->_header["Connection"] = "Keep-Alive";
}

int Request::readFirstLine(const std::string & str)
{
    size_t i;
    std::string line;

    i = str.find_first_of('\n');
    line = str.substr(0, i);
    i = line.find_first_of(' ');
    std::cout << i << std::endl;
    if (i == std::string::npos)
    {
        this->ret = 400;
        std::cerr << RED << "no space after method" << std::endl;
        return 400;
    }
    this->_method.assign(line, 0, i);
    std::cout << this->_method << std::endl;
    return this->readPath(line, i);
}

int Request::readPath(const std::string line, size_t i)
{
    size_t j;

    if ((j = line.find_first_not_of(' ', i)) == std::string::npos)
    {
        this->ret = 400;
        std::cerr << RED << "NO Path / HTTTP version" << std::endl;
        return 400;
    }
    if ((i = line.find_first_of(' ', j)) == std::string::npos)
    {
        this->ret = 400;
        std::cerr << RED << "No HTTP version " << std::endl;
        return 400;
    }
    this->path.assign(line, j, i - j);
    std::cout << path << std::endl;
    return this->readVersion(line, i);
}

int Request::readVersion(const std::string& line , size_t i)
{
    if ((i = line.find_first_not_of(' ', i)) == std::string::npos)
    {
        this->ret = 400;
        std::cerr << RED << "No HTTP version" << std::endl;
        return 400;
    }
    if (line[i] == 'H' && line[i + 1] == 'T' && line[i + 2] == 'T' &&
             line[i + 3] == 'P' && line[i + 4] == '/')
        this->_version.assign(line, i + 5, 3);
    std::cout <<"version :" << _version << std::endl;
    if (this->_version != "1.1" && this->_version != "1.0")
    {
        this->ret = 400;
        std::cerr << RED << "bad HTTP version ("<< this->_version <<")" << std::endl;
        return 400;
    }
    return this->checkMethod();
}

int Request::checkMethod()
{
    for (size_t i = 0; i < this->methods.size(); i++)
        if (this->methods[i] == this->_method)
            return this->ret;
    std::cerr << RED << "invalid method Requested" << std::endl;
    this->ret = 400;
    return 400;
}

int Request::Checkport()
{
    size_t i = this->_header["Host"].find_first_of(':');
    if (i == std::string::npos)
        this->_port = 80;
    else
    {
        std::string tmp(this->_header["Host"], i + 1);
        this->_port = atoi(tmp.c_str());
    }
    return this->_port;
}

std::string Request::nextLine(const std::string &str, size_t& i)
{
    std::string ret;
    size_t j;

    if (i == std::string::npos)
        return "";
    // std::cout << i << '\n';
    j = str.find_first_of('\n', i);
    ret = str.substr(i, j - i);
    if (ret[ret.size() - 1] == '\r')
        pop(ret);
    i = (j == std::string::npos ? j : j + 1);
    return ret;
}

int Request::parse(const std::string &str)
{
    std::string key;
    std::string value;
    std::string line;
    size_t i(0);

    this->readFirstLine(nextLine(str, i));
    while ((line = nextLine(str, i)) != "\r" && line != "" && this->ret != 400) 
	{
        std::cout << i << std::endl;
		key = readKey(line);
		value = readValue(line);
        std::cout << "value :" << value << '\n';
		if (this->_header.count(key))
				this->_header[key] = value;
		if (key.find("Secret") != std::string::npos)
			this->_envCgi[formatheaderforCgi(key)] = value;
	}
    std::cout << i << std::endl;
	if (this->_header["Www-Authenticate"] != "")
		this->_envCgi["Www-Authenticate"] = this->_header["Www-Authenticate"];
	this->setLang();
    if (i != std::string::npos)
    {
        this->setBody(str.substr(i, std::string::npos));
    }
	findQuery();
	return this->ret;
}

void Request::setLang()
{
    std::vector<std::string> token;
    std::string header;
    std::vector<std::string>::iterator it;
    size_t i;
    
    if ((header = this->_header["Accept-Language"]) != "")
    {
        float weight = 0.0;
        token = split(header, ',');
        for (it = token.begin(); it != token.end(); it++)
        {
            std::string lang;
            lang = (*it).substr(0, (*it).find_first_of('-'));
            strip(lang, ' ');
            if ((i = lang.find_last_of(';')) != std::string::npos)
                weight = atof((*it).substr(i + 4).c_str());
            lang.resize(i > 2 ? 2 : i);
            this->_lang.push_back(std::pair<std::string, float>(lang, weight));
            
        }
        this->_lang.sort(compare_langs);
    }
}

void Request::findQuery()
{
    size_t i;
    i = this->path.find_first_of('?');
    if (i != std::string::npos)
    {
        this->_query.assign(this->path, i + 1, std::string::npos);
        this->path = this->path.substr(0, i);
    }
}

void Request::stripAll()
{
    strip(this->_method, '\n');
    strip(this->_method, '\r');
    strip(this->_method, '\0');
    strip(this->_version, '\n');
    strip(this->_version, '\r');
    strip(this->_version,  ' ');
    strip(this->path, '\n');
    strip(this->path, '\r');
    strip(this->path, ' ');
}


std::string Request::formatheaderforCgi(std::string &key)
{
    to_upper(key);

    for (size_t i = 0; i < key.size(); i++)
    {
        if (key[i] == '-')
            key[i] = '_';
    }
    return "HTTP_" + key;
}
