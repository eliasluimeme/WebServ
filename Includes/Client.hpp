#pragma once

#include "../Includes/Includes.hpp"

class ConfData;

enum clientState {
    READY,
    REQUEST_RECEAVED,
    RESPONSE_COMPLETE,
};

class Client {
    public:
        Client();
        Client(int);
        ~Client();

        void setFd(int);
        int  getFd();
        void setIndex(int);
        int  getIndex();
        void setAddr(struct sockaddr_in &);
        sockaddr_in &getAddr();
        void setAddrLen(int);
        size_t getAddrLen();
        void setRequestMsg(std::string);
        std::string getRequestMsg();
        void setHeaders(std::map<std::string, std::string>);
        std::map<std::string, std::string> &getHeaders();
        void setHeaderValue(std::string &, std::string &);
        std::string getHeaderValue(std::string &);

    private:
        int index;
        int clientFd;
        struct sockaddr_in addr;
        size_t addrLen;
        std::string request;
        std::string method;
        std::string URI;
        std::string body;
        std::map<std::string, std::string> headers;

        ConfData* confData;
};
