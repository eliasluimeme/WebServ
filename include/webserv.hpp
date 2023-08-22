#if !defined(WEBSERV_HPP)
#define WEBSERV_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdio>
#include <sstream>

//cpp containers
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
#include <list>
#include <utility>

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <time.h>
# include <limits.h>
# include <errno.h>
# include <dirent.h>

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define RESET "\033[0m"


# define RECV_SIZE 65536
# define CGI_BUFSIZE 65536
# define DEFAULT_CONFIG "../config/files/default.conf"

bool compare_langs(const std::pair<std::string, float> first, const std::pair<std::string, float> second);
std::string& strip(std::string &str, char c);
std::string readValue(std::string &line);
std::string readKey(std::string &line);
std::vector<std::string>		split(const std::string& str, char c);
std::string&					pop(std::string& str);
std::string&					to_upper(std::string& str);
std::string&					capitalize(std::string& str);
int		pathIsFile(const std::string& path);
#endif // 

 