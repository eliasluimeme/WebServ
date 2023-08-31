#include "../Includes/Includes.hpp"

Response::Response() {}

Response::~Response() {}

bool Response::sendResponse(int clientFd, std::string &responseMsg) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << responseMsg.size() << "\r\n\r\n";

    // Send the HTTP response header
    send(clientFd, response.str().c_str(), response.str().size(), 0);

    size_t bytesSent;
    bytesSent += send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);

    if (bytesSent >= responseMsg.size())
        return true;
    else return false;
}

bool Response::buildResponse(Client &client, Data &serverData, std::string &filename) {
    std::cout << "--------   Building Response   ---------\n" << std::endl;

    CGI cgi;
    std::string cgiMsg;
    std::fstream file(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    // change to FILE *file = fopen("example.bin", "rb"); // Read binary

    std::string uri = client.getURI();

    if ((uri.find(".php") > 0 && uri.find(".php") != std::string::npos) || (uri.find(".py") > 0 && uri.find(".py") != std::string::npos))
        cgiMsg = cgi.start(client, serverData, filename);
    else cgiMsg = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>"; // for demonstration
 
    if (file.is_open()) {
        std::string line;
        // while (getline(file, line))
        //     std::cout << line << std::endl;
        file.close();
    } else {
        std::cout << "Can't open request file.." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Delete file after sending request
    // if (std::remove(filename.c_str())) { // TODO remove files when an error accures
    //     std::cerr << "Error removing file" << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    return sendResponse(client.getFd(), cgiMsg); 
}
