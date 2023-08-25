#pragma once

#include "../Includes/Includes.hpp"

class Client;
class Data;

class CGI {
    public:
        CGI();
        ~CGI();

        void start(Client &client, Data &serverData, std::string &filename);

    private:

}; 