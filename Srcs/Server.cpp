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
    std::string encoding, delimiter;
    std::istringstream iss(buff);
    std::string key, value;
    std::string line;

    if (std::getline(iss, line)) {
        std::istringstream iss(line);
        iss >> method >> uri >> http;
    } else exitWithError("Bad request"); // 400 Bad Request

    while (std::getline(iss, line)) {
        if (line.compare("\r") == 0)
            break;
        if (line.find(":") != std::string::npos) {
            key = line.substr(0, line.find(":") + 1);
            value = line.substr(line.find(":") + 2, line.size() - line.find(":") - 3);
            if (!key.empty() && !value.empty())
                headers[key] = value;
        }
    }

    if (headers.find("Transfer-Encoding:") != headers.end()) { // recheck for chunk and recheck for encoding
        if (headers["Transfer-Encoding:"] == "chunked")
            encoding = "chunked";
    }
    else if (headers.find("Content-Type:") != headers.end()) {
        std::string val = headers["Content-Type:"];
        if (val.find("multipart/form-data;") != std::string::npos) {
            encoding = "multipart";
            delimiter = val.substr(val.find("=") + 1, val.size() - val.find("="));
        }
    }
    else if (headers.find("Transfer-Encoding:") == headers.end() && headers.find("Content-Length:") != headers.end())
        encoding = "length";

    std::fstream reqBody;
    reqBody.open("reqBody", std::ios::in | std::ios::out | std::ios::trunc);
    if (reqBody.is_open()) {
        while (std::getline(iss, line))
            reqBody << line << std::endl;
    } else exitWithError("Can't open file");

    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        std::cout << it->first << " " << it->second << "." << std::endl;
    if (!encoding.empty())
        std::cout << "- Encoding: " << encoding << std::endl;
    if (!delimiter.empty())
        std::cout << "- Delimiter: " << delimiter << std::endl;

    reqBody.close();
    clientSockets[findClientIndex(fd)].setHeaders(headers);
    clientSockets[findClientIndex(fd)].setMethod(method);
    clientSockets[findClientIndex(fd)].setURI(uri);
    clientSockets[findClientIndex(fd)].setHTTP(http);
    clientSockets[findClientIndex(fd)].setEncoding(encoding);
    clientSockets[findClientIndex(fd)].setDelimiter(delimiter);
    clientSockets[findClientIndex(fd)].setReqFile("reqBody");
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
    
    cleanup(); // TODO: clean resources
    int socketIndex = findClientIndex(clientFd);
    if (socketIndex == -1)
        exitWithError("Couldn't find client socket index");
    clientSockets.erase(clientSockets.begin() + socketIndex);
    FD_CLR(clientFd, &writeSetTmp);
    // FD_SET(clientFd, &readSetTmp);
    close(clientFd);
}

void Server::handleRequest(int &clientFd) {
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
    } else if (bytesRead <= BUFFER_SIZE) {
        buff[bytesRead] = '\0';
        FD_SET(clientFd, &writeSetTmp);
        FD_CLR(clientFd, &readSetTmp);

        log("------ Received Request from client ------\n\n");

        parseRequest(clientFd, buff);
        clientSockets[findClientIndex(clientFd)].setRequestMsg(buff);
        std::cout << clientSockets[findClientIndex(clientFd)].getRequestMsg() << std::endl;
    } else exitWithError("Request too big");
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

    timeout.tv_sec = 0;
    timeout.tv_usec = 200000;
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
        for (int index = 0; index < clientSockets.size(); index++) {
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