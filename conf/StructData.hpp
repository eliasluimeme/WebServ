#pragma once

#define STRUCTDATA_HPP

#include "../Includes/Includes.hpp"

# define parseMap std::map<std::string, void (ConfigServer::*)(filevector)>

class ConfigServer;


typedef struct	s_listen {
	std::string	host;
	std::string	port;
}				t_listen;
