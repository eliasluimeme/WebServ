#include "../Includes/Includes.hpp"

CGI::CGI() {}

CGI::~CGI() {
    // delete[] env;
}

std::string CGI::toUpper(std::string str) {
    std::string ss;
    for (int i = 0; i < str.size(); i++)
        ss += std::toupper(str[i]);
    return ss;
}

void CGI::getEnv(Client &client) {
    std::fstream f;
    f.open("test.txt", std::ios::in | std::ios::out | std::ios::app);
    if (!f.is_open())
        exit(EXIT_FAILURE);

    std::map<std::string, std::string> headers = client.getHeaders();
    std::stringstream ss;
    std::vector<char*> ptr;
    std::string str;

    ptr.push_back(const_cast<char*>(std::string("REQUEST_METHOD=" + client.getMethod()).c_str()));
    ptr.push_back(const_cast<char*>(std::string("CONTENT_TYPE=" + headers["Content_type"]).c_str()));
    ss << client.toRead;
    ptr.push_back(const_cast<char*>(std::string("CONTENT_LENGTH=" + ss.str()).c_str()));
    ss.clear();
    ptr.push_back(const_cast<char*>(std::string("QUERY_STRING=" + client.getQuery()).c_str()));
    ss << inet_ntoa(client.getAddr().sin_addr);
    ptr.push_back(const_cast<char*>(std::string("REMOTE_ADDR=" + ss.str()).c_str()));
    ss.clear();
    ss << ntohs(client.getAddr().sin_port);
    str = std::string("REMOTE_PORT=" + ss.str());
    ptr.push_back(const_cast<char*>(str.c_str())); // AM HEEEEEEEEEEEEEERE!!
    ss.clear();
    str = std::string("REQUEST_URI =" + client.getURI());
    ptr.push_back(const_cast<char*>(str.c_str()));
    str = std::string("QUERY_STRING=" + client.getQuery());
    ptr.push_back(const_cast<char*>(str.c_str()));
    str = std::string("SCRIPT_NAME=" + client.getURI().substr(0, client.getURI().find("?")));
    ptr.push_back(const_cast<char*>(str.c_str()));

    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
        str = "HTTP_" + toUpper(it->first) + "=" + it->second;
        ptr.push_back(const_cast<char*>(str.c_str()));
    }

    f << ptr.size();

    env = new char*[ptr.size() + 1];
    env[ptr.size()] = NULL;
    for (size_t i = 0; i < ptr.size(); i++) {
        env[i] = ptr[i];
        f << env[i] << std::endl;
    }

}

void CGI::start(Client &client, Data &serverData, std::string &filename) {
    char **av;
    int cgiInput[2];
    int cgiOutput[2];

    pipe(cgiInput);
    pipe(cgiOutput);

    pid_t pid = fork();
	if (pid == -1)
		perror("fork failed..");
	else if (pid == 0) {
        close(cgiInput[1]);
        close(cgiOutput[0]);

        dup2(cgiInput[0], STDIN_FILENO);
        dup2(cgiOutput[1], STDOUT_FILENO);

        getEnv(client);

        // av[0] = "python-cgi";
        // av[1] = "t.py";
		// execve(av[0], av, getEnv(client));
        // delete[] env;
		exit(1);
	}
    else {
        close(cgiInput[0]);
        close(cgiOutput[1]);

        char buff[4069];
        size_t bytesRead;
        std::string response;
        struct timeval startTime;
        std::time_t time = std::time(NULL);
        startTime.tv_sec = static_cast<time_t>(time);
        while((bytesRead = read(cgiOutput[0], buff, sizeof(buff)) > 0)) {
            if(startTime.tv_sec - std::time(NULL) >= CGI_TIMEOUT)
                perror("cgi timeout..");
            response += std::string(buff, bytesRead);
        }

	    waitpid(pid, NULL, 0);
    }
	kill(pid, SIGTERM);
}
