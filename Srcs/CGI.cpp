#include "../Includes/Includes.hpp"

CGI::CGI() {}

CGI::~CGI() {
    // delete[] env;
}

std::string CGI::toUpper(std::string str) {
    std::string ss;
    for (int i = 0; i < static_cast<int>(str.size()); i++)
        ss += std::toupper(str[i]);
    return ss;
}

void exitWithError(const std::string &msg) {
    std::cerr << "[-] Error: " << msg << std::endl;
    exit(EXIT_FAILURE);
}

void CGI::errors(int status, Client &client) {
    std::cout << "Error in request sent. Cleaning.." << std::endl;

    std::map<int, std::string> error;

    error[501] = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length: 72\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>501 Not Implemented </h1></center></body></html>\r\n";
    error[400] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 68\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>400 Bad Request </h1></center></body></html>\r\n";
    error[408] = "HTTP/1.1 408 Request Timeout\r\nContent-Type: text/html\r\nContent-Length: 72\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>408 Request Timeout </h1></center></body></html>\r\n";
    error[414] = "HTTP/1.1 414 Request-URI Too Long\r\nContent-Type: text/html\r\nContent-Length: 77\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>414 Request-URI Too Long </h1></center></body></html>\r\n";
    error[413] = "HTTP/1.1 413 Request Entity Too Large\r\nContent-Type: text/html\r\nContent-Length: 81\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>413 Request Entity Too Large </h1></center></body></html>\r\n";
    error[404] = "HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/html\r\nContent-Length: 66\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>404 NOT FOUND </h1></center></body></html>\r\n";
    error[409] = "HTTP/1.1 409 CONFLICT\r\nContent-Type: text/html\r\nContent-Length: 64\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>409 CONFLICT </h1></center></body></html>\r\n";
    error[403] = "HTTP/1.1 403 FORBIDDEN\r\nContent-Type: text/html\r\nContent-Length: 66\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>403 FORBIDDEN </h1></center></body></html>\r\n";
    error[500] = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: 66\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>Server couldn't handle your request. Still, you won't kill it so easily !</h1></center></body></html>\r\n";
    
    if (send(client.getFd(), error[status].c_str(), strlen(error[status].c_str()), 0) <= 0)
        std::cerr << "[-] Sending error response to client failed. Connection closed" << std::endl;

    client.state = CLEAR;
}

void CGI::getEnv(Client &client) {
    std::stringstream ss;
    std::vector<char*> ptr;
    std::map<std::string, std::string> headers = client.getHeaders();

    ptr.push_back(strdup(std::string("REDIRECT_STATUS=302").c_str()));
    ptr.push_back(strdup(std::string("REQUEST_METHOD=" + client.getMethod()).c_str()));
    ptr.push_back(strdup(std::string("CONTENT_TYPE=" + headers["Content-Type"]).c_str()));
    ss << client.toRead;
    ptr.push_back(strdup(std::string("CONTENT_LENGTH=" + ss.str()).c_str()));
    ss.str("");
    ptr.push_back(strdup(std::string("QUERY_STRING=" + client.getQuery()).c_str()));
    ss << inet_ntoa(client.getAddr().sin_addr);
    ptr.push_back(strdup(std::string("REMOTE_ADDR=" + ss.str()).c_str()));
    ss.str("");
    ptr.push_back(strdup(std::string("REQUEST_URI=" + client.getURI()).c_str()));
    ptr.push_back(strdup(std::string("QUERY_STRING=" + client.getQuery()).c_str()));
    ptr.push_back(strdup(std::string("SCRIPT_NAME=/nfs/homes/iakry/Downloads/webserver-main/webserv/var" + client.getURI().substr(0, client.getURI().find("?"))).c_str()));
    ptr.push_back(strdup(std::string("SCRIPT_FILENAME=/nfs/homes/iakry/Downloads/webserver-main/webserv/var" + client.getURI().substr(0, client.getURI().find("?"))).c_str()));
    ptr.push_back(strdup(std::string("PATH_INFO=/nfs/homes/iakry/Downloads/webserver-main/webserv/var" + client.getURI().substr(0, client.getURI().find("?"))).c_str()));
    // std::cout << "PAAATH " << ptr[ptr.size() - 1] << std::endl;

    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
        ptr.push_back(strdup(std::string("HTTP_" + toUpper(it->first) + "=" + it->second).c_str()));
    }

    env = new char*[ptr.size() + 1];
    for (size_t i = 0; i < ptr.size(); i++) 
        env[i] = ptr[i];
    env[ptr.size()] = NULL;

    if (env == NULL)
        perror("No env");
}

std::string CGI::start(Client &client, std::string &location)
{
	std::string outFile("out_file");
	std::string msg;

    std::stringstream inFile;
    std::stringstream tmp;

    tmp << client.getFd();
    inFile << "./request-" << tmp.str();

	int input = open(inFile.str().c_str(), O_RDONLY);
	int output = open(outFile.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0755);

	pid_t pid = fork();
	if (pid == -1)
		return "";
	if (pid == 0)
	{
		if (dup2(input, 0) == -1 || dup2(output, 1) == -1)
            exitWithError("dup failed");

        getEnv(client);

        std::string interpreter;
        if (location.find(".py") != std::string::npos)
            interpreter = "/usr/bin/python3";
        else if (location.find(".php") != std::string::npos)
            interpreter = "/usr/bin/php-cgi";

        char *av[] = {const_cast<char*>(interpreter.c_str()), const_cast<char*>(location.c_str()), NULL};
 
	    execve(av[0], av, env);

		exit(1);
	}

	int timeout = CGI_TIMEOUT;
	int start_time = time(NULL);
	std::string response("HTTP/1.1 200 Ok\r\nContent-Length: ");

	while (1)
	{
        if (time(NULL) - start_time > timeout) {
            errors(408, client);
            kill(pid, SIGTERM);
            return std::string("");
        }

        int status = 0;
		pid_t pro = waitpid(pid, &status, WNOHANG);
		if (pro > 0)
		{
			close(input);
			close(output);

			std::ifstream file(outFile.c_str());
			std::stringstream buffer;

			buffer << file.rdbuf();
			msg = buffer.str();
			if (msg.size() == 0) {
				errors(400, client);
                return std::string("");
            }
			if (msg.find("Status:") != std::string::npos) {
                std::stringstream ss;
                ss << msg.substr(outFile.find("Status:") + 9, 3);
				errors(atoi(ss.str().c_str()), client);
                return std::string("");
			}
			std::stringstream ss;
			ss << (msg.substr(msg.find("\r\n\r\n") + 4, msg.size())).size();
			response += ss.str();
			response += "\r\n";
            if (location.find(".py") != std::string::npos)
                response += "\r\n";
			response += msg;
			file.close();
			return response;
		}
	}
	kill(pid, SIGTERM);
	return response;
}
