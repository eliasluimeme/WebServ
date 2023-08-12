#include "../Includes/Includes.hpp"

Server::Server(): header(false), received(false) {}

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
    log("cleaning...");
    if (file.is_open())
        file.close();
    
}

void Server::setNonBlocking(int &socket) {
    if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1)
        exitWithError("Setting nonblocking on server socket failed");
}

int Server::findClientIndex(int &clientSock) {
    int index = 0;
    for (std::vector<Client>::iterator it = clientSockets.begin(); it != clientSockets.end(); it++) {
        if (it->getFd() == clientSock)
            return index;
        index++;
    }
    exitWithError("Client not found");
    return -1;
}

void Server::setEncoding(std::map<std::string, std::string> &headers, std::string &encoding, int index) {
    if (headers.find("Transfer-Encoding:") != headers.end()) {
        if (headers["Transfer-Encoding:"] == "chunked")
            encoding = "chunked";
    }
    else if (headers.find("Content-Type:") != headers.end() && headers["Content-Type:"].find("multipart/form-data;") != std::string::npos) {
        std::string val = headers["Content-Type:"];
        delimiter = val.substr(val.find("=") + 1, val.size() - val.find("="));
        clientSockets[index].setDelimiter(delimiter);
        encoding = "multipart";
    }
    else if (headers.find("Content-Length:") != headers.end())
        encoding = "length";
    else if (method != "GET")
        exitWithError("Bad request"); // 400 Bad Request
    std::cout << "- Encoding: " << encoding << std::endl;
}

void Server::parseRequest(int fd, std::string request) {
    std::map<std::string, std::string> headers;
    std::string encoding, delimiter;
    std::string key, value;
    std::string line;

    int index = findClientIndex(fd);
    std::istringstream iss(request);

    if (clientSockets[index].header == false) {
        log("Parsing request");
        if (std::getline(iss, line)) {
            std::istringstream is(line);
            is >> method >> uri >> http; // check query string
            if (uri.find("?") != std::string::npos)
                query = uri.substr(uri.find("?") + 1, uri.size() - uri.find("?"));
        } else exitWithError("Bad request"); // 400 Bad Request
        while (std::getline(iss, line)) {
            if (line.compare("\r") == 0) {
                clientSockets[index].header = true;
                break;
            }
            if (line.find(":") != std::string::npos) {
                key = line.substr(0, line.find(":") + 1);
                value = line.substr(line.find(":") + 2, line.size() - line.find(":") - 3);
                if (!key.empty() && !value.empty())
                    headers[key] = value;
                else exitWithError("Bad request"); // 400 Bad Request
            }
        }
        setEncoding(headers, encoding, index);
        if (headers.find("Content-Length:") != headers.end())
            clientSockets[index].toRead = std::stoi(headers["Content-Length:"]);
        else clientSockets[index].toRead = 0;
        
        clientSockets[index].setHeaders(headers);
        clientSockets[index].setMethod(method);
        clientSockets[index].setURI(uri);
        clientSockets[index].setHTTP(http);
        clientSockets[index].setEncoding(encoding);
        clientSockets[index].setReqFile("reqBody");
    }

    if (std::string(fileName.str()).empty())
        fileName << "request-" << std::to_string(fd);
    file = std::fstream(fileName.str(), std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    if (!file.is_open())
        exitWithError("Can't open request file");

    int end = request.find("\r\n\r\n");
    if (end != std::string::npos) {
        end += 4;
        request = request.erase(0, end);
    }
    file.write(request.c_str(), request.size());
    clientSockets[index].readed += request.size();

    std::cout << "to read: " << clientSockets[index].toRead << " readed: " << clientSockets[index].readed << std::endl;
    if (clientSockets[index].readed >= clientSockets[index].toRead) {
        log("------ Request body received ------\n");
        clientSockets[index].received = true;
    } else
        log("------ Request body not received ------\n");
}

void Server::buildResponse() {
    std::string html = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size() << "\n\n" << html;
    responseMsg = ss.str();
}

void Server::sendResponse(int &clientFd) {
    int index = findClientIndex(clientFd);
    buildResponse();

    // read request body from file
    if (file.is_open()) {
        log("reading from file....");
        file.close();
    } else exitWithError("Can't open file");

    size_t bytesSent;
    bytesSent = send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);
    if (bytesSent >= responseMsg.size())
        log("------ Response sent to client ------\n\n");
    else
        exitWithError("Error sending response to client");
    
    // cleanup(); // TODO: clean resources
    clientSockets[index].cleanup();
    clientSockets.erase(clientSockets.begin() + index);
    FD_CLR(clientFd, &writeSetTmp);
    close(clientFd);

    // delete file after sending response
    std::string s(fileName.str());
    if (std::remove(s.c_str()))
        exitWithError("Error removing file");
}

void Server::handleRequest(int &clientFd) {
    log("------ Handling existing clients ------\n");
    std::cout << "Client " << clientFd << std::endl << std::endl;
    char buff[BUFFER_SIZE] = {0};

    std::string request;
    int index = findClientIndex(clientFd);
    size_t bytesRead = recv(clientFd, buff, BUFFER_SIZE, 0); // check body limit

    if (bytesRead <= 0) {
        if (bytesRead == 0)
            log("------ Connection closed -------\n\n");
        else exitWithError("Couldn't read from socket");

        clientSockets.erase(clientSockets.begin() + index);
        FD_CLR(clientFd, &readSetTmp);
        close(clientFd);
    } else {
        buff[bytesRead] = '\0';
        request = std::string(buff, bytesRead);
        parseRequest(clientFd, request);

        if (clientSockets[index].received == true) {
            FD_CLR(clientFd, &readSetTmp);
            FD_SET(clientFd, &writeSetTmp);
            log("------ Received Request from client ------\n\n");
        }
    }
    // else exitWithError("Request too big");
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

    setNonBlocking(serverSocket);
    startListning();
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
            if (FD_ISSET(clientFd, &writeSet))
                sendResponse(clientFd);
        }

        readSet = readSetTmp;
        writeSet = writeSetTmp;
    }
    close(serverSocket);
}