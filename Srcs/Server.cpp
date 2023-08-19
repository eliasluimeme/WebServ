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
    for (std::vector<Client>::iterator it = clientSockets.begin(); it != clientSockets.end(); it++)
        close(it->getFd());
    close(serverSocket);
    // exit(EXIT_FAILURE);
}

void Server::cleanup() {
    log("cleaning...");
    // if (file.is_open())
    //     file.close();
    endHeader = false;
}

void Server::setConfData(Data &confData) {
    data = confData;
}

Data &Server::getConfData() {
    return data;
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

void Server::buildResponse() {
    std::string html = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size() << "\n\n" << html;
    responseMsg = ss.str();
}

void Server::sendResponse(int &clientFd) {
    Response response;
    std::stringstream fileName;
    int index = findClientIndex(clientFd);
 
    buildResponse();
    std::stringstream ss;
    ss << clientSockets[index].getFd();
    fileName << "request-" << ss.str();
    std::string name(fileName.str());
    response.buildResponse(clientSockets[index], name, responseMsg);

    size_t bytesSent;
    bytesSent = send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);
    if (bytesSent >= responseMsg.size())
        log("------ Response sent to client ------\n\n");
    else
        exitWithError("Error sending response to client");
    
    cleanup(); // TODO: clean resources
    clientSockets[index].cleanup();
    clientSockets.erase(clientSockets.begin() + index);
    FD_CLR(clientFd, &writeSetTmp);
    close(clientFd);

    // delete file after sending response
    // std::string s(fileName.str());
    // if (std::remove(s.c_str()))
    //     exitWithError("Error removing file");
}

void Server::handleRequest(int &clientFd) {
    log("------ Handling existing clients ------\n");
    std::cout << "Client " << clientFd << std::endl << std::endl;
    char buff[BUFFER_SIZE] = {0};

    std::string request;
    int index = findClientIndex(clientFd);

    size_t bytesRead = read(clientFd, buff, BUFFER_SIZE); // check body limit
    if (bytesRead <= 0) {
        if (bytesRead == 0)
            log("------ Connection closed -------\n\n");
        else log("[-] Couldn't read from socket");

        clientSockets.erase(clientSockets.begin() + index);
        FD_CLR(clientFd, &readSetTmp);
        close(clientFd);
    } else {
        Request req;
        request = std::string(buff, bytesRead);
        clientSockets[index].setRequest(request);
        req.parseRequest(clientSockets[index]);

        if (clientSockets[index].received == true) {
            FD_CLR(clientFd, &readSetTmp);
            FD_SET(clientFd, &writeSetTmp);
            log("------ Received Request from client ------\n\n");
        }
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
    client.setAddr(clientAddr);
    client.setAddrLen(clientAddrLength);
    client.setConfData(data);
    clientSockets.push_back(client);

    FD_SET(clientSocket, &readSetTmp);
    maxFd = std::max(maxFd, clientSocket);
}

bool Server::initServer() {
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
    signal(SIGPIPE, SIG_IGN);
    // if (setsockopt(serverSocket, SOL_SOCKET, SO_NOSIGPIPE, (char *)&i, sizeof(i)) < 0)
        // exitWithError("Couldn't set SO_NOSIGPIPE");
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) < 0)
        exitWithError("Couldn't set SO_REUSEADDR");
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        exitWithError("Couldn't bind socket");

    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    setNonBlocking(serverSocket);

    if (listen(serverSocket, MAX_CLIENTS) < 0)
        exitWithError("Couldn't listen to socket");

    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " << inet_ntoa(serverAddr.sin_addr) << " PORT: " << ntohs(serverAddr.sin_port) << " ***\n\n";
    log(ss.str());

    FD_SET(serverSocket, &readSet);
    maxFd = serverSocket;
    return true;
}

void Server::startServer(Data &confData) {
    if (initServer())
        setConfData(confData);

    timeout.tv_sec = 0;
    timeout.tv_usec = 200000;

    // std::ofstream outputFile("output.txt");
    // // Store the current state of std::cout
    // std::streambuf* coutBuffer = std::cout.rdbuf();
    // // Redirect std::cout to the file
    // std::cout.rdbuf(outputFile.rdbuf());

    FD_ZERO(&readSetTmp);
    FD_ZERO(&writeSetTmp);
    while (true) {
        int maxFdTmp = maxFd;
        readSetTmp = readSet;
        writeSetTmp = writeSet;

        log("====== Waiting for a new connection ======\n\n");
        if (select(maxFdTmp + 1, &readSet, &writeSet, NULL, NULL) < 0)
            exitWithError("Select failed!");

        if (FD_ISSET(serverSocket, &readSet)) 
            acceptConnection();

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
    // // Restore the original state of std::cout
    // std::cout.rdbuf(coutBuffer);
    // outputFile.close();
    close(serverSocket);
}