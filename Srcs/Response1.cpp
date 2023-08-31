#include "../Includes/Includes.hpp"

// text/css
// text/csv
// text/html
// text/javascript (obsolete)
// text/plain
// text/xml

Response::Response()
{
    c_Type["html"] = "text/html";
    c_Type["htm"] = "text/html";
    c_Type["shtml"] = "text/html";
    c_Type["css"] = "text/css";
    c_Type["xml"] = "text/css";
    c_Type["gif"] = "image/gif";
    c_Type["jpeg"] = "image/jpeg";
    c_Type["jpg"] = "image/jpeg";
    c_Type["png"] = "image/png";
    c_Type["js"] = "application/javascript";
    c_Type["atom"] = "application/atom+xml";
    c_Type["rss"] = "application/rss+xml";
    c_Type["mml"] = "text/mathml";
    c_Type["txt"] = "text/plain";
    c_Type["jad"] = "text/vnd.sun.j2me.app-descriptor";
    c_Type["wml"] = "text/vnd.wap.wml";
    c_Type["htc"] = "text/x-component";
    c_Type["png"] = "image/png";
    c_Type["tif"] = "image/tiff";
    c_Type["tiff"] = "image/tiff";
    c_Type["wbmp"] = "image/vnd.wap.wbmp";
    c_Type["ico"] = "image/x-icon";
    c_Type["jng"] = "image/x-jng";
    c_Type["bmp"] = "image/x-ms-bmp";
    c_Type["svg"] = "image/svg+xml";
    c_Type["svgz"] = "image/svg+xml";
    c_Type["webp"] = "image/webp";
    c_Type["woff"] = "application/font-woff";
    c_Type["jar"] = "application/java-archive";
    c_Type["war"] = "application/java-archive";
    c_Type["ear"] = "application/java-archive";
    c_Type["json"] = "application/json";
    c_Type["hqx"] = "application/mac-binhex40";
    c_Type["doc"] = "application/msword";
    c_Type["pdf"] = "application/pdf";
    c_Type["ps"] = "application/postscript";
    c_Type["eps"] = "application/postscript";
    c_Type["ai"] = "application/postscript";
    c_Type["rtf"] = "application/rtf";
    c_Type["m3u8"] = "application/vnd.apple.mpegurl";
    c_Type["xls"] = "application/vnd.ms-excel";
    c_Type["eot"] = "application/vnd.ms-fontobject";
    c_Type["ppt"] = "application/vnd.ms-powerpoint";
    c_Type["wmlc"] = "application/vnd.wap.wmlc";
    c_Type["kml"] = "application/vnd.google-earth.kml+xml";
    c_Type["kmz"] = "application/vnd.google-earth.kmz";
    c_Type["7z"] = "application/x-7z-compressed";
    c_Type["cco"] = "application/x-cocoa";
    c_Type["jardiff"] = "application/x-java-archive-diff";
    c_Type["jnlp"] = "application/x-java-jnlp-file";
    c_Type["run"] = "application/x-makeself";
    c_Type["pl"] = "application/x-perl";
    c_Type["pm"] = "application/x-perl";
    c_Type["prc"] = "application/x-pilot";
    c_Type["pdb"] = "application/x-pilot";
    c_Type["rar"] = "application/x-rar-compressed";
    c_Type["rpm"] = "application/x-redhat-package-manager";
    c_Type["sea"] = "application/x-sea";
    c_Type["swf"] = "application/x-shockwave-flash";
    c_Type["sit"] = "application/x-stuffit";
    c_Type["tcl"] = "application/x-tcl";
    c_Type["tk"] = "application/x-tcl";
    c_Type["der"] = "application/x-x509-ca-cert"; 
    c_Type["pem"] = "application/x-x509-ca-cert";
    c_Type["cert"] = "application/x-x509-ca-cert";
    c_Type["xpi"] = "application/x-xpinstall";
    c_Type["xhtml"] = "application/xhtml+xml";
    c_Type["xspf"] = "application/xspf+xml";
    c_Type["zip"] = "application/zip";
    c_Type["bin"] = "application/octet-stream";
    c_Type["exe"] = "application/octet-stream";
    c_Type["dll"] = "application/octet-stream";
    c_Type["deb"] = "application/octet-stream";
    c_Type["dmg"] = "application/octet-stream";
    c_Type["iso"] = "application/octet-stream";
    c_Type["img"] = "application/octet-stream";
    c_Type["msi"] = "application/octet-stream";
    c_Type["msp"] = "application/octet-stream";
    c_Type["msm"] = "application/octet-stream";
    c_Type["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    c_Type["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    c_Type["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    c_Type["mid"] = "audio/midi";
    c_Type["midi"] = "audio/midi";
    c_Type["kar"] = "audio/midi";
    c_Type["mp3"] = "audio/mpeg";
    c_Type["ogg"] = "audio/ogg";
    c_Type["m4a"] = "audio/x-m4a";
    c_Type["ra"] = "audio/x-realaudio";
    c_Type["3gpp"] = "video/3gpp";
    c_Type["3gp"] = "video/3gpp";
    c_Type["ts"] = "video/mp2t";
    c_Type["mp4"] = "video/mp4";
    c_Type["mpeg"] = "video/mpeg";
    c_Type["mpg"] = "video/mpeg";
    c_Type["mov"] = "video/quicktime";
    c_Type["webm"] = "video/webm";
    c_Type["flv"] = "video/x-flv";
    c_Type["m4v"] = "video/x-m4v";
    c_Type["mng"] = "video/x-mng";
    c_Type["asx"] = "video/x-ms-asf";
    c_Type["asf"] = "video/x-ms-asf";
    c_Type["wmv"] = "video/x-ms-wmv";
    c_Type["avi"] = "video/x-msvideo";
}

Response::~Response() {}

// bool Response::sendResponse(int clientFd, std::string &responseMsg) {
//     size_t bytesSent;
//     bytesSent += send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);

//     if (bytesSent >= responseMsg.size())
//         return true;
//     else return false;
// }

// void Response::buildResponse(Client &cl, Data &serverData, std::string &filename) {
//     client = cl;
//     CGI cgi;
//     std::fstream file;
//     file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);

//     std::string uri = client.getURI();
//     std::string cgiLocation = serverData.locations["/cgi-bin"].root;
//     std::cout << "cgi location " << cgiLocation << std::endl;

//     if (uri.find("/cgi-bin") >= 0)
//         cgi.start(client, serverData, filename);

//     if (file.is_open()) {
//         std::string line;
//         // while (getline(file, line))
//         //     std::cout << line << std::endl;
//         file.close();
//     } else {
//         std::cout << "Can't open request file.." << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     std::string responseMsg;
//     // sendResponse(client.getFd(), responseMsg);

//     // Delete file after sending request
//     // if (std::remove(filename.c_str())) {
//     //     std::cout << "Error removing file" << std::endl;
//     //     exit(EXIT_FAILURE);
//     // }
// }

// bool ft_error(int status, Client &client) {
//     std::cout << "Error in request sent. Cleaning.." << std::endl;
//     std::map<int, std::string> error;
//     error[501] = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length: 72\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>501 Not Implemented </h1></center></body></html>\r\n";
//     error[400] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 68\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>400 Bad Request </h1></center></body></html>\r\n";
//     error[408] = "HTTP/1.1 408 Request Timeout\r\nContent-Type: text/html\r\nContent-Length: 72\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>408 Request Timeout </h1></center></body></html>\r\n";
//     error[414] = "HTTP/1.1 414 Request-URI Too Long\r\nContent-Type: text/html\r\nContent-Length: 77\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>414 Request-URI Too Long </h1></center></body></html>\r\n";
//     error[413] = "HTTP/1.1 413 Request Entity Too Large\r\nContent-Type: text/html\r\nContent-Length: 81\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>413 Request Entity Too Large </h1></center></body></html>\r\n";
//     error[404] = "HTTP/1.1 404 NOT FOND\r\nContent-Type: text/html\r\nContent-Length: 66\r\nConnection: keep-alive\r\n\r\n<html><body><center><h1>404 NOT FOUND </h1></center></body></html>\r\n";
//     write(client.getFd(), error[status].c_str(), strlen(error[status].c_str()));

//     reset(client);
//     return false;
// }

bool Response::sendResponse(int clientFd, std::string &responseMsg)
{
    size_t bytesSent;
    bytesSent += send(clientFd, responseMsg.c_str(), responseMsg.size(), 0);

    if (bytesSent >= responseMsg.size())
        return true;
    else
        return false;
}

void Response::getLocationMatch(std::string path, std::map<std::string, Data> locations, std::string &location_key)
{
    size_t match = 0;

    for (std::map<std::string, Data>::const_iterator it = locations.begin(); it != locations.end(); ++it)
    {
        if (path.find(it->first) == 0)
        {
            if (it->first == "/" || path.length() == it->first.length() || path[it->first.length()] == '/')
            {
                if (it->first.length() > match)
                {
                    match = it->first.length();
                    location_key = it->first;
                }
            }
        }
    }
}

bool Response::buildResponse(Client &client, Data &serverData, std::string &filename)
{
    Request rq;
    CGI cgi;
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);

    // std::string uri = client.getURI();
    // std::string cgiLocation = serverData.locations["/cgi-bin"].root;
    // std::cout << "cgi location " << cgiLocation << std::endl;
    // if (uri.find("/cgi-bin"))
    //     cgi.start(client, serverData, filename);

    std::string location_key = "";
    getLocationMatch(client.getURI(), serverData.locations, location_key);
    if (location_key == "")
        rq.ft_error(404, client);
    // std::map<std::string, Data>::iterator it = serverData.locations.find(location_key);

    std::cout << "Location : " << location_key << std::endl;
    std::cout << "Method : " << client.getMethod() << std::endl;
    if (client.getMethod().compare("GET") == 0)
        return GetMethod(client, serverData, location_key);
    // if (file.is_open()) {
    //     std::string line;
    //     // while (getline(file, line))
    //     //     std::cout << line << std::endl;
    //     file.close();
    // } else {
    //     std::cout << "Can't open request file.." << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // std::string responseMsg;
    // sendResponse(client.getFd(), responseMsg);

    // Delete file after sending request
    // if (std::remove(filename.c_str())) {
    //     std::cout << "Error removing file" << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    return false;
}

std::string find_filename(std::string path)
{
    size_t pos = path.find_last_of("/");
    std::string name = path.substr(pos + 1);
    return name;
}

void Response::sendchunked(int clientSocket, int offset, int portionSize)
{
    std::cout << "L : " << loc.c_str() << std::endl;
    std::ifstream file(loc.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error " << std::endl;
        return;
    }
    file.seekg(offset);
    char buffer[portionSize];
    file.read(buffer, portionSize);
    send(clientSocket, buffer, portionSize, 0);
    file.close();
}

bool Response::get_file(std::ifstream &a, Client &client, std::map<std::string, std::string> c_Type, std::string p)
{
    std::string filename = find_filename(p.c_str());
    const char *s = std::strrchr(filename.c_str(), '.');
    std::string extentions(s + 1);
    if(extentions.compare(".py") == 0 || extentions.compare(".php") == 0)
    {
        std::cout << "Extentions : " << extentions << std::endl; // CGI
        // exit(1);
    }
    else
    {
        if (client.headerSent == false)
        {
            std::cout << "File Name : \n"
                      << filename << std::endl;
            a.seekg(0, std::ios::end);
            client.fileSize = a.tellg();
            a.seekg(0, std::ios::end);
            std::ostringstream responses;
            responses << "HTTP/1.1 200 OK\r\n";
            responses << "Content-Type: " << c_Type[extentions] << "\r\n";
            responses << "Content-Length: " << client.fileSize << "\r\n";
            responses << "\r\n";
            send(client.getFd(), responses.str().c_str(), responses.str().size(), 0);
            client.headerSent = true;
            std::cout << "hello\n";
        }
        else
        {
            std::cout << "fileSize " << client.fileSize << std::endl;
            int portionSize = 1024 * 1024;
            if (client.offset < client.fileSize)
            {
                sendchunked(client.getFd(), client.offset, portionSize);
                client.offset += portionSize;
                std::cout << "Offset " << client.offset << std::endl;
            }
            else
            {
                std::cout << "BYEEEE\n";
                return true;
            }
        }
    }
    return 0;
}

void Response::auto_index(Client &client, std::string &root)
{
    DIR* dir = opendir(loc.c_str());
    std::string directories;
    struct dirent *dent;
    if(dir)
    {
        while((dent = readdir(dir)) != NULL)
        {
            if (root[root.length() -1] != '/')
                root += '/';
            if(!std::strcmp(dent->d_name, ".") || (!std::strcmp(dent->d_name, "..") && !std::strcmp(loc.c_str(), root.c_str())))
                continue;
            if(dent->d_type == DT_DIR)
            {
                directories = dent->d_name;
                directories += '/';
                auto_index_file.push_back(directories);
            }
            else
            {
                // std::cout << "File name : " << dent->d_name << std::endl;
                // exit(1);
                auto_index_file.push_back(dent->d_name);
            }
        }
        closedir(dir);
        std::vector<std::string>::iterator it;
        // std::string
        res_auto_index = "<html><body><h1>Index File</h1>";
        for(it = auto_index_file.begin(); it < auto_index_file.end(); it++)
        {
            res_auto_index += "<a href = \"" + *it + "\">" + *it + "</a><br>";
        }
        res_auto_index += "</body></html>";
        std::ostringstream re;
        re << "HTTP/1.1 200 OK\r\n";
        re << "Content-Type: text/html\r\n";
        re << "Content-Length: " << res_auto_index.length() << "\r\n";
        re << "\r\n";
        re << res_auto_index;
        send(client.getFd(), re.str().c_str(), re.str().length(), 0);
    }
    else
    {
        rq.ft_error(404, client);
        // exit(1);
    }
}

bool Response::get_directory(Client &c, bool &autoin, std::vector<std::string> &index, std::string &root)
{
    std::string dir = c.getURI();
    Request r;

    if (dir[dir.size() - 1] != '/')
    {
        std::string resp = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + dir + "/\r\n\r\n";
        send(c.getFd(), resp.c_str(), resp.size(), 0);
    }
    if (!index.empty())
    {
        root += "/" + index.front();
        loc = root;
        std::cout << "index file : " << root << std::endl;
        std::ifstream f(root.c_str());
        if (!f.is_open())
        {
            r.ft_error(404, c);
            // exit(1);
        }
        if (get_file(f, c, c_Type, root))
            return true;
    }
    else
    {
        std::cout << "autoindex : " << autoin << std::endl;
        if (autoin == false)
        {
            r.ft_error(403, c); // error Forbidden
            // exit (1);
        }
        else
        {
            std::cout << "hello from autoindex\n";
            // exit(1);
            auto_index(c, root);
        }
    }
    // exit(1);
    return false;
}

bool Response::GetMethod(Client &client, Data &serverdata, std::string location)
{
    // int fileSize;
    Request req;
    std::streampos ab;
    bool autoin;
    std::string root;
    std::vector<std::string> index;
    struct stat s;

    std::map<std::string, Data>::iterator it = serverdata.locations.find(location);
    if (it != serverdata.locations.end())
    {
        Data &data = it->second;
        root = data.root;
        autoin = data.autoIndex;
        index = data.index;
        std::cout << "ROOt : " << data.root << std::endl;
        std::string file = client.getURI().substr(location.length(), client.getURI().length());
        loc = data.root + file;
    }
    std::cout << "ROOT LO : " << loc << std::endl;
    std::ifstream uri_file(loc.c_str());
    if (!uri_file.is_open())
    {
        req.ft_error(404, client);
        // exit(1);
    }

    if (stat(loc.c_str(), &s) != 0)
    {
        std::cout << "ERROR with STAT function" << std::endl;
        uri_file.close();
        exit(1);
    }
    if (S_ISREG(s.st_mode) && (s.st_mode && S_IRUSR))
    {
        std::cout << "BOOL : " << client.headerSent << std::endl;

        if (get_file(uri_file, client, c_Type, client.getURI()))
            return true;
    }
    else if (S_ISDIR(s.st_mode))
    {
        autoin = true;
        std::cout << "URI :" << client.getURI() << std::endl;
        if (get_directory(client, autoin, index, root))
            return true;
    }
    return false;
}