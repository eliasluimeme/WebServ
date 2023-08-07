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
        void setAddr(struct sockaddr_in &);
        sockaddr_in &getAddr();
        void setAddrLen(int);
        size_t getAddrLen();
        void setRequestMsg(std::string);
        std::string getRequestMsg();
        void setHeaders(std::map<std::string, std::string> &);
        std::map<std::string, std::string> &getHeaders();
        void setHeaderValue(std::string &, std::string &);
        std::string getHeaderValue(std::string &);
        void setMethod(std::string &);
        std::string getMethod();
        void setURI(std::string &);
        std::string getURI();
        void setHTTP(std::string &);
        std::string getHTTP();
        void setEncoding(std::string &);
        std::string getEncoding();
        void setDelimiter(std::string &);
        std::string getDelimiter();
        void setReqFile(std::string);
        std::string &getReqFile();

    private:
        int clientFd;
        struct sockaddr_in addr;
        size_t addrLen;
        std::string request;
        std::string method, uri, http;
        std::map<std::string, std::string> headers;
        std::string encoding, delimiter;
        std::string reqFile;

        ConfData* confData;
};
