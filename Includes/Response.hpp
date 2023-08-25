#pragma once

#include "../Includes/Includes.hpp"

class Client;

class Response {
    public:
        Response();
        ~Response();
    
        void buildResponse(Client &, Data &, std::string &);
        bool sendResponse(int , std::string &);
    
    private:
        Client client;
};