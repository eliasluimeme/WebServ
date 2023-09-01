#include "../Includes/Includes.hpp"

Request::Request() {}

Request::~Request() {}

int Request::findClientIndex(std::vector<Client> &clientSockets, int &clientSock) {
    int index = 0;
    for (std::vector<Client>::iterator it = clientSockets.begin(); it != clientSockets.end(); it++) {
        if (it->getFd() == clientSock)
            return index;
        index++;
    }
    exitWithError("Client not found");
    return -1;
}

void Request::reset(Client &client) {
    // int index = findClientIndex(clientSockets, fd);
    // clientSockets[index].cleanup();
    // clientSockets.erase(clientSockets.begin() + index);
    // FD_CLR(fd, &readSetTmp);
    // close(fd);
    client.state = CLEAR;
}

bool Request::ft_error(int status, Client &client) {
    std::cout << "Error in request sent. Cleaning.." << std::endl;
    std::map<int, std::string> error;
    error[501] = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length: 72\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>501 Not Implemented </h1></center></body></html>\r\n";
    error[400] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 68\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>400 Bad Request </h1></center></body></html>\r\n";
    error[408] = "HTTP/1.1 408 Request Timeout\r\nContent-Type: text/html\r\nContent-Length: 72\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>408 Request Timeout </h1></center></body></html>\r\n";
    error[414] = "HTTP/1.1 414 Request-URI Too Long\r\nContent-Type: text/html\r\nContent-Length: 77\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>414 Request-URI Too Long </h1></center></body></html>\r\n";
    error[413] = "HTTP/1.1 413 Request Entity Too Large\r\nContent-Type: text/html\r\nContent-Length: 81\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>413 Request Entity Too Large </h1></center></body></html>\r\n";
    error[404] = "HTTP/1.1 404 NOT FOND\r\nContent-Type: text/html\r\nContent-Length: 66\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>404 NOT FOUND </h1></center></body></html>\r\n";
    write(client.getFd(), error[status].c_str(), strlen(error[status].c_str()));

    reset(client);
    return false;
}

void Request::log(std::string message) {
    std::cout << message << std::endl;
}

void Request::exitWithError(const std::string error) {
    std::cerr << "[-] Error: " << error << std::endl;
    // cleanup();
    // closeServers();
    exit(EXIT_FAILURE);
}

bool Request::setEncoding(Client &client, std::string &encoding) {
    if (client.getHeaders().find("Content-Length") != client.getHeaders().end()) {
        encoding = "length";
        client.toRead = atoi(client.getHeaders()["Content-Length"].c_str()); // check atoi
    }
    if (client.getHeaders().find("Transfer-Encoding") != client.getHeaders().end()) {
        if (client.getHeaders()["Transfer-Encoding"] == "chunked")
            encoding = "chunked";
        else return ft_error(501, client);
    }
    if (client.getHeaders().find("Content-Type") != client.getHeaders().end() && client.getHeaders()["Content-Type"].find("multipart/form-data;") != std::string::npos)
        return ft_error(501, client);

    if (method == "POST") {
        if (encoding != "chunked" && encoding != "length")
            return ft_error(400, client);
    }
    else if (method == "GET" && encoding.empty())
        client.state = REQUEST_RECEAVED;

    client.setEncoding(encoding);
    return true;
}

bool Request::isHexa(std::string &str) {
    for (int i = 0; i < str.size(); i++) {
        if (!std::isxdigit(str[i]))
            return false;
    }
    return true;
}

