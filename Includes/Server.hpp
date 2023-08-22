#pragma once

#include "../Includes/Includes.hpp"

#define BUFFER_SIZE 10000
#define MAX_CLIENTS 10
#define TIMEOUT 5.0

class ConfData;
class Response;
class Client;
class Request;

struct Servers {
    std::string ipAdress;
    int port;
    int serverSocket;
    struct sockaddr_in serverAddr;
    std::vector<Client> clientSockets;
};

class Server {
    public:
        Server();
        ~Server();
        void startServer(Data &);
        bool initServer();
        void setConfData(Data &);
        void closeServer();
        void acceptConnection(Servers &);
        void handleRequest(Servers &,int &);
        void buildResponse();
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
        // int serverSocket, clientSocket;
        struct sockaddr_in clientAddr;
        std::vector<Servers> servers;
        // std::vector<Client> clientSockets;
        std::string method, uri, http, query, delimiter, responseMsg;
        int toRead, readed;
        bool header, received, endHeader;
};

// TO DO
// Multi servers