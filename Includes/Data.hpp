#pragma once

#include "../Includes/Includes.hpp"
#include "../conf/ConfigServer.hpp"

class ConfigServer;

struct confData {
	std::vector<ConfigServer> server;
};

struct Data {
	std::vector<std::string>           		serverName;
	std::map<std::string, std::string> 		listen;
	int 									bodySize;
	bool 									autoIndex;
	std::string								uploadPass;
	std::string 							root;
	std::vector<std::string> 				index;
	std::vector<std::string> 				methods;
	std::vector<std::string> 				cgi;
	std::map<int, std::string>				errorPages;
	std::map<std::string, Data>		 		locations;
};

// std::vector<Data> serverData;

// std::vector<Data>& setupData(confData &config) {
// 	for (int i = 0; i < config.server.size(); i++) {
// 		Data data;

// 		std::vector<std::string> name = config.server[i].get_server_name();
// 		for (std::vector<std::string>::iterator it = name.begin(); it != name.end(); it++)
// 			data.serverName.push_back(*it);
// 		for (int index = 0; index < config.server[i].get_listen().size(); index++) {
// 			data.listen[config.server[i].get_listen()[index].host] = config.server[i].get_listen()[index].port;
// 		}

// 		data.bodySize = config.server[i].getclientbodyBuffersize();
// 		data.autoIndex = config.server[i].getAutoindex();
// 		data.root = config.server[i].get_root();

// 		std::vector<std::string> indx = config.server[i].getIndex();
// 		for (std::vector<std::string>::iterator it = indx.begin(); it != indx.end(); it++)
// 			data.index.push_back(*it); //

// 		std::set<std::string> metho= config.server[i].getAllowedmethod();
// 		for (std::set<std::string>::iterator it = metho.begin(); it != metho.end(); it++)
// 			data.methods.push_back(*it);

// 		data.cgi.push_back(config.server[i].getCgipass()); // todo get the whole pass + param 
// 		data.errorPages = config.server[i].get_errorPage();

// 		std::map<std::string, ConfigServer> loc = config.server[i].getLocation();
// 		for (std::map<std::string, ConfigServer>::iterator it = loc.begin(); it != loc.end(); it++) {
// 			Data locData;

// 			locData.bodySize = it->second.getclientbodyBuffersize();
// 			locData.autoIndex = it->second.getAutoindex();
// 			locData.root = it->second.get_root();

// 			std::vector<std::string> ind = it->second.getIndex();
// 			for (std::vector<std::string>::iterator itindex = ind.begin(); itindex != ind.end(); itindex++)
// 				locData.index.push_back(*itindex);
			
// 			locData.cgi.push_back(it->second.getCgipass()); // todo get the whole pass + param 

// 			std::pair<std::string, Data> pair(it->first, locData);
// 			data.locations.insert(pair);
// 		}

// 		serverData.push_back(data);
// 	}
// 	return serverData;
// }