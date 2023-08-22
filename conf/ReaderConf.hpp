#if !defined(READERCONF_HPP)
#define READERCONF_HPP
#include "../include/webserv.hpp"
#define READER_BUFFER_SIZE 1024

typedef std::vector<std::string> filevector;

class ReaderConf
{
private:
    
public:
    ReaderConf();
    ReaderConf(const ReaderConf& other);
    virtual ~ReaderConf();
    ReaderConf & operator=(const ReaderConf& rhs);

    static filevector readfile(const char* filename);
    static filevector split(std::string str, std::string c);
    class FileNotfoundException: public std::exception
    {
        virtual const char * what() const throw();
    };

};


#endif 
