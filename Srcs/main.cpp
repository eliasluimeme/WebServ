#include "../Includes/Includes.hpp"
#include "../conf/webserv.hpp"

int main(int ac, char **av) {

    if (ac == 2) {
        Config conf;
        Server server;
        confData   confdata;

        if (!conf.parse(av[1], confdata)) {
            setupDataa(confdata);
            for (size_t i = 0; i < servers.size(); i++) {
                for (std::map<unsigned int, int>::iterator it = servers[i].listen.begin(); it != servers[i].listen.end(); it++)
                    std::cout << "ip " << it->first << " port " << it->second << '\n';
            }
            // for (size_t i = 0; i < confdata.server.size(); i++)
            // {
            //         std::cout << confdata.server[i] << '\n';
            // }
            // server.startServer(confdata);
        }
    }
    else 
        std::cerr << "Error: Invalid input. Enter a valid config file." << std::endl;
        
    return 0;
}

// TODO FOR PARSING CONFIG
// check IP and port (khlihom strings)
// implement ; in config file
// caught exceptions
// change names in config;
    // allow_methods -> methods
    // client_body_buffer_size -> body_size
