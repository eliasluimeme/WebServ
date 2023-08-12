#include "../Includes/Includes.hpp"

int main(int ac, char **av) {
    if (ac == 2) {
        Parser parser(av[1]);
        Server server;
        Data   data;

        if (parser.checkConfigFile(data))
            server.startServer();
    }
    else std::cerr << "Error: Invalid input. Enter a valid config file." << std::endl;
    return 0;
}