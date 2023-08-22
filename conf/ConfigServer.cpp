#include "ConfigServer.hpp"

//constructors

ConfigServer::ConfigServer():
_root(""),
_client_body_buffer_size(0),
_autoindex(false),
_aliasSet(false)
{
    return ; 
}

ConfigServer::~ConfigServer()
{
    return ;
}

ConfigServer ConfigServer::_defaultServer = ConfigServer();

parseMap ConfigServer::_initServerMap()
{
    parseMap map;
    map["listen"] = &ConfigServer::addlisten;
    map["root"] = &ConfigServer::addRoot;
    map["server_name"] = &ConfigServer::addServerName;
    map["error_page"] = &ConfigServer::addErrorPage;
    map["client_body_buffer_size"] = &ConfigServer::addclientbodybuffersize;
    map["cgi_param"] = &ConfigServer::addCgiparams;
    map["cgi_pass"] = &ConfigServer::addCgipass;
    map["allow_methods"] = &ConfigServer::addAllowedmethod;
    map["index"] = &ConfigServer::addIndex;
    map["autoindex"] = &ConfigServer::addAutoindex;
    return map;
}

parseMap ConfigServer::serverParsingMap = ConfigServer::_initServerMap();

parseMap ConfigServer::_initLocationMap()
{
    parseMap  map;
    map["root"] = &ConfigServer::addRoot;
    map["error_page"] = &ConfigServer::addErrorPage;
    map["client_body_buffer_size"] = &ConfigServer::addclientbodybuffersize;
    map["cgi_param"] = &ConfigServer::addCgiparams;
    map["cgi_pass"] = &ConfigServer::addCgipass;
    map["allow_methods"] = &ConfigServer::addAllowedmethod;
    map["index"] = &ConfigServer::addIndex;
    map["autoindex"] = &ConfigServer::addAutoindex;
    map["alias"] = &ConfigServer::addAlias;
    return map;
}

parseMap ConfigServer::locationParsingMap = ConfigServer::_initLocationMap();

ConfigServer ConfigServer::_initserverDefault(const char * filename)
{
    ConfigServer server;
    filevector file;

    // std::cout << filename << std::endl;
    file =  ReaderConf::readfile(filename);
    if (file.empty())
    {
        std::cerr << RED << "could not open default file at location nmm" << filename << std::endl;
        throw ReaderConf::FileNotfoundException();
    }
    // exit(1);
    filevector begin;
    begin.push_back("server");
    begin.push_back("{");
    file.insert(file.begin(), begin.begin(), begin.end());
    file.insert(file.end(), "}");
    // exit(1);
    unsigned int index = 2;
    if (!server.parseServer(index, file))
    {
        
        std::cerr << RED << "invalid default config file" << std::endl;
        throw ConfigServer::InvalidArgumentsException();
    }
    ConfigServer::_defaultServer = server;
    return server;
}

// parsing config file
int ConfigServer::parseServer(unsigned int &index, filevector &file)
{
    filevector  args;
    parseMap::iterator it;
    std::string directive;
    // std::cout << "i = " << index << std::endl;
    //calling the function that corresponds to a directive with its arg as parameters
    for ( ;index < file.size() && file[index] != "}"; index++)
    {
        if ((it = ConfigServer::serverParsingMap.find(file[index])) == ConfigServer::serverParsingMap.end())
        {
            if (file[index] == "location")
            {
                // std::cout << "directive location  " << directive << std::endl;
                ConfigServer location;
                std::string locationName;

                if (directive != "")
                {
                    (this->*ConfigServer::serverParsingMap[directive])(args);
                    args.clear();
                    directive = "";
                }
                index++;
                if (file[index] == "{" || file[index] == "}")
                    return 0;
                locationName = file[index];
                index++;
                if (!location.parseLocation(index, file))
                    return 0;
                this->_location[locationName] = location;
                if (file[index] == "}")
                    continue;
            }
            else if (!directive.compare(""))
                return file[index] == "}" ? 1 : 0;
            else
            {
                args.push_back(file[index]);
            }
        }
        else
        {
            // std::cout << GREEN << "this is dir : "<< directive << std::endl;
            if (directive != "")
            {
                (this->*ConfigServer::serverParsingMap[directive])(args);
                args.clear();
            }
            directive = it->first;
        }
    }
    if (directive != "")
        (this->*ConfigServer::serverParsingMap[directive])(args);
    if (!file[index].compare("}"))
    {
        ConfigServer::_defaultServer.passMembers(*this);
        for (std::map<std::string, ConfigServer>::iterator i = this->_location.begin() ; i != this->_location.end(); i++)
            this->passMembers(i->second);
        return 1;
    }
    return 0;
}

