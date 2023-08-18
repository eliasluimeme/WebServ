#include "../Includes/Includes.hpp"

Response::Response() {}

Response::~Response() {}

void Response::buildResponse(Client &cl, std::string &filename, std::string &msg) {
    client = cl;
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out);

    if (file.is_open()) {
        std::cout << "Reading from response file... " << std::endl;
        std::string line;
        while (getline(file, line))
            std::cout << line << std::endl;
        file.close();
    } else {
        std::cout << "Can't open request file" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Delete file after sending request
    if (std::remove(filename.c_str())) {
        std::cout << "Error removing file" << std::endl;
        exit(EXIT_FAILURE);
    }
}