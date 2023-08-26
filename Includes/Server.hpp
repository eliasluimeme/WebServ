#pragma once

#include "../Includes/Includes.hpp"

#define BUFFER_SIZE 10000
#define REQUEST_TIMEOUT 2
#define CGI_TIMEOUT 2
#define RESPONSE_TIMEOUT 30
#define MAX_CLIENTS 20
#define CHUNK_SIZE 1024

class Response;
class Client;
class Request;

struct Servers {
    std::string ipAdress;
    int port;
    int serverSocket;
    Data serverData;
    struct sockaddr_in serverAddr;
    std::vector<Client> clientSockets;
};

class Server {
    public:
        Server();
        ~Server();
        void startServers(std::vector<Data> &);
        bool initServers(std::vector<Data> &);
        void closeServer();
        void acceptConnection(Servers &);
        void handleRequest(Servers &,int &);
        void buildResponse();
        void buildImage(int );
        void buildVideo(int );
        void sendResponse(Servers &, int &);
        void setNonBlocking(int &);
        void exitWithError(std::string);
        void log(std::string);
        void cleanup();
        int  findClientIndex(Servers &, int &);
        Data &getConfData();
        // void parseRequest(int, std::string);
        // void setEncoding(std::map<std::string, std::string> &, std::string &, int);
        // void processChunked(std::string chunks, std::string filename);

    private:
        Data data;
        fd_set readSet, writeSet, readSetTmp, writeSetTmp;
        struct timeval timeout;
        std::string ipAdress;
        int port, maxFd;
        std::vector<Servers> servers;
        std::string method, uri, http, query, delimiter, responseMsg;
        int toRead, readed;
        bool header, received, endHeader;
};
