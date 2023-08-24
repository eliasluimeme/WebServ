#include "../Includes/Includes.hpp"

int main(int ac, char **av) {
    if (ac == 2) {
        Parser parser(av[1]);
        Server server;
        Data   data;

        if (parser.checkConfigFile(data))
            server.startServer(data);
    }
    else std::cerr << "Error: Invalid input. Enter a valid config file." << std::endl;
    return 0;
}
// TODO FOR PARSING CONFIG
// check IP and port (khlihom strings)
// implement ; in config file
// change names in config;
    // allow_methods -> methods
    // client_body_buffer_size -> body_size