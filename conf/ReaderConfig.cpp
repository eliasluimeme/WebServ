
#include "ReaderConf.hpp"

ReaderConf::ReaderConf(void)
{

}

ReaderConf::~ReaderConf()
{

}
ReaderConf::ReaderConf(const ReaderConf& other)
{
    *this = other;
}

ReaderConf & ReaderConf::operator=(const ReaderConf& rhs)
{
    if (this != &rhs)
    {
        *this = rhs;
    }
    return *this;
} 
filevector ReaderConf::split(std::string str, std::string c)
{
    filevector tokens;
    str += c[0];
    std::string::size_type start = str.find_first_not_of(c, 0);
    std::string::size_type end = 0;
    while (1)
    {
        end = str.find_first_of(c, start);
        if (end == std::string::npos)
            break;
        std::string s = str.substr(start, end - start);
        tokens.push_back(s);
        if ((start = str.find_first_not_of(c, end)) == std::string::npos)
            break;
    }
    return tokens;
}

filevector ReaderConf::readfile(const char * filename)
{
    int  ret = READER_BUFFER_SIZE;
    char buffer[READER_BUFFER_SIZE + 1];
    std::string line = "";
    int fd;
    filevector file;

    for (size_t i = 0; i < READER_BUFFER_SIZE + 1; i++)
        buffer[i] = '\0';
    if ((fd = open(filename, O_RDONLY)) <= 0)
        throw ReaderConf::FileNotfoundException();
    for (ret = READER_BUFFER_SIZE; ret > 0; ret = read(fd, buffer, READER_BUFFER_SIZE))
    {
        buffer[ret] = '\0';
        line += buffer;
    }
    if (ret == -1)
    {
        std::cerr << RED << "error while reading config file" << std::endl;
        return file;
    }
    file = ReaderConf::split(line, std::string(" \n\t"));
    return file;
}

const char	*ReaderConf::FileNotfoundException::what() const throw(){
	return "Exception thrown: could not open configuration file";
}
