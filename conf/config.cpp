#include "config.hpp"

Config::Config(std::string defaultserverpath)
{
    std::cout << defaultserverpath << std::endl;
    try
    {
        ConfigServer::_initserverDefault(defaultserverpath.c_str());
    }
    catch(ReaderConf::FileNotfoundException &e)
    {
        std::cerr << RED << "could not open default config filea" << '\n';
    }
    return ;
}

Config::~Config()
{
    return ;
}

std::vector<ConfigServer> Config::getServer() const
{
    return this->_server;
}

int Config::parse(const char *filename, confData &data)
{
    filevector file;
    unsigned int filesize;

    file = ReaderConf::readfile(filename);
    filesize = file.size();
    for (unsigned int i = 0; i < filesize; i++)
    {
        if (file[i] == "server")
        {
            ConfigServer server;
            ++i;
            if (file[i] != "{")
            {
                std::cerr << RED << "Error : Expected '{' after server directive." << std::endl;
                return 1;
            }
            ++i;
            if (!server.parseServer(i, file))
            {
                std::cerr << RED << "Error : error in config file ["<< filename << "]" << std::endl;
                return 1;
            }
            this->_server.push_back(server);
        }
        else
        {
            std::cerr << "Error : Unknown directive ["<< file[i] << "]" << std::endl;
            return 1;
        }   
    }
    data.server = this->_server;

    return 0;
}

bool Config::getServerforRequest(ConfigServer &ret, t_listen const address, std::string const hostName) const 
{
    std::vector<ConfigServer> possibleServers;
    for (std::vector<ConfigServer>::const_iterator serveriter = this->_server.begin(); serveriter != this->_server.end(); serveriter++)
    {
        std::vector<t_listen> listen = serveriter->get_listen();
        for (std::vector<t_listen>::iterator listniter = listen.begin(); listniter < listen.end() ; listniter++)
        {
            if (address.host == (*listniter).host && address.port == (*listniter).port)
                possibleServers.push_back((*serveriter));
        }
    }
    if (possibleServers.empty())
        return false;
    for ( std::vector<ConfigServer>::iterator servreiter = possibleServers.begin(); servreiter != possibleServers.end(); servreiter++)
    {
        std::vector<std::string> servernames = servreiter->get_server_name();
        for (filevector::iterator servernameIter= servernames.begin(); servernameIter !=servernames.end(); servernameIter++)
        {
            if (*servernameIter == hostName)
            {
                ret = *servreiter;
                return true;
            }
        }
    }
    ret = possibleServers[0];
    return true;
}


