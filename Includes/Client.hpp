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
        Client& operator=(const Client&);
        ~Client();

        void setFd(int);
        int  getFd();
        void setAddr(struct sockaddr_in &);
        void cleanup();
        sockaddr_in &getAddr();
        void setAddrLen(int);
        size_t getAddrLen();
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
        void setQuery(std::string &);
        std::string getQuery();
        void setRequest(std::string &);
        std::string &getRequest();
        void setConfData(Data &);
        Data &getConfData();

        bool header, received;
        int toRead, readed, chunkSize, left;
        std::string encoding, delimiter;
        std::string leftInChunk;
        struct timeval timeout, startTime;

    private:
        Data data; // assign
        int clientFd;
        struct sockaddr_in addr;
        std::string method, uri, query, http;
        std::string request;
        std::map<std::string, std::string> headers;

};
