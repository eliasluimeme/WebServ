#pragma once

#include "../Includes/Includes.hpp"
#include "../conf/ConfigServer.hpp"

class ConfigServer;

struct confData {
	std::vector<ConfigServer> server;
};

struct Listen {
	std::string ip;
	std::string port;
};

struct Data {
	std::vector<std::string>           		serverName;
	std::vector<Listen> 					listen;
	int 									bodySize;
	bool 									autoIndex;
	std::string								uploadPass;
	std::string 							root;
	std::vector<std::string> 				index;
	std::set<std::string> 					methods;
	std::vector<std::string> 				cgi;
	std::map<int, std::string>				errorPages;
	std::map<std::string, Data>		 		locations;
};
