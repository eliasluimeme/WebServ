#pragma once

#include "../Includes/Includes.hpp"

#define BUFFER_SIZE 100000
#define REQUEST_TIMEOUT 30
#define CGI_TIMEOUT 20
#define RESPONSE_TIMEOUT 30
#define MAX_CLIENTS 20
#define CHUNK_SIZE 1024

class Response;
class Client;
class Request;

enum servState {
    DEFAULT,
    VIRTUAL,
};

struct Servers {
    std::string ipAdress;
    int port;
    int serverSocket;
    struct sockaddr_in serverAddr;
    enum servState state;
    Data serverData;
    std::vector<Client> clientSockets;
};

class Server {
    public:
        Server();
        ~Server();
        void startServers(std::vector<Data> &);
        bool initServers(std::vector<Data> &);
        void closeServers();
        void acceptConnection(Servers &);
        void handleRequest(Servers &,int &);
        void buildResponse(std::string &);
        void sendResponse(Servers &, int &);
        void setNonBlocking(int &);
        void exitWithError(std::string);
        void log(std::string);
        void reset();
        int  findClientIndex(Servers &, int &);
        Data &getConfData();
        void reforwarding(std::vector<Servers> &servers, int servIndex, int clIndex);

    private:
        Data data;
        int maxFd, maxFdTmp;
        // int servIndex;
        std::vector<Servers> servers;
        std::map<int, std::map<std::string, int> > ports;
        fd_set readSet, writeSet, readSetTmp, writeSetTmp;
};
