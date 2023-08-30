#pragma once

#include "../Includes/Includes.hpp"

class Client;

class Request {
    public:
        Request();
        ~Request();
    
        bool parseRequest(Client &, std::vector<Client> &, std::string &);
        void log(std::string);
        void exitWithError(const std::string);
        bool setEncoding(Client &, std::string &, std::vector<Client> &);
        void processChunked(Client &, std::string &, std::string &);
        bool isHexa(std::string &);
        void reset(Client &);
        int  findClientIndex(std::vector<Client> &, int &);

        bool ft_error(int, Client &);

    private:
        std::string method, uri, http, query, delimiter, tmp;
        int re;
};