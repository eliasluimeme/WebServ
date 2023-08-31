#pragma once

#include "../Includes/Includes.hpp"

class Client;

class Response {
    public:
        Response();
        ~Response();
    
        bool buildResponse(Client &, Data &, std::string &);
        bool sendResponse(int , std::string &);
        bool    GetMethod(Client &, Data &, std::string );
        std::map<std::string, std::string> c_Type;
        void getLocationMatch(std::string , std::map<std::string ,Data> , std::string &);
        bool get_file(std::ifstream &a, Client &client, std::map<std::string, std::string> c_Type, std::string p);
        bool get_directory(Client &c, bool &autoin, std::vector<std::string>& index, std::string& root);
        void sendchunked(int clientSocket, int offset, int portionSize);
        void auto_index(Client &c, std::string &root);
        std::string loc;
        std::vector<std::string> auto_index_file;
        std::string res_auto_index;
        Request rq;

    private:
        // Client client;
};