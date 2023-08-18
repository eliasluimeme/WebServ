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
    // std::map<std::string, std::string> h = client.getHeaders();
    // for (std::map<std::string, std::string>::iterator it, it = h.begin(); it != h.end(); ++it)
    //     std::cout << it->first << " " << it->second << std::endl;
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
    std::cout << "- Encoding: " << encoding << std::endl;
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

void Request::processChunked(Client &client, std::string &chunks, std::string &filename) {
    // std::cout << chunks << std::endl;
	std::string	body = "";
	std::string	subchunk = "";
	size_t		i = 0;
    // int chunksize = 0;
    int end = 0;
    int readed;
    readed = 0;
    re = 0;

	std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    if (file.is_open()) {
	    while (1) {
            if (client.left) {
                if (!client.left)
                    break;
                std::cout << "In Left: " << client.left << std::endl;
                body = chunks.substr(0, client.left);
                std::cout << "Read in left: " << body.size() << std::endl;
                client.readed += body.size();
                client.left -= body.size();
                if (!body.size())
                    break;
                if (client.left < 0)
                    exitWithError("ERRRRROR");
                file.write(body.c_str(), body.size());
                chunks.erase(0, client.left + 2);
                continue;
            }
            end = chunks.find("\r\n");
            if (end < 0)
                break;
            subchunk = chunks.substr(0, end);
            std::cout << "End: " << end << std::endl;
            if (isHexa(subchunk)) {
                client.chunkSize = strtol(subchunk.c_str(), NULL, 16);
                chunks.erase(0, end + 2);
                std::cout << "Subchunk: " << subchunk << std::endl;
                std::cout << "Chunksize: " << client.chunkSize << std::endl;
            }
            if (!client.chunkSize)
                break;

	    	body = chunks.substr(0, client.chunkSize);
            re = body.size();
            client.readed += re;
            if(!re)
                break;
            client.left = client.chunkSize - re;
            file.write(body.c_str(), body.size());
            chunks.erase(0, client.chunkSize + 2);

            std::cout << "Readed: " << re << std::endl;
            std::cout << "client.left: " << client.left << std::endl;
            // std::cout << "Total: " << tot << std::endl;
            if (client.left)
                break;
            body.clear();
	    }

        file.close();
    } else {
        std::cout << "Error opening file" << std::endl;
        exit(1);
    }
}

void Request::parseRequest(Client &client) {
    std::string request = client.getRequest();
    // std::cout << request << std::endl;
    std::map<std::string, std::string> headers;
    std::string encoding, delimiter;
    std::string key, value;
    std::string line;

    std::istringstream iss(request);

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
            client.toRead = std::stoi(headers["Content-Length"]);
        else client.toRead = 0;
        // if (client.toRead > BODY_SIZE)
        //     exitWithError("Request too big"); // 413 Request Intity Too Large

        int end = request.find("\r\n\r\n");
        if (end != std::string::npos) {
            end += 4;
            request = request.erase(0, end);
            // client.setRequest(request);
        }

        client.setMethod(method);
        client.setURI(uri);
        client.setQuery(query);
        client.setHTTP(http);
    }

    std::stringstream fileName;
    fileName << "request-" << std::to_string(client.getFd());
    std::fstream file = std::fstream(fileName.str(), std::ios::out | std::ios::app | std::ios::binary);
    if (!file.is_open())
        exitWithError("Can't open request file..");

    // parse body
    if (client.encoding == "chunked") {
        std::string name(fileName.str());
        processChunked(client, request, name);
    }
    else if (client.encoding == "length")
        file.write(request.c_str(), request.size());

    std::cout << "to read: " << client.toRead << " readed: " << client.readed << std::endl;
    if (client.readed >= client.toRead) {
        log("------ Request body received ------\n");
        client.received = true;
        file.close();
    } else
        log("------ Request body not received ------\n");
}