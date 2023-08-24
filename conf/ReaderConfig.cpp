
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
    filevector file;
    std::ifstream inputFile(filename); 

    if (!(inputFile.is_open()))
        throw ReaderConf::FileNotfoundException();
    std::string fileContents((std::istreambuf_iterator<char>(inputFile)),
                                 (std::istreambuf_iterator<char>()));
    inputFile.close();

    
    file = ReaderConf::split(fileContents, std::string(" \n\t"));
    return file;
}

const char	*ReaderConf::FileNotfoundException::what() const throw()
{
	return "Exception thrown: could not open configuration file";
}
