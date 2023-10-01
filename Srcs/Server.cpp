#include "../Includes/Includes.hpp"

Server::Server():reset(false), maxFd(0) {}

Server::~Server() {
    closeServers();
}


void Server::log(std::string message) {
    std::cout << message << std::endl;
}


void Server::closeServers() {
    for (size_t i = 0; i < servers.size(); i++) {
        for (std::vector<Client>::iterator it = servers[i].clientSockets.begin(); it != servers[i].clientSockets.end(); it++)
            close(it->getFd());
        close(servers[i].serverSocket);
    }
}

void Server::exitWithError(const std::string error) {
    std::cerr << "[-] Error: " << error << std::endl;
    closeServers();
    exit(EXIT_FAILURE);
}

Data &Server::getConfData() {
    return data;
}

void Server::setNonBlocking(int &socket) {
    if (fcntl(socket, F_SETFL, O_NONBLOCK) < 0)
        exitWithError("Setting nonblocking on socket failed");
}

int Server::findClientIndex(Servers &server, int &clientSock) {
    int index = 0;
    for (std::vector<Client>::iterator it = server.clientSockets.begin(); it != server.clientSockets.end(); it++) {
        if (it->getFd() == clientSock)
            return index;
        index++;
    }
    exitWithError("Client not found");
    return -1;
}

void Server::buildResponse(std::string &responseMsg) {
    std::string html = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size() << "\n\n" << html;
    responseMsg = ss.str();
}

void Server::sendResponse(Servers &server, int &clientFd) {
    Response response;
    std::stringstream fileName;
    int index = findClientIndex(server, clientFd);

    std::string responseMsg;
    std::stringstream ss;
    ss << server.clientSockets[index].getFd();
    fileName << "request-" << ss.str();
    std::string name(fileName.str());

    response.buildResponse(server.clientSockets[index], server.clientSockets[index].getConfData(), name);

    if (response.state == SENT || response.state == DROP_CLIENT) {
        std::cout << "---- Response sent. Droping client " << server.clientSockets[index].getFd() << " ----\n" << std::endl;
        server.clientSockets[index].cleanup();
        server.clientSockets.erase(server.clientSockets.begin() + index);
        FD_CLR(clientFd, &writeSetTmp);
        close(clientFd);
        reset = true;

        // delete file after sending response
        if (server.clientSockets[index].rename == false) {
            std::string s(fileName.str());
            if (std::remove(s.c_str()))
                std::cerr << "Error removing request file" << std::endl;
        }
    }

}

void Server::handleRequest(Servers &server, int &clientFd) {
    std::stringstream ss;
    ss << clientFd;
    log("------ Handling existing clients: " + ss.str() + " ------\n");

    std::string requestMsg;
    char buff[BUFFER_SIZE] = {0};
    int index = findClientIndex(server, clientFd);
    std::time_t time = std::time(NULL);
    if (server.clientSockets[index].startTime.tv_sec == 0)
        server.clientSockets[index].startTime.tv_sec = static_cast<time_t>(time);

    size_t bytesRead = 0;
    bytesRead = read(clientFd, buff, BUFFER_SIZE);

    if (bytesRead <= 0) {
        std::stringstream fd;
        fd << clientFd;
        if (bytesRead == 0)
            log("------ Connection closed -------\n\n");
        else exitWithError("[-] Couldn't read from socket " + fd.str());

        server.clientSockets.erase(server.clientSockets.begin() + index);
        FD_CLR(clientFd, &readSetTmp);
        close(clientFd);
        reset = true;
    } else {
        Request request;
        struct timeval currentTime;
        currentTime.tv_sec = static_cast<time_t>(time);
        if (currentTime.tv_sec - server.clientSockets[index].startTime.tv_sec >= REQUEST_TIMEOUT)
            request.ft_error(408, server.clientSockets[index], " Request Timeout");

        requestMsg = std::string(buff, bytesRead);
        request.parseRequest(server.clientSockets[index], requestMsg);

        // std::cout << "Receiving request body: " << server.clientSockets[index].readed << "  bytes received" << std::endl;

        if (server.clientSockets[index].state == CLEAR) {
            std::cout << "-------   Droping client " << server.clientSockets[index].getFd() << "   -------\n" << std::endl;
            server.clientSockets.erase(server.clientSockets.begin() + index);
            FD_CLR(clientFd, &readSetTmp);
            close(clientFd);
            reset = true;
        }
        else if ((server.clientSockets[index].received == true) || (server.clientSockets[index].state == REQUEST_RECEAVED)) {
            // std::cout << "to read: " << server.clientSockets[index].toRead << " readed: " << server.clientSockets[index].readed << std::endl;
            log("------ Received Request from client: " + ss.str() + " ------\n");
            FD_CLR(clientFd, &readSetTmp);
            FD_SET(clientFd, &writeSetTmp);
        }
    }
}

void Server::acceptConnection(Servers &server) {
    Client client;
    struct sockaddr_in clientAddr;

    memset(&clientAddr, 0, sizeof(clientAddr));
    unsigned int clientAddrLength = sizeof(clientAddr);

    int clientSocket = accept(server.serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLength);
    if (clientSocket < 0)
        perror("Couldn't accept connection. Accept failed");
    
    log("------   New Connection accepted    ------\n");

    setNonBlocking(clientSocket);
    FD_SET(clientSocket, &readSetTmp);
    maxFd = std::max(maxFd, clientSocket);

    client.setFd(clientSocket);
    client.setAddr(clientAddr);
    client.setConfData(server.serverData);
    server.clientSockets.push_back(client);
}

