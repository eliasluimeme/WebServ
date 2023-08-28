#pragma once

#include "../Includes/Includes.hpp"

class Client;

class Response {
    public:
        Response();
        ~Response();
    
        bool buildResponse(Client &, Data &, std::string &);
        bool sendResponse(int , std::string &);
        bool    GetMethod(Client &);
    
    private:
        // Client client;
};