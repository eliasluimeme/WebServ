#include "../Includes/Includes.hpp"

Response::Response() {}

Response::~Response() {}

bool Response::sendResponse(int clientFd, std::string &responseMsg) {
    size_t bytesSent;
    bytesSent += send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);

    if (bytesSent >= responseMsg.size())
        return true;
    else return false;
}

void Response::buildResponse(Client &cl, Data &serverData, std::string &filename) {
    client = cl;
    CGI cgi;
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);

    std::string uri = client.getURI();
    std::string cgiLocation = serverData.locations["/cgi-bin"].root;
    std::cout << "cgi location " << cgiLocation << std::endl;

    if (uri.find("/cgi-bin") >= 0)
        cgi.start(client, serverData, filename);

    if (file.is_open()) {
        std::string line;
        // while (getline(file, line))
        //     std::cout << line << std::endl;
        file.close();
    } else {
        std::cout << "Can't open request file.." << std::endl;
        exit(EXIT_FAILURE);
    }


    std::string responseMsg;
    // sendResponse(client.getFd(), responseMsg);

    // Delete file after sending request
    // if (std::remove(filename.c_str())) {
    //     std::cout << "Error removing file" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
}