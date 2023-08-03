#include "../Includes/Includes.hpp"

Server::Server() {}

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
    for (std::vector<Client>::iterator it = clientSockets.begin(); it != clientSockets.end(); it++)
        close(it->getFd());
    close(serverSocket);
    // exit(EXIT_FAILURE);
}

void Server::cleanup() {

}

void Server::setNonBlocking(int &socket) {
    if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1)
        exitWithError("Setting nonblocking on server socket failed");
}

void Server::parseRequest(int fd, char *buff) {
    std::map<std::string, std::string> headers;
    std::istringstream iss(buff);
    std::string line;

    if (std::getline(iss, line)) {
        if(line.find("GET") != std::string::npos)
            headers["Method"] = "GET";
        else if (line.find("POST") != std::string::npos)
            headers["Method"] = "POST";
        else if (line.find("DELETE") != std::string::npos)
            headers["Method"] = "DELETE";
        else exitWithError("Method not supported"); // TODO: manage error message. 400 Bad Request

        size_t pos = line.find_first_of(" ");
        size_t pos2 = line.find_last_of(" ");
        if (pos && pos2) {
            headers["URI"] = line.substr(pos + 1, pos2 - pos - 1);
            headers["HTTP"] = line.substr(pos2 + 1, line.size() - pos2 - 2);
        }
        else exitWithError("Bad Request"); // TODO: manage error message. 400 Bad Request
    }
    else exitWithError("Bad Request"); // TODO: manage error message. 400 Bad Request

    while (std::getline(iss, line)) {
        if (line.compare("\r") == 0)
            break;
        if (line.find(":") != std::string::npos)
            headers[line.substr(0, line.find(":"))] = line.substr(line.find(":") + 2, line.size() - line.find(":") - 3);
    }

    // TODO: check body for chunked and multipart content type
    while (std::getline(iss, line))
        std::cout << line << std::endl;

    // for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
    //     std::cout << it->first << ": " << it->second << "." << std::endl;log("\n");

    clientSockets[findClientIndex(fd)].setHeaders(headers);
}

void Server::buildResponse() {
    std::string html = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size() << "\n\n" << html;
    responseMsg = ss.str();
}

int Server::findClientIndex(int &clientSock) {
    int index = 0;
    for (std::vector<Client>::iterator it = clientSockets.begin(); it != clientSockets.end(); it++) {
        if (it->getFd() == clientSock)
            return index;
        index++;
    }
    return -1;
}

void Server::sendResponse(int &clientFd) {
    buildResponse();

    size_t bytesSent;
    bytesSent = send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);
    if (bytesSent >= responseMsg.size())
        log("------ Response sent to client ------\n\n");
    else
        exitWithError("Error sending response to client");
    
    int socketIndex = findClientIndex(clientFd);
    if (socketIndex == -1)
        exitWithError("Couldn't find client socket index");
    clientSockets.erase(clientSockets.begin() + socketIndex);
    FD_CLR(clientFd, &writeSetTmp);
    close(clientFd);
}

void Server::handleRequest(int &clientFd) {
    // Read or save request to a file
    log("------ Handling existing clients ------\n");
    std::cout << "Client " << clientFd << std::endl << std::endl;
    char buff[BUFFER_SIZE] = {0};

    size_t bytesRead = recv(clientFd, buff, BUFFER_SIZE, 0);
    if (bytesRead <= 0) {
        if (bytesRead == 0)
            log("------ Connection closed -------\n\n");
        else exitWithError("Couldn't read from socket");

        int clientIndex = findClientIndex(clientFd);
        if (clientIndex == -1)
            exitWithError("Couldn't find client socket index");

        clientSockets.erase(clientSockets.begin() + clientIndex);
        FD_CLR(clientFd, &readSetTmp);
        close(clientFd);
    } else {
        buff[bytesRead] = '\0';
        FD_SET(clientFd, &writeSetTmp);
        FD_CLR(clientFd, &readSetTmp);

        log("------ Received Request from client ------\n\n");

        parseRequest(clientFd, buff);
        clientSockets[findClientIndex(clientFd)].setRequestMsg(buff);
        // std::cout << clientSockets[findClientIndex(clientFd)].getRequestMsg() << std::endl;
    }
}

void Server::acceptConnection() {
    unsigned int clientAddrLength = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLength);
    if (clientSocket < 0)
        exitWithError("Couldn't accept connection. Accept failed");
    
    log("------ New Connection accepted ------\n");
    std::cout << "connection from client socket: " << clientSocket << std::endl << std::endl;

    setNonBlocking(clientSocket);

    Client client;
    client.setFd(clientSocket);
    client.setIndex(findClientIndex(clientSocket));
    client.setAddr(clientAddr);
    client.setAddrLen(clientAddrLength);
    clientSockets.push_back(client);

    FD_SET(clientSocket, &readSetTmp);
    maxFd = std::max(maxFd, clientSocket);
}

void Server::startListning() {
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    if (listen(serverSocket, MAX_CLIENTS) < 0)
        exitWithError("Couldn't listen to socket");

    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " << inet_ntoa(serverAddr.sin_addr) << " PORT: " << ntohs(serverAddr.sin_port) << " ***\n\n";
    log(ss.str());

    FD_SET(serverSocket, &readSet);
    maxFd = serverSocket;
}

void Server::initServer() {
    int i = 1;
    ipAdress = "127.0.0.1";
    port = 8080;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ipAdress.c_str());

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        exitWithError("Couldn't create socket");
    if (setsockopt(serverSocket, SOL_SOCKET, SO_NOSIGPIPE, (char *)&i, sizeof(i)) < 0)
        exitWithError("Couldn't set SO_NOSIGPIPE");
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) < 0)
        exitWithError("Couldn't set SO_REUSEADDR");
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        exitWithError("Couldn't bind socket");

    setNonBlocking(serverSocket);startListning();
}

void Server::startServer() {

    initServer();

    FD_ZERO(&readSetTmp);
    FD_ZERO(&writeSetTmp);
    while (true) {
        log("====== Waiting for a new connection ======\n\n");

        int maxFdTmp = maxFd;
        readSetTmp = readSet;
        writeSetTmp = writeSet;

        if (select(maxFdTmp + 1, &readSet, &writeSet, NULL, NULL) < 0)
            exitWithError("Select failed!");

        // Check the server socket for new connection
        if (FD_ISSET(serverSocket, &readSet)) 
            acceptConnection();

        // Check each client socket for activity
        for (index = 0; index < clientSockets.size(); index++) {
            int clientFd = clientSockets[index].getFd();
            if (FD_ISSET(clientFd, &readSet))
                handleRequest(clientFd);
            else if (FD_ISSET(clientFd, &writeSet))
                sendResponse(clientFd);
        }

        readSet = readSetTmp;
        writeSet = writeSetTmp;
    }
    close(serverSocket);
}