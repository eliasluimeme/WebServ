#include "../Includes/Includes.hpp"
#include "../conf/webserv.hpp"

std::vector<Data> setupData(confData &config) { // TODO check errors
    std::vector<Data> serverData;

	for (size_t i = 0; i < config.server.size(); i++) {
		Data data;

		std::vector<std::string> name = config.server[i].get_server_name();
		for (std::vector<std::string>::iterator it = name.begin(); it != name.end(); it++)
			data.serverName.push_back(*it);
		for (size_t index = 0; index < config.server[i].get_listen().size(); index++) {
			Listen list;
			list.ip = config.server[i].get_listen()[index].host;
			list.port = config.server[i].get_listen()[index].port;
			data.listen.push_back(list);
			for (size_t check = 0; check < index; check++)
				if (data.listen[check].port == list.port) {
					std::cout << "[-] Error: Port duplicated" << std::endl;
					exit(EXIT_FAILURE);
				}
		}

		data.bodySize = config.server[i].getclientbodyBuffersize();
		data.autoIndex = config.server[i].getAutoindex();
		data.root = config.server[i].get_root();

		std::vector<std::string> indx = config.server[i].getIndex();
		for (std::vector<std::string>::iterator it = indx.begin(); it != indx.end(); it++)
			data.index.push_back(*it); //

		std::set<std::string> metho= config.server[i].getAllowedmethod();
		for (std::set<std::string>::iterator it = metho.begin(); it != metho.end(); it++)
			data.methods.insert(*it);

		// data.cgi.push_back(config.server[i].getCgipass());
		data.errorPages = config.server[i].get_errorPage();
		data.uploadPass = config.server[i].get_Upload_pass();
		std::set<std::string> cgii = config.server[i].getCgi();
		for (std::set<std::string>::iterator it = cgii.begin(); it != cgii.end(); it++)
			data.cgi.push_back(*it);

		std::map<std::string, ConfigServer> loc = config.server[i].getLocation();
		for (std::map<std::string, ConfigServer>::iterator it = loc.begin(); it != loc.end(); it++) {
			Data locData;

			locData.bodySize = it->second.getclientbodyBuffersize();
			locData.autoIndex = it->second.getAutoindex();
			locData.root = it->second.get_root();
			locData.uploadPass = it->second.get_Upload_pass();

			std::set<std::string> meth = it->second.getAllowedmethod();
			for (std::set<std::string>::iterator it = meth.begin(); it != meth.end(); it++)
				locData.methods.insert(*it);

			std::vector<std::string> ind = it->second.getIndex();
			for (std::vector<std::string>::iterator itindex = ind.begin(); itindex != ind.end(); itindex++)
				locData.index.push_back(*itindex);
			
			// locData.cgi.push_back(it->second.getCgipass());
			std::set<std::string> cgii = it->second.getCgi();
			for (std::set<std::string>::iterator it = cgii.begin(); it != cgii.end(); it++)
				locData.cgi.push_back(*it);

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
                server.startServers(servers);
            }
            else 
                std::cerr << "Error: Invalid input. Enter a valid config file." << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    } else 
        std::cerr << "Error: Invalid input. Enter a valid config file." << std::endl;
    
    return 0;
}
