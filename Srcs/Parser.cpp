#include "../Includes/Includes.hpp"

Parser::Parser(const char *filename) : fileName(filename) {}

Parser::~Parser() {}

bool Parser::checkConfigFile(Data &data) {
    if (checkExtension(fileName))
        parseFile();
    
    // blah blah

    return true;
}

bool Parser::checkExtension(std::string filename) {
    if (filename.empty() && filename.find(".conf") != std::string::npos) {
        std::cerr << "Error: Invalid conf file" << std::endl;
        return false;
    }
    return true;
}

void Parser::parseFile() {}