void    ConfigServer::passMembers(ConfigServer &server) const 
{
    if (this != &server)
    {
        if (server._listen.empty())
            server._listen.insert(server._listen.begin(), this->_listen.begin(), this->_listen.end());
        if (server._root == "")
            server._root = this->_root;
        server._server_name.insert(server._server_name.end(), this->_server_name.begin(), this->_server_name.end());
        for (std::map<int , std::string>::const_iterator i = this->_error_page.begin(); i != this->_error_page.end(); i++)
        {
            if (server._error_page.find(i->first) == server._error_page.end())
                server._error_page[i->first] = i->second;
        }
        if (server._client_body_buffer_size == 0)
            server._client_body_buffer_size = this->_client_body_buffer_size;
        for (std::map<std::string, std::string>::const_iterator i = this->_cgi_param.begin(); i != this->_cgi_param.end(); i++)
        {
            if (server._cgi_param.find(i->first) == server._cgi_param.end())
                server._cgi_param[i->first] = i->second;
        }
        if (server._cgi_pass == "")
            server._cgi_pass = this->_cgi_pass;
        if (server._allowed_methods.empty())
            server._allowed_methods = this->_allowed_methods;
        server._index.insert(server._index.begin(), this->_index.begin(), this->_index.end());
    }
    for (std::map<std::string, ConfigServer>::iterator i = server._location.begin(); i != server._location.end(); i++)
        server.passMembers(i->second); 
}
int     ConfigServer::parseLocation(unsigned int &index, filevector &file)
{
    filevector  args;
    parseMap::iterator it;
    std::string  directive = "";

    if (file[index++] != "{")
        return 0;
    for ( ;index < file.size() && file[index] != "}"; index++)
    {
        if ((it = ConfigServer::locationParsingMap.find(file[index])) == ConfigServer::locationParsingMap.end())
        {
            if (file[index] == "location")
            {
                ConfigServer    location;
                std::string  locationName;

                if (directive != "")
                {
                    (this->*ConfigServer::locationParsingMap[directive])(args);
                    args.clear();
                    directive = "";
                }
                index++;
                if (file[index] == "{" || file[index] == "{")
                    return 0;
                locationName = file[index];
                index++;
                if (!location.parseLocation(index, file))
                    return 0;
                this->_location[locationName] = location;
                if (file[index] == "}")
                    continue;
            }
            else if(directive == "")
                return file[index] == "}" ? 1 : 0;
            else
                args.push_back(file[index]);
        }
        else
        {
            if (directive != "")
                {
                    (this->*ConfigServer::locationParsingMap[directive])(args);
                    args.clear();
                }
            directive = it->first;
        }
    }
    if (directive != "")
        (this->*ConfigServer::locationParsingMap[directive])(args);
    if (!file[index].compare("}"))
        return 1;
    return 0;
}

//AddMember funtion

