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

// bool Config::checkfile(filevector file)
// {
//     size_t comma;
//     for ( filevector::iterator i = file.begin() ;i < file.end(); i++)
//     {
//         if (i->data() == "server" || i->data() == "{")
//         {
            
//         }
        
//         if (comma = i->find(";") == std::string::npos)
//             return false;
//     }
//     return true;
// }

int Config::parse(const char *filename)
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
    std::cout << this->_server[0] << '\n';  
    exit(1);

    return 0;
}

// RequestConfiga Config::getConfigForRequest(t_listen const adress,std::string const uri, std::string hostname, const std::string &method, Request &request) const
//     {
//         ConfigServer server;
//         std::string locationPath;

//         hostname = hostname.substr(0, hostname.find_last_of(':'));
//         this->getServerforRequest(server, adress, hostname);
//         server = server.getlocationforRequest(uri, locationPath);
//         if (*(--locationPath.end()) == '/')
//             locationPath.resize(locationPath.size() - 1);
//         RequestConfiga config(server, request, uri, method, locationPath);
//         config.setHostPort(adress);
//         return config;
//     }

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


