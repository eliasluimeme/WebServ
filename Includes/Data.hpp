#pragma once

#include "../Includes/Includes.hpp"

class ConfigServer;

struct confData {
	std::vector<ConfigServer> server;
};

struct Data {
	std::string             serverName;
	std::map<std::string, std::string> listen;
	int bodySize;
	std::string root;
	std::string index;
	std::vector<std::string> methods;
	std::vector<std::string> cgi;
	std::map<int, std::string> errorPages;
	std::map<std::string, std::map<std::string, std::string> > locations;

    // std::string             port;
	// std::string				host;
	// std::string				upload;
    // std::vector<std::string>            locations;
};

std::vector<Data> servers;

struct Data& setupData() {
	Data data;

	data.serverName = ;
}