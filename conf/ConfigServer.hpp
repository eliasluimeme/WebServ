#if !defined(CONFIGSERVER_HPP)
#define CONFIGSERVER_HPP

#include "../Includes/Includes.hpp"
#include "config.hpp"

class ConfigServer
{
private:
    int     parseLocation(unsigned int &index, filevector &file);
    void    addlisten(std::vector<std::string> args);
    void    addRoot(std::vector<std::string> args);
    void    addServerName(std::vector<std::string> args);
    void    addErrorPage(std::vector<std::string> args);
    void    addclientbodybuffersize(std::vector<std::string> args);
    void    addCgiparams(std::vector<std::string> args);
    void    addCgipass(std::vector<std::string> args);
    void    addAllowedmethod(std::vector<std::string> args);
    void    addIndex(std::vector<std::string> args);
    void    addAlias(std::vector<std::string> args);
    void    addAutoindex(std::vector<std::string> args);

    std::vector<t_listen>				_listen;
	std::string							_root;


	std::vector<std::string>   			_server_name;
	std::map<int, std::string>			_error_page; // error page redirections
	int									_client_body_buffer_size; // max size for the client body, defaults to 8 000
	std::map<std::string, std::string>	_cgi_param;
	std::string							_cgi_pass;
	std::map<std::string, ConfigServer> _location;
	std::set<std::string>				_allowed_methods;
	std::vector<std::string>			_index;
	bool								_autoindex;
	std::string     					_alias;
	bool								_aliasSet;
	static  ConfigServer				_defaultServer;
	static  parseMap					serverParsingMap;
	static  parseMap					locationParsingMap;
	static  parseMap 					_initServerMap();
	static  parseMap 					_initLocationMap();

public:
    ConfigServer();
    virtual ~ConfigServer();

    int     parseServer(unsigned int &index ,filevector &file);
    void    passMembers(ConfigServer &server) const;

    class InvalidArgumentsException : public std::exception
    {
        virtual const char *what() const throw()
        {
            return "invalide argument exception in configuration file"; 
        }
    };
    static ConfigServer _initserverDefault(const char* filename);
    //getters 
    std::vector<t_listen> get_listen() const;
    std::string     get_root() const;
    std::vector<std::string>    get_server_name() const;
    std::map<int, std::string> get_errorPage() const ;
    int                          getclientbodyBuffersize() const;
    std::map<std::string, std::string>  getCgiParms() const;
    std::string getCgipass()const;
    std::map<std::string, ConfigServer> getLocation() const;
    std::set<std::string> getAllowedmethod() const;
    std::vector<std::string> getIndex()const;
    bool getAutoindex()const;
    std::string getalias()const;
    bool getaliasSet()const;
    static ConfigServer &getDefaultServer();

    ConfigServer    getlocationforRequest(std::string const path, std::string &locationPath);
    friend std::ostream& operator <<(std::ostream& out, const ConfigServer& server);
};



#endif // MACRO
