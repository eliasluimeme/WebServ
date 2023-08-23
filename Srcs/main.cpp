#include "../Includes/Includes.hpp"

int main(int ac, char **av) {

    if (ac == 2) {
        Config conf;
        Server server;
        Data   data;

        // if (!conf.parse(av[1]))
            server.startServer(data);
    }
    else 
        std::cerr << "Error: Invalid input. Enter a valid config file." << std::endl;
        
    return 0;
}