bool Server::initServers(std::vector<Data> &serversData) { // TODO check errors
    // check ports
    // if multiple servers listen on the same port check for host with server name
    // define sensible default value when not found
    int serverNum = serversData.size();

    for (int i = 0; i < serverNum; i++) {
        for (std::vector<Listen>::iterator listenTo = serversData[i].listen.begin(); listenTo != serversData[i].listen.end(); listenTo++) {
            Servers serv;
            serv.serverData = serversData[i];



            if (listenTo->ip.empty()) 
                serv.ipAdress = "127.0.0.1";
            else serv.ipAdress = listenTo->ip;
            if (listenTo->port.empty())
                serv.port = 8080;
            else {
                try {
                    serv.port = atoi(listenTo->port.c_str());
                } catch(const std::exception &e) {
                    exitWithError(e.what());
                }
            }

            serv.state = DEFAULT;
            for (size_t index = 0; index < servers.size(); index++) {
                if (serv.ipAdress == servers[index].ipAdress) {
                    if (serv.port == servers[index].port) {
                        if (serv.serverData.serverName[0] != servers[index].serverData.serverName[0]) {
                            serv.state = VIRTUAL;
                            break;
                        }
                        else exitWithError("Invalid server configuration. Check port and server name");
                    } else serv.state = DEFAULT;
                } else serv.state = DEFAULT;
            }

            memset(&serv.serverAddr, 0, sizeof(serv.serverAddr));
            serv.serverAddr.sin_family = AF_INET;
            serv.serverAddr.sin_port = htons(serv.port);
            serv.serverAddr.sin_addr.s_addr = inet_addr(serv.ipAdress.c_str());

            serv.serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (serv.serverSocket < 0)
                exitWithError("Couldn't create socket");

            // if (setsockopt(serverSocket, SOL_SOCKET, SO_NOSIGPIPE, (char *)&i, sizeof(i)) < 0)
                // exitWithError("Couldn't set SO_NOSIGPIPE");
            signal(SIGPIPE, SIG_IGN);
            if (serv.state == DEFAULT) {
                int a = 1;
                if (setsockopt(serv.serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&a, sizeof(i)) < 0) // redo a
                    exitWithError("Couldn't set REUSEADDR opt");
                if (bind(serv.serverSocket, (struct sockaddr *)&serv.serverAddr, sizeof(serv.serverAddr)) < 0)
                    exitWithError("Couldn't bind socket. Check your port number");

                setNonBlocking(serv.serverSocket);

                if (listen(serv.serverSocket, MAX_CLIENTS) < 0)
                    exitWithError("Couldn't listen to socket");

                FD_SET(serv.serverSocket, &readSetTmp);
                maxFd = std::max(maxFd, serv.serverSocket);
            }

            servers.push_back(serv);

            std::ostringstream ss;
            ss << "\n*** Listening on ADDRESS: " << inet_ntoa(serv.serverAddr.sin_addr) << " PORT: " << ntohs(serv.serverAddr.sin_port) << " ***\n\n";
            log(ss.str());
        }
    }
    return true;
}

void Server::reforwarding(std::vector<Servers> &servers, int servIndex, int clIndex) {
    // if VIRTUAL
    // if same ip and same port
    // if host != servname -> reforward
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i].state == VIRTUAL) {
            if ((servers[i].ipAdress == servers[servIndex].ipAdress) && (servers[i].port == servers[servIndex].port)) {
                if (servers[i].serverData.serverName[0] == servers[servIndex].clientSockets[clIndex].getHeaders()["Host"]) {
                    servers[servIndex].clientSockets[clIndex].reIndex = false;
                    servers[servIndex].clientSockets[clIndex].setConfData(servers[i].serverData);
                    std::cout << "forwarded from server " << servIndex << " to " << i << std::endl;
                    break;
                }
            }
        }
    }
}

void Server::startServers(std::vector<Data> &serversData) {
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    FD_ZERO(&readSetTmp);
    FD_ZERO(&writeSetTmp);

    initServers(serversData);

    while (true) {
        int activity = 0;
        maxFdTmp = maxFd;

        readSet = readSetTmp;
        writeSet = writeSetTmp;
        log("====== Waiting for a new connection ======\n");

        if ((activity = select(maxFdTmp + 1, &readSet, &writeSet, NULL, NULL)) < 0)
            exitWithError("Select failed!");

        for (int servIndex = 0; servIndex < static_cast<int>(servers.size()); servIndex++) {
            if (FD_ISSET(servers[servIndex].serverSocket, &readSet)) 
                acceptConnection(servers[servIndex]);

            // std::cout << "server " << servIndex << std::endl;
            // std::cout << "server state " << servers[servIndex].state << std::endl;

            for (int clIndexx = 0; clIndexx < static_cast<int>(servers[servIndex].clientSockets.size()); clIndexx++) {
                int clientFd = servers[servIndex].clientSockets[clIndexx].getFd();

                if (FD_ISSET(clientFd, &readSet))
                    handleRequest(servers[servIndex], clientFd);
                if (reset) {
                    reset = false;
                    break;
                }
                if (servers[servIndex].clientSockets[clIndexx].reIndex == true)
                    reforwarding(servers, servIndex, clIndexx);
                if (FD_ISSET(clientFd, &writeSet))
                    sendResponse(servers[servIndex], clientFd);
            }
        }
    }
    closeServers();
}
