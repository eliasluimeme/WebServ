#include "../Includes/Includes.hpp"
#include "../conf/webserv.hpp"

std::vector<Data> setupData(confData &config) { // TODO check errors
    std::vector<Data> serverData;

	for (int i = 0; i < config.server.size(); i++) {
		Data data;

		std::vector<std::string> name = config.server[i].get_server_name();
		for (std::vector<std::string>::iterator it = name.begin(); it != name.end(); it++)
			data.serverName.push_back(*it);
		for (int index = 0; index < config.server[i].get_listen().size(); index++) {
			data.listen[config.server[i].get_listen()[index].host] = config.server[i].get_listen()[index].port;
		}

		data.bodySize = config.server[i].getclientbodyBuffersize();
		data.autoIndex = config.server[i].getAutoindex();
		data.root = config.server[i].get_root();

		std::vector<std::string> indx = config.server[i].getIndex();
		for (std::vector<std::string>::iterator it = indx.begin(); it != indx.end(); it++)
			data.index.push_back(*it); //

		std::set<std::string> metho= config.server[i].getAllowedmethod();
		for (std::set<std::string>::iterator it = metho.begin(); it != metho.end(); it++)
			data.methods.push_back(*it);

		data.cgi.push_back(config.server[i].getCgipass()); // todo get the whole pass + param 
		data.errorPages = config.server[i].get_errorPage();

		std::map<std::string, ConfigServer> loc = config.server[i].getLocation();
		for (std::map<std::string, ConfigServer>::iterator it = loc.begin(); it != loc.end(); it++) {
			Data locData;

			locData.bodySize = it->second.getclientbodyBuffersize();
			locData.autoIndex = it->second.getAutoindex();
			locData.root = it->second.get_root();

			std::vector<std::string> ind = it->second.getIndex();
			for (std::vector<std::string>::iterator itindex = ind.begin(); itindex != ind.end(); itindex++)
				locData.index.push_back(*itindex);
			
			locData.cgi.push_back(it->second.getCgipass()); // todo get the whole pass + param 

			std::pair<std::string, Data> pair(it->first, locData);
			data.locations.insert(pair);
		}

		serverData.push_back(data);
	}
	return serverData;
}

int main(int ac, char **av) {

    if (ac == 2) {
        Config conf;
        Server server;
        confData   confdata;

        try {
            if (!conf.parse(av[1], confdata)) {
                std::vector<Data> servers = setupData(confdata);
                // for (size_t i = 0; i < servers.size(); i++) {
                //     for (std::map<std::string, std::string>::iterator it = servers[i].listen.begin(); it != servers[i].listen.end(); it++)
                //         std::cout << "ip " << it->first << " port " << it->second << '\n';
                // }
                server.startServers(servers);
            }
            else 
                std::cerr << "Error: Invalid input. Enter a valid config file." << std::endl;
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
    
    return 0;
}

// TODO FOR PARSING CONFIG
// check IP and port (khlihom strings)
// implement ; in config file
// change names in config;
    // allow_methods -> methods
    // client_body_buffer_size -> body_size
// to check in conf: root, bodysize, 


// TODO FOR SERVER
// retest for errors in headers and requested data
// implement timeout 408 504