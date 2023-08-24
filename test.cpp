#include <iostream>
#include <fstream>
#include <string>

int main(int ac, char **av) {
    std::ifstream inputFile(av[1]); 

    if (inputFile.is_open()) { 
        std::string fileContents((std::istreambuf_iterator<char>(inputFile)),
                                 (std::istreambuf_iterator<char>())); 
        inputFile.close();

        std::cout << "File contents: " << std::endl;
        std::cout << fileContents << std::endl; // Print the contents
    } else {
        std::cout << "Failed to open the file." << std::endl;
    }

    return 0;
}