void ConfigServer::addlisten(std::vector<std::string> args)
{
    t_listen listen;
    size_t sep;

    if (args.size() != 1)
        throw ConfigServer::InvalidArgumentsException();
    if ((sep = args[0].find(":")) == std::string::npos)
    {
        if (isDigits(args[0]))
        {
            listen.host = 0;
            listen.port = atoi(args[0].c_str());
            for (std::vector<t_listen>::const_iterator it  = _listen.begin(); it != _listen.end(); it++)
            {
                if (it->port == listen.port)
                    throw ConfigServer::InvalidArgumentsException();
            }
            this->_listen.push_back(listen);
            return;
        }
        throw ConfigServer::InvalidArgumentsException();
    }
    else
    {
        listen.host = strToIp(args[0].substr(0, sep));
        sep++;
        std::string portstr = args[0].substr(sep);
        if (isDigits(portstr))
        {
            listen.port = atoi(portstr.c_str());
            this->_listen.push_back(listen);
            return ;
        }
        throw ConfigServer::InvalidArgumentsException();
    }
}

void ConfigServer::addRoot(std::vector<std::string> args)
{
    if (args.size() != 1 || this->_root != "")
        throw ConfigServer::InvalidArgumentsException();
        
    this->_root = args[0];
}

void ConfigServer::addServerName(std::vector<std::string> args)
{
    if(args.size() == 0)
        throw ConfigServer::InvalidArgumentsException();
    for (unsigned int i = 0; i < args.size(); i++)
        this->_server_name.push_back(args[i]);
}

void    ConfigServer::addErrorPage(std::vector<std::string> args)
{
    std::vector<int> codes;
    std::string     uri = "";
    size_t  len = args.size();

    for (size_t i = 0; i < len; i++)
    {
        if (isDigits(args[i]))
            codes.push_back(atoi(args[i].c_str()));
        else if (codes.empty())
            throw ConfigServer::InvalidArgumentsException();
        else if (i == len - 1)
            uri = args[i];
        else
            throw ConfigServer::InvalidArgumentsException();
    }
    if (uri == "")
        throw ConfigServer::InvalidArgumentsException();
    for (std::vector<int>::iterator i = codes.begin(); i != codes.end(); i++)
        this->_error_page[*i] = uri;
}

void    ConfigServer::addclientbodybuffersize(std::vector<std::string> args)
{
    if (args.size() != 1 || !isDigits(args[0]))
        throw ConfigServer::InvalidArgumentsException();
    this->_client_body_buffer_size = atoi(args[0].c_str());
}

void    ConfigServer::addCgiparams(std::vector<std::string> args)
{
    if (args.size() != 2)
        throw ConfigServer::InvalidArgumentsException();
    this->_cgi_param[args[0]] = args[1];
}

void ConfigServer::addCgipass(std::vector<std::string> args)
{
    if (args.size() != 1)
        throw ConfigServer::InvalidArgumentsException();
    this->_cgi_pass = args[0];
}
void ConfigServer::addAllowedmethod(std::vector<std::string> args)
{
    if (args.empty())
        throw ConfigServer::InvalidArgumentsException();
    this->_allowed_methods.clear();
    for (filevector::iterator i = args.begin(); i < args.end(); i++)
        this->_allowed_methods.insert(*i);
}

void ConfigServer::addIndex(std::vector<std::string> args)
{
    if (args.empty())
        throw ConfigServer::InvalidArgumentsException();
    this->_index.insert(this->_index.end(), args.begin(), args.end());
}

void    ConfigServer::addAlias(std::vector<std::string> args)
{
    if (args.size() > 1)
        throw ConfigServer::InvalidArgumentsException();
    if (args.size())
        this->_alias = args[0];
    this->_aliasSet = true;
}

void ConfigServer::addAutoindex(std::vector<std::string> args)
{
    if (args.size() != 1)
        throw ConfigServer::InvalidArgumentsException();
    if (args[0] == "on")
        this->_autoindex = true;
    if (args[0] == "off")
        this->_autoindex = false;
    else
        throw ConfigServer::InvalidArgumentsException();
}


std::string ConfigServer::get_root() const
{
    return this->_root;
}


std::vector<t_listen> ConfigServer::get_listen() const 
{
    return this->_listen;
}

std::vector<std::string> ConfigServer::get_server_name() const
{
    return this->_server_name;
}

