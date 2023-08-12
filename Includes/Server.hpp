#pragma once

#include "../Includes/Includes.hpp"

#define BUFFER_SIZE 1000000
#define MAX_CLIENTS 10
#define TIMEOUT 5.0

class ConfData;
class Client;

class Server {
    public:
        Server();
        ~Server();
        void startServer();
        void initServer();
        void closeServer();
        void startListning();
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

    private:
        fd_set readSet, writeSet, readSetTmp, writeSetTmp;
        struct timeval timeout;
        std::string ipAdress;
        int port, maxFd;
        int serverSocket, clientSocket;
        struct sockaddr_in serverAddr, clientAddr;
        std::vector<Client> clientSockets;
        std::string method, uri, http, query, delimiter, responseMsg;
        std::fstream file;
        std::stringstream fileName;
        int toRead, readed;
        bool header, received;
};

// TO DO
// Parse request
// Manage responce
// Multi servers