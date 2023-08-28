#include "../Includes/Includes.hpp"

Response::Response() {}

Response::~Response() {}

// bool Response::sendResponse(int clientFd, std::string &responseMsg) {
//     size_t bytesSent;
//     bytesSent += send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);

//     if (bytesSent >= responseMsg.size())
//         return true;
//     else return false;
// }

// void Response::buildResponse(Client &cl, Data &serverData, std::string &filename) {
//     client = cl;
//     CGI cgi;
//     std::fstream file;
//     file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);

//     std::string uri = client.getURI();
//     std::string cgiLocation = serverData.locations["/cgi-bin"].root;
//     std::cout << "cgi location " << cgiLocation << std::endl;

//     if (uri.find("/cgi-bin") >= 0)
//         cgi.start(client, serverData, filename);

//     if (file.is_open()) {
//         std::string line;
//         // while (getline(file, line))
//         //     std::cout << line << std::endl;
//         file.close();
//     } else {
//         std::cout << "Can't open request file.." << std::endl;
//         exit(EXIT_FAILURE);
//     }


//     std::string responseMsg;
//     // sendResponse(client.getFd(), responseMsg);

//     // Delete file after sending request
//     // if (std::remove(filename.c_str())) {
//     //     std::cout << "Error removing file" << std::endl;
//     //     exit(EXIT_FAILURE);
//     // }
// }

bool Response::sendResponse(int clientFd, std::string &responseMsg) {
    size_t bytesSent;
    bytesSent += send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);

    if (bytesSent >= responseMsg.size())
        return true;
    else return false;
}

bool Response::buildResponse(Client &client, Data &serverData, std::string &filename) {
    // client = cl;
    CGI cgi;
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);

    // std::string uri = client.getURI();
    // std::string cgiLocation = serverData.locations["/cgi-bin"].root;
    // std::cout << "cgi location " << cgiLocation << std::endl;
    // if (uri.find("/cgi-bin"))
    //     cgi.start(client, serverData, filename);

    std::cout << "Method : " << client.getMethod() << std::endl;
    if(client.getMethod().compare("GET") == 0)
        return GetMethod(client);
    // if (file.is_open()) {
    //     std::string line;
    //     // while (getline(file, line))
    //     //     std::cout << line << std::endl;
    //     file.close();
    // } else {
    //     std::cout << "Can't open request file.." << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // std::string responseMsg;
    // sendResponse(client.getFd(), responseMsg);

    // Delete file after sending request
    // if (std::remove(filename.c_str())) {
    //     std::cout << "Error removing file" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    return false;
}
int get_file(std::ifstream &a, Client &c)
{
    // std::cout << "SIZE FIle : " << i << std::endl;
    // char buffer[30];
    std::string responses;
    // std::sprintf(buffer, "%d", i);
    // std::cout << "BUFFER : " << buffer << std::endl;
    char buffer2[3000];
    a.read(buffer2, 3000);
    c.setPos(a.tellg());
    responses = std::string(buffer2, 3000);
    size_t sent = write(c.getFd(), responses.c_str(), responses.length());

    if (a.eof()) {
        // close(c.getFd());
        // exit(0);
        return 1;
    }
    return 0;
}

void get_directory(std::string a)
{

}

void sendImagePortion(int clientSocket, const std::string& filePath, int offset, int portionSize) {
    std::ifstream file;
    file.open(filePath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the image file" << std::endl;
        return;
    }

    // Move the file pointer to the specified offset
    file.seekg(offset);

    char buffer[portionSize];
    int totalSent = 0;
    int bytesRead = 0;

    while (totalSent < portionSize && (bytesRead = file.readsome(buffer, portionSize))) {
        // Send the portion of the image
        int bytesToSend = std::min(bytesRead, (int)portionSize - totalSent);
        send(clientSocket, buffer, bytesToSend, 0);
        totalSent += bytesToSend;
    }

    file.close();
}

bool Response::GetMethod(Client &client)
{
    // int fileSize;
    Request req;
    std::streampos ab;
    struct stat s;
    std::ifstream uri_file(client.getURI().c_str());

    if(!uri_file.is_open())
        req.ft_error(404, client.getFd());


    // std::cout << "Position :  " << client.getPos() << std::endl;
    // uri_file.seekg(client.getPos());
    if(stat(client.getURI().c_str(), &s) != 0)
    {
        std::cout << "ERROR with STAT function" << std::endl;
        uri_file.close();
        // exit(1);
    }
    if(S_ISREG(s.st_mode) && (s.st_mode && S_IRUSR))
    {
        std::cout << "HI\n\n";
        std::cout << "BOOL : " <<  client.headerSent << std::endl;
        if(client.headerSent == false)
        {
            uri_file.seekg(0, std::ios::end);
            client.fileSize = uri_file.tellg();
            uri_file.seekg(0, std::ios::end);

            std::ostringstream responses;
            responses << "HTTP/1.1 200 OK\r\n";
            responses << "Content-Type: image/jpeg\r\n";
            responses << "Content-Length: " << client.fileSize << "\r\n";
            responses << "\r\n";
            send(client.getFd(), responses.str().c_str(), responses.str().size(), 0);
            client.headerSent = true;
            std::cout << "hello\n";
        }
        else
        {
            std::cout << "fileSize " << client.fileSize << std::endl;
            int portionSize = 1024 * 1024;
            if (client.offset < client.fileSize) {
                sendImagePortion(client.getFd(), client.getURI(), client.offset, portionSize);
                client.offset += portionSize;
                std::cout << "Offset " << client.offset << std::endl;
            }
            else {
                std::cout << "BYEEEE\n";
                return true;
            }
            // if(get_file(uri_file, client))
        }
    }
    else if(S_ISDIR(s.st_mode))
        get_directory(client.getURI()); // Function of uri dir 
    return false;
}