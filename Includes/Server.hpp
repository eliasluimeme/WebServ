#pragma once

#include "../Includes/Includes.hpp"

#define BUFFER_SIZE 10000
#define MAX_CLIENTS 10

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
        void parseRequest(int fd, char *buff);
        void setNonBlocking(int &);
        void exitWithError(std::string);
        void log(std::string);
        void cleanup();
        int  findClientIndex(int &);
        // void  listenTo();
        // void createSocket();
        // void waitForConnection();
        // void receiveLoop();
        // void sendBack();

    private:
    fd_set readSet, writeSet, readSetTmp, writeSetTmp;

    int index;

    std::string ipAdress;
    int port;
    int maxFd;
    int serverSocket;
    int clientSocket;
    long incommingMessage;
    std::vector<Client> clientSockets;
    struct sockaddr_in serverAddr, clientAddr;
    std::string serverMsg;
    std::string responseMsg;
};

// TO DO
// Parse request
// Manage responce
// Multi servers