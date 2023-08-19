#include "../Includes/Includes.hpp"

Request::Request() {}

Request::~Request() {}

void Request::log(std::string message) {
    std::cout << message << std::endl;
}

void Request::exitWithError(const std::string error) {
    std::cerr << "[-] Error: " << error << std::endl;
    // cleanup();
    // closeServer();
    exit(EXIT_FAILURE);
}

void Request::setEncoding(Client &client, std::string &encoding) {
    if (client.getHeaders().find("Transfer-Encoding") != client.getHeaders().end()) {
        if (client.getHeaders()["Transfer-Encoding"] == "chunked")
            encoding = "chunked";
        else exitWithError("Invalid Transfer-Encoding"); // 501 Not Implemented
    }
    else if (client.getHeaders().find("Content-Type") != client.getHeaders().end() && client.getHeaders()["Content-Type"].find("multipart/form-data;") != std::string::npos) {
        std::string val = client.getHeaders()["Content-Type"];
        delimiter = val.substr(val.find("=") + 1, val.size() - val.find("="));
        client.setDelimiter(delimiter);
        encoding = "multipart";
    }
    else if (client.getHeaders().find("Content-Length") != client.getHeaders().end())
        encoding = "length";
    else if (method == "POST") {
        if (encoding != "chunked" && encoding != "length" && encoding != "multipart")
            exitWithError("Invalid encoding"); // 400 Bad Request
    }
}

bool Request::isHexa(std::string &str) {
    bool is = false;
    for (int i = 0; i < str.size(); i++) {
        if (std::isxdigit(str[i]))
            is = true;
        else {
            is = false;
            break;
        }
    }
    return is;
}

void Request::processChunked(Client &client, std::string &filename) {
    std::string chunks = client.getRequest();
	std::string	body = "";
	std::string	subchunk = "";
	size_t		i = 0;
    int         readed = 0;
    int         end = 0;
    bool        flag = false;

	std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    if (file.is_open()) {
	    while (1) {
            // std::cout << "Body size: " << chunks.size() << std::endl;
            if (!chunks.size())
                break;
            if (client.left) {
                // std::cout << "In Left: " << client.left << std::endl;
                body = chunks.substr(0, client.left);
                // std::cout << "Read in left: " << body.size() << std::endl;
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
                    if (chunks.size() < 10) {
                        client.leftInChunk = chunks;
                        // std::cout << "client.leftInChunk: " << client.leftInChunk << std::endl;
                        break;
                    }
                    std::cout << "Body: " << body << std::endl;
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
                    // std::cout << "Subchunk: " << subchunk << std::endl;
                    // std::cout << "Chunksize: " << client.chunkSize << std::endl;
                }
                if (!client.chunkSize)
                    break;
            }
	    	body = chunks.substr(0, client.chunkSize);
            readed = body.size();
            if(!readed)
                break;
            client.readed += readed;
            client.left = client.chunkSize - readed;
            file.write(body.c_str(), body.size());
            chunks.erase(0, readed);

            // std::cout << "Readed: " << readed << std::endl;
            // std::cout << "client.left: " << client.left << std::endl;
            if (client.left)
                break;
            body.clear();
	    }
        file.close();
    } else exitWithError("Cant open file.");
}

void Request::parseRequest(Client &client) {
    std::string request = client.getRequest();
    std::map<std::string, std::string> headers;
    std::istringstream iss(request);
    std::string encoding, delimiter;
    std::string key, value, line;

    if (client.header == false) {
        if (std::getline(iss, line)) {
            std::istringstream is(line);
            is >> method >> uri >> http;
            if (method.compare("GET") && method.compare("POST") && method.compare("DELETE"))
                exitWithError("Bad request"); // 400 Bad Request
            if (uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
                exitWithError("Bad request"); // 400 Bad Request
            if (uri.size() > 2048)
                exitWithError("URI too long"); // 414 URI Too Long
            if (uri.find("?") != std::string::npos)
                query = uri.substr(uri.find("?") + 1, uri.size() - uri.find("?"));
        } else exitWithError("Bad request"); // 400 Bad Request
        while (std::getline(iss, line)) {
            if (line.compare("\r") == 0) {
                client.header = true;
                break;
            }
            if (line.find(":") != std::string::npos) {
                key = line.substr(0, line.find(":"));
                value = line.substr(line.find(":") + 2, line.size() - line.find(":") - 3);
                if (!key.empty() && !value.empty())
                    headers[key] = value;
                else exitWithError("Bad request"); // 400 Bad Request
            }
        }
        client.setHeaders(headers);
        setEncoding(client, encoding);
        client.setEncoding(encoding);
        if (headers.find("Content-Length") != headers.end())
            client.toRead = atoi(headers["Content-Length"].c_str());
        else client.toRead = 0;
        // if (client.toRead > BODY_SIZE)
        //     exitWithError("Request too big"); // 413 Request Intity Too Large

        int end = request.find("\r\n\r\n");
        if (end != std::string::npos)
            request = request.erase(0, end + 4);

        client.setRequest(request);
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

    // parse body
    std::string name(fileName.str());
    if (client.encoding == "chunked")
        processChunked(client, name);
    else if (client.encoding == "length") {
        file.write(request.c_str(), request.size());
        client.readed += request.size();
    }

    std::cout << "to read: " << client.toRead << " readed: " << client.readed << std::endl;
    if (client.readed >= client.toRead) {
        log("------ Request body received ------\n");
        client.received = true;
        file.close();
    } else
        log("------ Request body not received ------\n");
}