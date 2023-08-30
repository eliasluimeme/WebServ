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
        perror("cant open test file");

    std::stringstream ss;
    std::vector<char*> ptr;
    std::map<std::string, std::string> headers = client.getHeaders();

    ptr.push_back(strdup(std::string("REQUEST_METHOD=" + client.getMethod()).c_str()));
    ptr.push_back(strdup(std::string("CONTENT_TYPE=" + headers["Content-Type"]).c_str()));
    ss << client.toRead;
    ptr.push_back(strdup(std::string("CONTENT_LENGTH=" + ss.str()).c_str()));
    ss.str("");
    ptr.push_back(strdup(std::string("QUERY_STRING=" + client.getQuery()).c_str()));
    ss << inet_ntoa(client.getAddr().sin_addr);
    ptr.push_back(strdup(std::string("REMOTE_ADDR=" + ss.str()).c_str()));
    ss.str("");
    // uint16_t port = ntohs(client.getAddr().sin_port);
    // ss << port;
    // ptr.push_back(strdup(std::string("REMOTE_PORT=" + ss.str()).c_str()));
    // ss.str("");
    ptr.push_back(strdup(std::string("REQUEST_URI =" + client.getURI()).c_str()));
    ptr.push_back(strdup(std::string("QUERY_STRING=" + client.getQuery()).c_str()));
    ptr.push_back(strdup(std::string("SCRIPT_NAME=" + client.getURI().substr(0, client.getURI().find("?"))).c_str()));

    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
        ptr.push_back(strdup(std::string("HTTP_" + toUpper(it->first) + "=" + it->second).c_str()));
    }

    env = new char*[ptr.size() + 1];
    for (size_t i = 0; i < ptr.size(); i++) {
        env[i] = ptr[i];
        f << env[i] << std::endl;
    }
    env[ptr.size()] = NULL;

    if (env == NULL)
        perror("No env");
}

std::string CGI::start(Client &client, Data &serverData, std::string &filename) {
    int cgiInput[2];
    int cgiOutput[2];

    if (pipe(cgiInput) < 0 || pipe(cgiOutput))
        perror("pipe error");

    std::string response;
    pid_t pid = fork();
	if (pid == -1)
		perror("fork failed..");
	else if (pid == 0) {
        close(cgiInput[1]);  // Close write end of input pipe
        close(cgiOutput[0]); // Close read end of output pipe

        // Redirect standard input and output to the pipes
        dup2(cgiInput[0], STDIN_FILENO);
        dup2(cgiOutput[1], STDOUT_FILENO);

        getEnv(client);
        std::string interpreter;
        if (client.getURI().find(".phy"))
            interpreter = "/usr/bin/php";
        else if (client.getURI().find(".py"))
            interpreter = "/usr/bin/python3";

        std::string scriptName(client.getURI().substr(0, client.getURI().find("?")));
        if (scriptName.find(".") != 0)
            scriptName = "." + scriptName;

        if (client.getMethod() == "POST") {
            std::fstream f;
            char buff[4069];
            std::string requestBody;
            f.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::app); // change to FILE *file = fopen("example.bin", "rb"); // Read binary
            if (!f.is_open())
                perror("cant open request file");

            while ((f.read(buff, sizeof(buff))))
                requestBody += std::string(buff, sizeof(buff));

            if (write(STDIN_FILENO, requestBody.c_str(), requestBody.size()) == -1)
                perror("error writing request body to cgi");
        }

        char *av[] = {const_cast<char*>(interpreter.c_str()), const_cast<char*>(scriptName.c_str()), NULL};
		execve(av[0], av, env);
        perror("execve failed");
        // delete[] env;
		exit(1);
	}
    else {
        close(cgiInput[0]);
        close(cgiOutput[1]);

        char buff[4069];
        size_t bytesRead;
        struct timeval startTime;
        std::time_t time = std::time(NULL);
        startTime.tv_sec = static_cast<time_t>(time);
        while ((bytesRead = read(cgiOutput[0], buff, sizeof(buff))) > 0) {
            if(startTime.tv_sec - std::time(NULL) >= CGI_TIMEOUT)
                perror("cgi timeout..");
            response += std::string(buff, bytesRead);
        }
        // std::cout << response << std::endl;

        // Close input pipe
        close(cgiInput[1]);

        int status = 0;
	    waitpid(pid, &status, 0);

        close(cgiOutput[0]);

        if (WIFEXITED(status)) {
            // Child process exited normally
            int exitCode = WEXITSTATUS(status);
            std::cout << "Child process exited with code " << exitCode << std::endl;
        } else if (WIFSIGNALED(status)) {
            // Child process was terminated by a signal
            int signalNumber = WTERMSIG(status);
            std::cout << "Child process terminated by signal " << signalNumber << std::endl;
        }
    }
	kill(pid, SIGTERM);
    return response;
}
