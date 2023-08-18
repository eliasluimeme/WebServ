#pragma once

#include "../Includes/Includes.hpp"

class Client;
class Request {
    public:
        Request();
        ~Request();
    
        void parseRequest(Client &);
        void log(std::string);
        void exitWithError(const std::string);
        void setEncoding(Client &, std::string &);
        void processChunked(Client &, std::string &, std::string &);
        bool isHexa(std::string &);

    private:
        std::string method, uri, http, query, delimiter;
        int re;
};