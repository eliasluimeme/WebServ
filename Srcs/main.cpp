#include "../Includes/Includes.hpp"

int main(int ac, char **av) {

    if (ac == 2) {
        Config conf;
        Server server;
        confData   confdata;

        if (!conf.parse(av[1], confdata)) {
            for (size_t i = 0; i < confdata.server.size(); i++)
            {
                    std::cout << confdata.server[i] << '\n';
            }
            // server.startServer(confdata);
        }
    }
    else 
        std::cerr << "Error: Invalid input. Enter a valid config file." << std::endl;
        
    return 0;
}