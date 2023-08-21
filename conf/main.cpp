
#include "config.hpp"

int main(int argc, char const *argv[])
{
    if (argc == 2)
    {
       Config conf;
       try
       {
            conf.parse(argv[1]);
       }
       catch(const std::exception& e)
       {
        std::cerr << e.what() << '\n';
       }
    }
    else 
    {
        std::cerr << "invalid number of arguments. " << RED << std::endl;
        return 1;
    }
    
    return 0;
}
