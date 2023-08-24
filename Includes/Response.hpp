#pragma once

#include "../Includes/Includes.hpp"

class Client;

class Response {
    public:
        Response();
        ~Response();
    
        void buildResponse(Client &, Data &, std::string &,std::string &);
    
    private:
        Client client;
};