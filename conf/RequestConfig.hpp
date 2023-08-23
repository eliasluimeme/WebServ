#if !defined(REQUEST_CONFIG_HPP)
#define REQUEST_CONFIG_HPP

// #include "../include/webserv.hpp"
// #include "../request/Request.hpp"
#include "config.hpp"

class RequestConfiga
{
    private:
        std::string _contentlocation; //public part of the path
        std::string _path; // local path for request
        std::map<int, std::string> _error_page; //error page redirection 
        unsigned long  _client_body_size; // max size fo the client body, default 8000 
        std::map<std::string, std::string> _cgi_param;
        std::string _cgi_pass;
        std::set<std::string> _allowed_method; //allowed http methods for request 
        t_listen _hostport;
        std::string _lang;
        bool   autoindex;

        std::vector<std::string> _index; // THE SERVER WILL SEARCH FOR THESE FILE IN THE _path DIRECTORIES AND FETCH THE FIRST ONE FOUND
        // bool  autoindex; // THE SERVER WILL GENERATE A LIST OF ALL THE FILES IN THE DIRECTORY, MUCH LIKE THE NGINX DIRECTIVE
    public:  
        RequestConfiga();
        RequestConfiga(ConfigServer &config, Request &request, const std::string &path, const std::string &method, std::string &locationName);
        virtual ~RequestConfiga();

        std::string addindex(Request& request);
        //getters 
        const std::string&							getContentLocation() const;
		const std::string					getPath() const;
		const std::map<int, std::string>			&getErrorPage() const;
		const unsigned long							&getClientBodyBufferSize() const;
		const std::map<std::string, std::string>	&getCgiParam() const;
		const std::string&							getCgiPass() const;
		const std::string&							getLang() const;
		const std::set<std::string>				&getAllowedMethods() const;
		const t_listen							&getHostPort() const;
		bool  									getAutoIndex() const;


        //setters

        void								setPath(int code);
		void								setPath(const std::string &path);
		void								setContentLocation(const std::string&);
		void								setHostPort(const t_listen hostPort);
};


#endif 