void Request::processChunked(Client &client, std::string &filename, std::string &chunks) {
	std::string	body = "";
	std::string	subchunk = "";
	size_t		i = 0;
    int         readed = 0;
    int         end = 0;
    bool        flag = false;

	std::fstream file(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);
    // change to FILE *file = fopen("example.bin", "rb"); // Read binary
    if (file.is_open()) {
	    while (1) {
            if (!chunks.size())
                break;
            if (client.left) {
                body = chunks.substr(0, client.left);
                if (!body.size())
                    break;
                chunks.erase(0, client.left);
                client.readed += body.size();
                client.left -= body.size();
                // std::cout << "Left: " << client.left << std::endl;
                file.write(body.c_str(), body.size());
                if (client.left == 0 && chunks.size()) {
                    if (chunks.find("\r\n") == 0)
                        chunks.erase(0, 2);
                    if (chunks.size() < 15) {
                        client.leftInChunk = chunks;
                        break;
                    }
                }
                continue;
            }
            if (!client.leftInChunk.empty()) {
                chunks = client.leftInChunk + chunks;
                client.leftInChunk.clear();
                if (chunks.find("\r\n") == 0)
                    chunks.erase(0, 2);
            }
            if ((end = chunks.find("\r\n")) > 0) {
                // std::cout << "End: " << end << std::endl;
                subchunk = chunks.substr(0, end);
                if (isHexa(subchunk)) {
                    client.chunkSize = strtol(subchunk.c_str(), NULL, 16);
                    chunks.erase(0, end + 2);
                }
                if (client.chunkSize == 0) {
                    client.state = REQUEST_RECEAVED;
                    break;
                }
            }
	    	body = chunks.substr(0, client.chunkSize);
            readed = body.size();
            if(!readed)
                break;
            client.readed += readed;
            client.left = client.chunkSize - readed;
            file.write(body.c_str(), body.size());
            chunks.erase(0, readed);
            if (chunks.find("\r\n") == 0)
                    chunks.erase(0, 2);
            if (client.left)
                break;
            body.clear();
	    }
        file.close();
    } else exitWithError("Cant open file.");
}

bool Request::parseRequest(Client &client, std::string &request) {
    std::map<std::string, std::string> headers;
    std::istringstream iss(request);
    std::string encoding, delimiter;
    std::string key, value, line, left;

    if (client.header == false) {
        if (std::getline(iss, line)) {
            std::istringstream is(line);
            is >> method >> uri >> http >> left;
            if (!left.empty())
                return ft_error(400, client);
            if (method.compare("GET") && method.compare("POST") && method.compare("DELETE"))
                return ft_error(400, client);
            if (uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
                return ft_error(400, client);
            if (uri.size() > 2048)
                return ft_error(414, client);
            if (uri.find("?") != std::string::npos)
                query = uri.substr(uri.find("?") + 1, uri.size() - uri.find("?"));
        } else return ft_error(400, client); // exitWithError("Bad request"); // 400 Bad Request
        line.clear();
        while (std::getline(iss, line)) {
            if (line.compare("\r") == 0 || line.empty()) {
                if (line.empty())
                    return ft_error(400, client);
                client.header = true;
                break;
            }
            else if (line.find(":") != std::string::npos) {
                key = line.substr(0, line.find(":"));
                value = line.substr(line.find(":") + 2, line.size() - line.find(":") - 3);
                if (!key.empty() && !value.empty())
                    headers[key] = value;
                else return ft_error(400, client);
            } else return ft_error(400, client);
            line.clear();
        }
        client.setHeaders(headers);
        if (!setEncoding(client, encoding))
            return false;

        if (client.toRead > client.getConfData().bodySize)
            return ft_error(413, client);

        int end = request.find("\r\n\r\n");
        if (end != std::string::npos)
            request = request.erase(0, end + 4);
        if ((!request.size() && client.toRead) || (!client.getMethod().compare("GET") && (request.size() || client.toRead)))
            return ft_error(400, client);

        if (headers.find("Host") != headers.end()) {
            std::cout << "host " << headers["Host"] << std::endl;
            if (headers["Host"] != client.getConfData().serverName[0])
                client.reIndex = true;
        }

        client.setMethod(method);
        client.setURI(uri);
        client.setQuery(query);
        client.setHTTP(http);
    }

    std::stringstream fileName;
    std::stringstream temp;
    temp << client.getFd();
    fileName << "request-" << temp.str();
    std::fstream file(fileName.str().c_str(), std::ios::out | std::ios::app | std::ios::binary);
    if (!file.is_open())
        exitWithError("Can't open request file..");

    std::string name(fileName.str());
    if (client.encoding == "chunked")
        processChunked(client, name, request);
    else if (client.encoding == "length" && client.toRead != 0) {
        file.write(request.c_str(), request.size());
        client.readed += request.size();
        if (client.readed >= client.toRead)
            client.state == REQUEST_RECEAVED;
    }
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> handle when method is get and there is no content length
    if ((client.state == REQUEST_RECEAVED)) { // handle when body size > content length
        log("------     Request body received     ------\n");
        client.received = true;
        file.close();
    }
    return true;
}