std::map<int, std::string> ConfigServer::get_errorPage() const
{
    return this->_error_page;
}

int ConfigServer::getclientbodyBuffersize() const
{
    return this->_client_body_buffer_size;
}

std::map<std::string, std::string> ConfigServer::getCgiParms() const
{
    return this->_cgi_param;
}

std::string ConfigServer::getCgipass() const
{
    return this->_cgi_pass;
}

std::map<std::string, ConfigServer> ConfigServer::getLocation() const
{
    return this->_location;
}

std::set<std::string> ConfigServer::getAllowedmethod() const
{
    return this->_allowed_methods;
}

std::vector<std::string> ConfigServer::getIndex() const
{
    return this->_index;
}

bool ConfigServer::getAutoindex() const
{
    return this->_autoindex;
}

std::string ConfigServer::getalias() const
{
    return this->_alias;
}

bool ConfigServer::getaliasSet() const 
{
    return this->_aliasSet;   
}
ConfigServer & ConfigServer::getDefaultServer()
{
    return ConfigServer::_defaultServer;
}

ConfigServer  ConfigServer::getlocationforRequest(std::string const path, std::string &locationpath)
{
    std::string::size_type  len = path.size();
    std::map<std::string, ConfigServer>::iterator it;
    std::string  try_location;

    if (!len)
        return *this;
    if (!this->_location.empty())
    {
        do 
        {
            try_location = path.substr(0, len);
            it = this->_location.find(try_location);
            if (it != this->_location.end() && it->first[0] != '*')
            {
                locationpath = try_location;
                return it->second.getlocationforRequest(path, locationpath);
            }
            len--;
        }while(len);
        for (std::map<std::string,ConfigServer>::iterator i = this->_location.begin(); i != this->_location.end(); i++)
        {
            if (i->first[0] == '*')
            {
                std::string suffix = i->first.substr(1);
                if (path.length() > suffix.length() && !path.compare(path.length() - suffix.length(), suffix.length(),suffix))
                {
                    return i->second.getlocationforRequest(path, locationpath);
                }
            }
        }
    }
    return *this;
}

std::ostream	&operator<<(std::ostream &out, const ConfigServer &server) {
	out << "Listen:" << std::endl;
	for (size_t i = 0; i < server._listen.size(); i++) {
		out << "\thost: " << server._listen[i].host << " port: " << server._listen[i].port << std::endl;
	}
	out << "root: " << server._root << std::endl;
	out << "server_name: ";
	for (size_t i = 0; i < server._server_name.size(); i++) {
		out << server._server_name[i];
		if (i != server._server_name.size() - 1)
			out << " ";
	}
	out << std::endl<< "error_page:" << std::endl;
	for (std::map<int, std::string>::const_iterator i = server._error_page.begin(); i != server._error_page.end(); i++) {
		out << "\t" << i->first << " " << i->second << std::endl;
	}
	out << "client_body_buffer_size: " << server._client_body_buffer_size << std::endl;
	out << "cgi_param:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator i = server._cgi_param.begin(); i != server._cgi_param.end(); i++)
		out << "\t" << i->first << " = " << i->second << std::endl;
	out << "cgi_pass:	" << server._cgi_pass << std::endl;
	out << "allowed methods: ";
	for (std::set<std::string>::iterator i = server._allowed_methods.begin(); i != server._allowed_methods.end(); i++)
		out << " " << *i;
	out << std::endl;
	out << "autoindex " << (server._autoindex ? "on" : "off") << std::endl;
	out << "index: ";
	for (filevector::const_iterator i = server._index.begin(); i != server._index.end(); i++)
		out << *i << " ";
	out << std::endl;
	out << "alias: " << server._alias << std::endl;
	for (std::map<std::string, ConfigServer>::const_iterator i = server._location.begin(); i != server._location.end(); i++) {
		out << std::endl << "LOCATION " << i->first << std::endl;
		out << i->second << std::endl;
	}
	return out;
}



