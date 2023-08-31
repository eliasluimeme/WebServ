#include "../Includes/Includes.hpp"

Server::Server() {}

Server::~Server() {
    closeServers();
}


void Server::log(std::string message) {
    std::cout << message << std::endl;
}

void Server::reset() { // reset
    // if (file.is_open())
    //     file.close();
}

void Server::closeServers() {
    for (int i = 0; i < servers.size(); i++) {
        for (std::vector<Client>::iterator it = servers[i].clientSockets.begin(); it != servers[i].clientSockets.end(); it++)
            close(it->getFd());
        close(servers[i].serverSocket);
    }
}

void Server::exitWithError(const std::string error) {
    std::cerr << "[-] Error: " << error << std::endl;
    reset();
    closeServers();
    exit(EXIT_FAILURE);
}

Data &Server::getConfData() {
    return data;
}

void Server::setNonBlocking(int &socket) {
    if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1)
        exitWithError("Setting nonblocking on server socket failed");
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

void Server::buildResponse() {
    std::string responseMsg;
    std::string html = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size() << "\n\n" << html;
    responseMsg = ss.str();
}

void Server::sendResponse(Servers &server, int &clientFd) {
    Response response;
    std::stringstream fileName;
    int index = findClientIndex(server, clientFd);
 
    // buildImage(clientFd);
    // buildVideo(clientFd);
    // buildResponse();
    std::stringstream ss;
    ss << server.clientSockets[index].getFd();
    fileName << "request-" << ss.str();
    std::string name(fileName.str());

    // size_t bytesSent;
    // bytesSent = send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);
    // if (bytesSent >= responseMsg.size())
    //     log("------ Response sent to client ------\n\n");
    // else
    //     exitWithError("Error sending response to client");
    std::cout << "here" << std::endl;
    if (response.buildResponse(server.clientSockets[index], server.serverData, name) == true) {
        std::cout << "cleanin..." << std::endl;
        reset(); // TODO: clean resources
        server.clientSockets[index].cleanup();
        server.clientSockets.erase(server.clientSockets.begin() + index);
        FD_CLR(clientFd, &writeSetTmp);
        close(clientFd);
        // exit(0);
    }
    std::cout << "nothing" << std::endl;

    // delete file after sending response
    // std::string s(fileName.str());
    // if (std::remove(s.c_str()))
    //     exitWithError("Error removing file");
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

    size_t bytesRead = read(clientFd, buff, BUFFER_SIZE); // check body limit
    std::stringstream fd;
    fd << clientFd;

    if (bytesRead <= 0) {
        if (bytesRead == 0)
            log("------ Connection closed -------\n\n");
        else exitWithError("[-] Couldn't read from socket " + fd.str());

        server.clientSockets.erase(server.clientSockets.begin() + index);
        FD_CLR(clientFd, &readSetTmp);
        close(clientFd);
    } else {
        Request request;
        struct timeval currentTime;
        currentTime.tv_sec = static_cast<time_t>(time);
        if (currentTime.tv_sec - server.clientSockets[index].startTime.tv_sec >= REQUEST_TIMEOUT)
            request.ft_error(408, server.clientSockets[index]);
        
        requestMsg = std::string(buff, bytesRead);
        request.parseRequest(server.clientSockets[index], requestMsg);
        std::cout << "tema" << std::endl;
        if (server.clientSockets[index].state == CLEAR) {
            server.clientSockets.erase(server.clientSockets.begin() + index);
            FD_CLR(clientFd, &readSetTmp);
            close(clientFd);
        }
        else if ((server.clientSockets[index].received == true) || (server.clientSockets[index].state == REQUEST_RECEAVED)) {
            std::cout << "to read: " << server.clientSockets[index].toRead << " readed: " << server.clientSockets[index].readed << std::endl;
            log("------ Received Request from client: " + ss.str() + " ------\n\n");
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
    if (server.state != VIRTUAL) {
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
    }

    server.clientSockets.push_back(client);
}

bool Server::initServers(std::vector<Data> &serversData) { // TODO check errors
    // check ports
    // if multiple servers listen on the same port check for host with server name
    // define sensible default value when not found
    int i = 1;
    int serverNum = serversData.size();

    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    for (int i = 0; i < serverNum; i++) {
        Servers serv;
        std::map<std::string, std::string>::iterator listenTo = serversData[i].listen.begin();
        serv.serverData = serversData[i];

        if (listenTo->first.empty()) 
            serv.ipAdress = "127.0.0.1";
        else serv.ipAdress = listenTo->first;
        if (listenTo->second.empty()) // TODO check the default server in config
            serv.port = 8080;
        else {
            try {
                serv.port = atoi(listenTo->second.c_str());
            } catch(const std::exception &e) {
                exitWithError(e.what());
            }
        }

        // check for same ports
        // The first server block (default server) acts as a catch-all for unrecognized domains.
        // The subsequent server blocks are configured for specific domain names.
        // When a request comes in, the web server examines the Host header and routes the request to the appropriate server block based on the domain name in the Host header.

        // check if port already in use
        // if so dont bind it 
        // handle the request 
        // then route the request to the appropriate server

        for (int index = 0; index < servers.size(); index++) {
            if (serv.port == servers[index].port) {
                if (serv.serverData.serverName[0] != servers[index].serverData.serverName[0])
                    serv.state = VIRTUAL;
                else exitWithError("Invalid server configuration");
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
        int a = 1;
        signal(SIGPIPE, SIG_IGN);
        if (setsockopt(serv.serverSocket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(i)) < 0) // redo a
            exitWithError("Couldn't set REUSEADDR opt");
        if (serv.state != VIRTUAL) {
            if (bind(serv.serverSocket, (struct sockaddr *)&serv.serverAddr, sizeof(serv.serverAddr)) < 0)
                exitWithError("Couldn't bind socket. Check your port number");

            setNonBlocking(serv.serverSocket);

            if (listen(serv.serverSocket, MAX_CLIENTS) < 0)
                exitWithError("Couldn't listen to socket");

            FD_SET(serv.serverSocket, &readSet);
            maxFd = std::max(maxFd, serv.serverSocket);
        }

        servers.push_back(serv);

        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(serv.serverAddr.sin_addr) << " PORT: " << ntohs(serv.serverAddr.sin_port) << " ***\n\n";
        log(ss.str());
    }
    return true;
}

void Server::startServers(std::vector<Data> &serversData) {
    initServers(serversData);

    // std::ofstream outputFile("output.txt");
    // // Store the current state of std::cout
    // std::streambuf* coutBuffer = std::cout.rdbuf();
    // // Redirect std::cout to the file
    // std::cout.rdbuf(outputFile.rdbuf());

    FD_ZERO(&readSetTmp);
    FD_ZERO(&writeSetTmp);
    while (true) {
        int activity = 0;
        int maxFdTmp = maxFd;
        readSetTmp = readSet;
        writeSetTmp = writeSet;

        log("====== Waiting for a new connection ======\n");
        
        if ((activity = select(maxFdTmp + 1, &readSet, &writeSet, NULL, NULL)) < 0)
            exitWithError("Select failed!");

        for (int servIndex = 0; servIndex < servers.size(); servIndex++) {
            if (FD_ISSET(servers[servIndex].serverSocket, &readSet)) 
                acceptConnection(servers[servIndex]);

            for (int clIndex = 0; clIndex < servers[servIndex].clientSockets.size(); clIndex++) {
                int clientFd = servers[servIndex].clientSockets[clIndex].getFd();
                if (FD_ISSET(clientFd, &readSet))
                    handleRequest(servers[servIndex], clientFd);
                // if (servers[servIndex].clientSockets[clIndex].state == REINDEX)
                //     routeRequest(servers);
                if (FD_ISSET(clientFd, &writeSet))
                    sendResponse(servers[servIndex], clientFd);
            }
        }

        readSet = readSetTmp;
        writeSet = writeSetTmp;
    }
    // Restore the original state of std::cout
    // std::cout.rdbuf(coutBuffer);
    // outputFile.close();
    closeServers();
}