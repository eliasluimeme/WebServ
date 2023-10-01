#pragma once

#include "../Includes/Includes.hpp"

class Client;
class Request;

enum respState {
    IN_PROGRESS,
    DROP_CLIENT,
    SENT,
};

class Response {
    public:
        Response();
        ~Response();
    
        bool buildResponse(Client &, Data &, std::string &);
        void exitWithError(std::string );
        bool sendResponse(int , std::string &);
        bool GetMethod(Client &, Data &, std::string );
        void getLocationMatch(std::string , std::map<std::string ,Data> , std::string &);
        bool get_file(std::ifstream &a, Client &client, Data &serverdata, std::map<std::string, std::string> c_Type, std::string p);
        bool get_directory(Client &c, Data &serverdata, bool &autoin, std::vector<std::string>& index, std::string& root);
        void sendchunked(Client &client, int offset, int portionSize, std::string );
        bool auto_index(Client &c, std::string &root);
        std::string loc;
        std::vector<std::string> auto_index_file;
        std::string res_auto_index;
        Request rq;

        std::map<std::string, std::string> c_Type;
        enum respState state;
        int statee;
        bool chunked;
        bool  isallowdMethod(std::string method, Data &location);

        //Post
        bool PostMethod(Client &cl, Data &serverdata, std::string &location);
        bool get_file_post(Client &);

        //Delete
        bool DeleteMethod(Client &client, Data &serverData, std::string &location);
        bool remouveDirectories(Client &, std::string);

    private:
        // Client client;
};