#pragma once

#include "../Includes/Includes.hpp"

class Client;

class Request {
    public:
        Request();
        ~Request();
    
        bool parseRequest(Client &, std::string &);
        void log(std::string);
        void exitWithError(const std::string);
        bool setEncoding(Client &, std::string &);
        void processChunked(Client &, std::string &, std::string &);
        bool isHexa(std::string &);
        void reset(Client &);
        int  findClientIndex(std::vector<Client> &, int &);
        void checkURI();
        void repl(std::string&, std::string, std::string);

        bool ft_error(int, Client &, std::string);
        std::map<int, std::string>error_p;
        std::map<int, std::string>deferror;

    private:
        std::string method, uri, http, query, delimiter, tmp;
        int re;
};