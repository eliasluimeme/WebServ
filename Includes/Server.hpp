#pragma once

#include "../Includes/Includes.hpp"

#define BUFFER_SIZE 4000
#define MAX_CLIENTS 10
#define TIMEOUT 5.0

class ConfData;
class Response;
class Client;
class Request;

class Server {
    public:
        Server();
        ~Server();
        void startServer(Data &);
        bool initServer();
        void setConfData(Data &);
        void closeServer();
        void acceptConnection();
        void handleRequest(int &);
        void buildResponse();
        void sendResponse(int &);
        void parseRequest(int, std::string);
        void setNonBlocking(int &);
        void exitWithError(std::string);
        void log(std::string);
        void cleanup();
        void setEncoding(std::map<std::string, std::string> &, std::string &, int);
        int  findClientIndex(int &);
        Data &getConfData();
        // void processChunked(std::string chunks, std::string filename);

    private:
        Data data;
        fd_set readSet, writeSet, readSetTmp, writeSetTmp;
        struct timeval timeout;
        std::string ipAdress;
        int port, maxFd;
        int serverSocket, clientSocket;
        struct sockaddr_in serverAddr, clientAddr;
        std::vector<Client> clientSockets;
        std::string method, uri, http, query, delimiter, responseMsg;
        int toRead, readed;
        bool header, received, endHeader;
};

// TO DO
// Multi servers