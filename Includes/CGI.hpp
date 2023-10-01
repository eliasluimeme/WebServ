#pragma once

#include "../Includes/Includes.hpp"

class Client;
struct Data;

class CGI {
    public:
        CGI();
        ~CGI();

        // std::string start(Client &, std::string &);
        std::string start(Client &, std::string &);
        void errors(int status, Client &client);

        void getEnv(Client &);
        std::string toUpper(std::string);

    private:
        char **env;
}; 