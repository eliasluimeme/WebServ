#include <iostream>
#include "Request.hpp"

void lang()
{
    std::vector<std::string> token;
    std::string header;
    std::vector<std::string>::iterator it;
    size_t i;
    std::map<std::string, std::string> _header;
    std::list<std::pair<std::string, float> > _lang;
    header = _header["Accept-Language"] = "fr-CH, fr;q=0.9, en;q=0.8, de;q=0.7, *;q=0.5\r";
    
    if ((header = _header["Accept-Language"]) != "")
    {
        float weight = 0.0;
        token = split(header, ',');
        for (it = token.begin(); it != token.end(); it++)
        {
            std::string lang;
            lang = (*it).substr(0, (*it).find_first_of('-'));
            strip(lang, ' ');
            if ((i = lang.find_last_of(';')) != std::string::npos)
                weight = atof((*it).substr(i + 4).c_str());
            lang.resize(i > 2 ? 2 : i);
            _lang.push_back(std::pair<std::string, float>(lang, weight));
        }
        _lang.sort(compare_langs);
        for (const auto& pair : _lang)
        {
            std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
        }
    }
}

int main()
{
    lang();
    
    return 0;
}

