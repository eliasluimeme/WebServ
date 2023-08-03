#pragma once

#include "../Includes/Includes.hpp"

class ConfData;

// port servername locations
class Parser {
    public:
    Parser(const char *filename);
    ~Parser();
    bool checkConfigFile(Data &data);
    void parseFile();
    bool checkExtension(std::string filename);

    private:
    std::string             configFile;
    std::string             port;
	std::string				host;
	std::string             server_name;
	std::string				upload;
	ssize_t					body_size;
	// std::vector<>           locations;
	std::map<std::string, std::string>	errorPages; 
};