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

bool Response::buildResponse(Client &cl, Data &serverData, std::string &filename) {
    // cl;
    CGI cgi;
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);

    // std::string uri = client.getURI();
    // std::string cgiLocation = serverData.locations["/cgi-bin"].root;
    // std::cout << "cgi location " << cgiLocation << std::endl;
    // if (uri.find("/cgi-bin"))
    //     cgi.start(client, serverData, filename);

    std::cout << "Method : " << cl.getMethod() << std::endl;
    if(cl.getMethod().compare("GET") == 0)
        return GetMethod(cl);
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
int get_file(std::ifstream &a, int i, Client &c)
{
    // std::cout << "SIZE FIle : " << i << std::endl;
    // char buffer[30];
    std::string responses;
    // std::sprintf(buffer, "%d", i);
    // std::cout << "BUFFER : " << buffer << std::endl;
    char buffer2[i];
    a.read(buffer2, i);
    c.setPos(a.tellg());
    responses.append(buffer2, i);
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

bool Response::GetMethod(Client &a)
{
    Request req;
    std::streampos ab;
    std::ifstream uri_file(a.getURI().c_str());
    struct stat s;
    std::string responses = "HTTP/1.1 200 OK\r\nContent-Type: video/mp4\r\nTransfer-encoding: chunked\r\nConnection: keep-alive \r\n\r\n";
    // send_headers = false;

    if(!uri_file.is_open())
        req.ft_error(404, a.getFd());
    uri_file.seekg(a.getPos());
    if(stat(a.getURI().c_str(), &s) != 0)
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
            send(a.getFd(), responses.c_str(), responses.length(), 0);
            client.headerSent = true;
            std::cout << "hello\n";
        }
        else
        {
            if(get_file(uri_file, 3000, a))
                return 1; // Function of uri file
            std::cout << "BYEEEE\n";
            exit(17);
        }
    }
    else if(S_ISDIR(s.st_mode))
        get_directory(a.getURI()); // Function of uri dir 
    std::cout << "hnaaaa " << client.headerSent << std::endl;
    return false;
}