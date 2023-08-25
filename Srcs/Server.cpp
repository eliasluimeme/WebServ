#include "../Includes/Includes.hpp"

Server::Server(): header(false), received(false), endHeader(false) {}

Server::~Server() {
    closeServer();
}


void Server::log(std::string message) {
    std::cout << message << std::endl;
}

void Server::exitWithError(const std::string error) {
    std::cerr << "[-] Error: " << error << std::endl;
    cleanup();
    closeServer();
    exit(EXIT_FAILURE);
}

void Server::closeServer() {
    for (int i = 0; i < servers.size(); i++) {
        for (std::vector<Client>::iterator it = servers[i].clientSockets.begin(); it != servers[i].clientSockets.end(); it++)
            close(it->getFd());
        close(servers[i].serverSocket);
    }
    // close(serverSocket);
    // exit(EXIT_FAILURE);
}

void Server::cleanup() { // reset
    // if (file.is_open())
    //     file.close();
    endHeader = false;
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
    std::string html = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size() << "\n\n" << html;
    responseMsg = ss.str();
}

void Server::sendResponse(Servers &server, int &clientFd) {
    Response response;
    std::stringstream fileName;
    int index = findClientIndex(server, clientFd);
 
    buildResponse();
    std::stringstream ss;
    ss << server.clientSockets[index].getFd();
    fileName << "request-" << ss.str();
    std::string name(fileName.str());
    response.buildResponse(server.clientSockets[index], server.serverData, name); // TODO send from response

    size_t bytesSent;
    bytesSent = send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);
    if (bytesSent >= responseMsg.size())
        log("------ Response sent to client ------\n\n");
    else
        exitWithError("Error sending response to client");
    
    cleanup(); // TODO: clean resources
    server.clientSockets[index].cleanup();
    server.clientSockets.erase(server.clientSockets.begin() + index);
    FD_CLR(clientFd, &writeSetTmp);
    close(clientFd);

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
            exitWithError("Request timeout...");
        
        requestMsg = std::string(buff, bytesRead);
        server.clientSockets[index].setRequest(requestMsg);
        request.parseRequest(server.clientSockets[index]);

        if (server.clientSockets[index].received == true) {
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
    int clientSocket = accept(server.serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLength);
    if (clientSocket < 0)
        exitWithError("Couldn't accept connection. Accept failed");
    
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
    // if multiple servers listen on the same port check for server name
    // define sensible default value when not found
    int i = 1;
    int serverNum = serversData.size();
    Servers serv;

    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    for (int i = 0; i < serverNum; i++) {
        std::map<std::string, std::string>::iterator listenTo = serversData[i].listen.begin();
        serv.ipAdress = listenTo->first;
        serv.port = atoi(listenTo->second.c_str());

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
        if (setsockopt(serv.serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) < 0)
            exitWithError("Couldn't set SO_REUSEADDR");
        if (bind(serv.serverSocket, (struct sockaddr *)&serv.serverAddr, sizeof(serv.serverAddr)) < 0)
            exitWithError("Couldn't bind socket");

        setNonBlocking(serv.serverSocket);

        if (listen(serv.serverSocket, MAX_CLIENTS) < 0)
            exitWithError("Couldn't listen to socket");

        serv.serverData = serversData[i];
        servers.push_back(serv);

        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(serv.serverAddr.sin_addr) << " PORT: " << ntohs(serv.serverAddr.sin_port) << " ***\n\n";
        log(ss.str());

        FD_SET(serv.serverSocket, &readSet);
        maxFd = std::max(maxFd, serv.serverSocket);
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
        int sel = 0;
        int maxFdTmp = maxFd;
        readSetTmp = readSet;
        writeSetTmp = writeSet;

        log("====== Waiting for a new connection ======\n");
        if ((sel = select(maxFdTmp + 1, &readSet, &writeSet, NULL, NULL)) < 0)
            exitWithError("Select failed!");
        // else if (sel == 0)
        //     exitWithError("Timeout accured.."); // error 504 timeout

        for (int servIndex = 0; servIndex < servers.size(); servIndex++) {
            if (FD_ISSET(servers[servIndex].serverSocket, &readSet)) 
                acceptConnection(servers[servIndex]);

            for (int clIndex = 0; clIndex < servers[servIndex].clientSockets.size(); clIndex++) {
                int clientFd = servers[servIndex].clientSockets[clIndex].getFd();
                if (FD_ISSET(clientFd, &readSet))
                    handleRequest(servers[servIndex], clientFd);
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
    for (int i = 0; i < servers.size(); i++)
        close(servers[i].serverSocket);
}