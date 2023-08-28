#pragma once

#include "../Includes/Includes.hpp"

class Client;
class Data;

class CGI {
    public:
        CGI();
        ~CGI();

        std::string start(Client &, Data &, std::string &);
        void getEnv(Client &);
        std::string toUpper(std::string);

    private:
        char **env;
}; 