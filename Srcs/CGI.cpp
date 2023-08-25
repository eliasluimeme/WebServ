#include "../Includes/Includes.hpp"

CGI::CGI() {}

CGI::~CGI() {}

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

		// execve(av[0], av, NULL);
		exit(1);
	}
    else {
        close(cgiInput[0]);
        close(cgiOutput[1]);

        char buff[4069];
        size_t bytesRead;
        std::string response;
        std::time_t time = std::time(NULL);
        struct timeval currentTime;
        currentTime.tv_sec = static_cast<time_t>(time);
        if()
        while((bytesRead = read(cgiOutput[0], buff, sizeof(buff)) > 0))
            response += std::string(buff, bytesRead);

	    waitpid(pid, nullptr, 0);
    }
	kill(pid, SIGTERM);
}
