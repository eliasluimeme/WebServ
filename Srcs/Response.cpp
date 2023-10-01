#include "../Includes/Includes.hpp"

// text/css
// text/csv
// text/html
// text/javascript (obsolete)
// text/plain
// text/xml

Response::Response() : state(IN_PROGRESS), statee(0), chunked(false)
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

void Response::exitWithError(std::string msg) {
    std::cout << "[-] Error: " << msg << std::endl;
    exit(EXIT_FAILURE);
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

bool  Response::isallowdMethod(std::string method, Data &location)
{
    for (std::set<std::string>::iterator i = location.methods.begin(); i != location.methods.end(); i++)
    {
        if (i->find(method) != std::string::npos)
            return true;
    }
    return false;
}

bool Response::buildResponse(Client &client, Data &serverData, std::string &filename)
{
    Request rq;
    CGI cgi;
    std::fstream file;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);

    std::string location_key = "";
    getLocationMatch(client.getURI(), serverData.locations, location_key);
    if (location_key == "")
    {
        rq.ft_error(404, client, " NOT FOUND");
        state = SENT;
        return false;
    }
    if(!(isallowdMethod(client.getMethod(), serverData.locations[location_key])))
    {
        rq.ft_error(405, client, " Method Not Allowed");
        state = SENT;
        return false;
    }

    if (client.getMethod().compare("GET") == 0)
    {
        if(GetMethod(client, serverData, location_key))
            return true;
    }
    if (client.getMethod().compare("POST") == 0)
    {
        if(PostMethod(client, serverData, location_key))
            return true;
    }
    if (client.getMethod().compare("DELETE") == 0)
    {
        std::cout << "hello from delete\n";
        if(DeleteMethod(client, serverData, location_key))
            return true;
    }

    // Delete file after sending request
    // if (std::remove(filename.c_str())) {
    //     std::cout << "Error removing file" << std::endl;
    //     // exit(EXIT_FAILURE);
    // }
    return false;
}

std::string find_filename(std::string path)
{
    size_t pos = path.find_last_of("/");
    std::string name = path.substr(pos + 1);
    return name;
}

void Response::sendchunked(Client &client, int offset, int portionSize, std::string header)
{
    std::ifstream file(loc.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening requested file" << std::endl;
        return;
    }
    file.seekg(offset);
    char buffer[portionSize];
    file.read(buffer, portionSize);
    std::string resp(buffer, portionSize);
    resp = header + resp;

    if (send(client.getFd(), resp.c_str(), resp.size(), 0) <= 0 ) {
        state = DROP_CLIENT;
    }
    file.close();
}

bool Response::get_file(std::ifstream &a, Client &client, Data &serverData, std::map<std::string, std::string> c_Type, std::string p)
{
    std::string filename = find_filename(p.c_str());
    if (filename.find("?"))
        filename = filename.substr(0, filename.find("?"));

    const char *s = std::strrchr(filename.c_str(), '.');
    std::string extentions(s);
    std::string extentions1(s + 1);

    CGI cgi;
    Request req;
    std::string cgiMsg;

    if (extentions.compare(".py") == 0 || extentions.compare(".php") == 0)
    {
        if (serverData.locations.find(client.cgiLocation) != serverData.locations.end()) {
            for (std::vector<std::string>::iterator it = serverData.locations[client.cgiLocation].cgi.begin(); it != serverData.locations[client.cgiLocation].cgi.end(); it++) {
                std::string ex = "." + *it;
                if (ex == extentions) {
                    cgiMsg = cgi.start(client, loc);
                    if(send(client.getFd(), cgiMsg.c_str(), cgiMsg.size(), 0) <= 0) {
                        state = DROP_CLIENT;
                        return false;
                    }
                    else state = SENT;
                }
                else if (it == serverData.locations[client.cgiLocation].cgi.end()) {
                    req.ft_error(501, client, " Not Implemented");
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
            std::ostringstream responses;
        if (client.headerSent == false)
        {
            a.seekg(0, std::ios::end);
            client.fileSize = a.tellg();
            responses << "HTTP/1.1 200 OK\r\n";
            responses << "Content-Type: " << c_Type[extentions1] << "\r\n";
            responses << "Content-Length: " << client.fileSize << "\r\n";
            responses << "\r\n";     
            client.headerSent = true;
            state = IN_PROGRESS;
        }
        if (client.headerSent)
        {
            int portionSize = 1024 * 512;
            if (client.offset < client.fileSize)
            {
                sendchunked(client, client.offset, portionSize, responses.str()); 
                client.offset += portionSize;
            }
            if (state == DROP_CLIENT) 
                return false;
            if (client.offset >= client.fileSize)
            {
                state = SENT;
                return true;
            }
        }
    }
    return false;
}

bool Response::auto_index(Client &client, std::string &root)
{
    DIR *dir = opendir(loc.c_str());
    std::string directories;
    struct dirent *dent;
    if (dir)
    {
        while ((dent = readdir(dir)) != NULL)
        {
            if (root[root.length() - 1] != '/')
                root += '/';
            if (!std::strcmp(dent->d_name, ".") || (!std::strcmp(dent->d_name, "..") && !std::strcmp(loc.c_str(), root.c_str())))
                continue;
            if (dent->d_type == DT_DIR)
            {
                directories = dent->d_name;
                directories += '/';
                auto_index_file.push_back(directories);
            }
            else
            {
                auto_index_file.push_back(dent->d_name); 
            }
        }
        closedir(dir);
        std::vector<std::string>::iterator it;
        res_auto_index = "<html><body><h1>Index File</h1>";
        for (it = auto_index_file.begin(); it < auto_index_file.end(); it++)
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
        if (send(client.getFd(), re.str().c_str(), re.str().length(), 0) <= 0) {
            state = DROP_CLIENT;

        }
        else state = SENT;
        return true;
    }
    else
    {
        rq.ft_error(404, client, " NOT FOUND");
        state = SENT;
        return false;
    }
    return false;
}

bool Response::get_directory(Client &c, Data &serverdata, bool &autoin, std::vector<std::string> &index, std::string &root)
{
    std::string dir = c.getURI();
    Request r;

    if (dir[dir.size() - 1] != '/')
    {
        std::string resp = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + dir + "/\r\n\r\n";
        if (send(c.getFd(), resp.c_str(), resp.size(), 0) <= 0) {
            state = DROP_CLIENT;
            return false;
        }
        else state = SENT;
    }
    if (!index.empty())
    {
        loc += index.front();
        std::ifstream f(loc.c_str());
        std::cout << loc << std::endl;
        if (!f.is_open())
        {
            r.ft_error(404, c, " NOT FOUND");
            state = SENT;
        }
        if (get_file(f, c, serverdata, c_Type, loc))
            return true;
    }
    else
    {
        if (autoin == false)
        {
            r.ft_error(403, c, " FORBIDDEN");
            state = SENT;
        }
        else
        {
            std::cout << "Hello from autoindex\n";
            return auto_index(c, root);
        }
    }
    return false;
}

bool Response::GetMethod(Client &client, Data &serverdata, std::string location)
{
    std::streampos ab;
    bool autoin;
    std::string root;
    std::vector<std::string> index;
    struct stat s;

    std::map<std::string, Data>::iterator it = serverdata.locations.find(location);
    client.cgiLocation = location;
    if (it != serverdata.locations.end())
    {
        Data &data = it->second;
        root = data.root;
        if (root[root.length() - 1] != '/')
            root += "/";
        autoin = data.autoIndex;
        index = data.index;
        std::string file = client.getURI().substr(location.length(), client.getURI().substr(location.length()).find_last_of("?")); 
        loc = root + file;
    }
    if (stat(loc.c_str(), &s) != 0)
    {
        std::cout << "ERROR with STAT function" << std::endl;
        rq.ft_error(404, client, " NOT FOUND");
        state = SENT;
        return false;
    }
    if (S_ISREG(s.st_mode) && (s.st_mode && S_IRUSR))
    {
         if(access(loc.c_str(), R_OK) != 0)
        {
            rq.ft_error(403, client, " FORBIDDEN");
            state = SENT;
            return false;
        }
        std::ifstream uri_file(loc.c_str());
        if (get_file(uri_file, client, serverdata, c_Type, client.getURI()))
            return true;
    }
    else if (S_ISDIR(s.st_mode))
    {
        if(access(loc.c_str(), F_OK | R_OK) != 0)
        {
            std::cout << "access1\n";
            rq.ft_error(403, client, " FORBIDDEN");
            state = SENT;
            return false;
        }
        if (get_directory(client, serverdata, autoin, index, root))
            return true;
    }
    else
    {
        rq.ft_error(404, client, " NOT FOUND");
        state = SENT;
        return false;
    }
    return false;
}

bool Response::get_file_post(Client &cl)
{
    std::string filename = find_filename(loc.c_str());
    std::cout << "filename : " << filename << std::endl;
    const char *s = std::strrchr(filename.c_str(), '.');
    std::string extentions(s + 1);
    std::string extention(s);
    Data serverData = cl.getConfData();
    std::string cgiMsg;
    CGI cgi;

    if (extention.compare(".py") == 0 || extention.compare(".php") == 0)
    {
        if (serverData.locations.find("/cgi-bin") != serverData.locations.end())
        {
            for (std::vector<std::string>::iterator it = serverData.locations["/cgi-bin"].cgi.begin(); it != serverData.locations["/cgi-bin"].cgi.end(); it++)
            {
                std::string ex = "." + *it;
                if (ex == extention)
                {
                    std::cout << "extension " << ex << std::endl;
                    cgiMsg = cgi.start(cl, loc);
                    if (send(cl.getFd(), cgiMsg.c_str(), cgiMsg.size(), 0) <= 0) {
                        state = DROP_CLIENT;
                        return false;
                    }
                    else state = SENT;
                }
                else if (it == serverData.locations[cl.cgiLocation].cgi.end()) {
                    rq.ft_error(501, cl, " Not Implemented");
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
        rq.ft_error(403, cl, " FORBIDDEN");
        state = SENT;
        return true;
    }
    return false;
}

bool Response::PostMethod(Client &cl, Data &serverdata, std::string &location)
{
    std::string root;
    std::vector<std::string> index;
    // check if location has path_upload
    std::map<std::string, Data>::iterator it = serverdata.locations.find(location);
    if (it != serverdata.locations.end())
    {
        Data &data = it->second;
        if (!data.uploadPass.empty())
        {
            if(access(data.uploadPass.c_str(), R_OK) != 0)
            {
                rq.ft_error(403, cl, " FORBIDDEN");
                state = SENT;
                return false;
            }
            std::ostringstream p;
            p << "./request-";
            p << cl.getFd();
            std::string value;
            for (std::map<std::string, std::string>::iterator it = c_Type.begin(); it != c_Type.end(); it++)
            {
                if (it->second == cl.getHeaders()["Content-Type"])
                    value = it->first;
            }
            int c = clock();
            std::string roo = data.root;
            std::ostringstream f1;
            f1 << data.uploadPass;
            f1 << "file";
            f1 << c;
            f1 << ".";
            f1 << value;
            int i = rename(p.str().c_str(), f1.str().c_str()); //// rename with the path to destination
            if (i == 0)
            {
                std::cout << "--------    Posted    --------\n";
                cl.rename = true;
                state = SENT;
                std::stringstream resp;
                resp << "HTTP/1.1 201 CREATED\r\n";
                resp << "Content-Type: text/html\r\nContent-Lenght: 118\r\n\r\n";
                resp << "<html>\r\n<body>\r\n<h1>Operation Successful</h1>\r\n<p>Your request has been processed.</p>\r\n</body>\r\n</html>\r\n";
                if (send(cl.getFd(), resp.str().c_str(), resp.str().size(), 0) <= 0)
                    state = DROP_CLIENT;
                return true;
            }
        }
        root = data.root;
        index = data.index;
        if (root[root.length() - 1] != '/')
            root += '/';
        index = data.index;
        std::string file = cl.getURI().substr(location.length(), cl.getURI().length());
        loc = root + file;
        cl.cgiLocation = loc;
    }
    // check request ressource if is there
    if(access(loc.c_str(), R_OK) != 0)
    {
        rq.ft_error(403, cl, " FORBIDDEN");
        state = SENT;
        return false;
    }
    struct stat s;
    if (stat(loc.c_str(), &s) != 0)
    {
        std::cout << "ERROR with STAT function" << std::endl;
        rq.ft_error(404, cl, " NOT FOUND");
        state = SENT;
        return false;
    }
    if (S_ISREG(s.st_mode) && (s.st_mode && S_IRUSR))
    {
        if (get_file_post(cl))
            return true;
    }
    else if (S_ISDIR(s.st_mode))
    {
        std::string dir = cl.getURI();
        if (dir[dir.size() - 1] != '/')
        {
            std::string resp = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + dir + "/\r\n\r\n";
            if (send(cl.getFd(), resp.c_str(), resp.size(), 0) <= 0) {
                state = DROP_CLIENT;
                return false;
            }
            state = SENT;
        }
        else
        {
            if (!index.empty())
            {
                loc += index.front();
                if (get_file_post(cl))
                    return true;
            }
            else
            {
                rq.ft_error(403, cl, " FORBIDDEN");
                state = SENT;
                return false;
            }
        }
    }
    return false;
}

// Delete

bool Response::remouveDirectories(Client &c, std::string  path)
{
    if(path[path.size() - 1] != '/')
        path += '/';
    struct dirent *d;
    DIR *dir = opendir(path.c_str());
    std::string directories;
    if(dir)
    {
        while((d = readdir(dir)) != NULL)
        {
            if(!std::strcmp(d->d_name, ".") || !std::strcmp(d->d_name, ".."))
                continue;
            if(d->d_type == DT_REG)
            {
                std::string name = path + d->d_name ;
                std::cout << name << std::endl;
                int i = access(name.c_str(), R_OK | W_OK);
                if(i == -1)
                {
                    rq.ft_error(403, c, " FORBIDDEN");
                    state = SENT;
                    return false;
                }
                else
                {
                    if(remove(name.c_str()) == 0)
                        std::cout << "file remouved\n";
                    else
                    {
                        rq.ft_error(403, c, " FORBIDDEN");
                        state = SENT;
                        return false;
                    }
                }
            }
            else if(d->d_type == DT_DIR)
            {
                std::cout << "folder name : " <<  path + d->d_name << "/\n";
                remouveDirectories(c, path + d->d_name + "/");
            }
            std::string n = path + d->d_name;
            if(remove(n.c_str()) == 0)
                std::cout << "folder remouved\n";
            else
                std::cout << "folder not remouved\n";
        }
    }
    return true;
}

bool Response::DeleteMethod(Client &client, Data &serverData, std::string &location)
{
    std::string root;
    std::vector<std::string> index;
    std::vector<std::string> cgi;
    std::map<std::string, Data>::iterator it = serverData.locations.find(location);
    if (it != serverData.locations.end())
    {
        Data &data = it->second;
        root = data.root;
        if (root[root.length() - 1] != '/')
            root += '/';
        index = data.index;
        cgi = data.cgi;
        std::string file = client.getURI().substr(location.length(), client.getURI().length());
        loc = root + file;
    }
    struct stat s;
    if (stat(loc.c_str(), &s) != 0)
    {
        std::cout << "ERROR with STAT function" << std::endl;
        rq.ft_error(404, client, " NOT FOUND");
        state = SENT;
        return false;
    }
    if (S_ISREG(s.st_mode))
    {
        if(access(loc.c_str(), F_OK | R_OK) != 0)
        {
            rq.ft_error(403, client, " FORBIDDEN");
            state = SENT;
            return false;
        }
        std::string res = "HTTP/1.1 204 No Content \r\n\r\n";
        if(remove(loc.c_str()) == 0)
        {
            std::cout << "file remouved\n";
            if (send(client.getFd(), res.c_str(), res.length(), 0) <= 0)
                state = DROP_CLIENT;
            else state = SENT;
            return true;
        }
        else
        {
            rq.ft_error(404, client, " NOT FOUND");
            state = SENT;
            return false;
        }
    }
    else if(S_ISDIR(s.st_mode))
    {
        if(access(loc.c_str(), R_OK) != 0)
        {
            rq.ft_error(403, client, " FORBIDDEN");
            state = SENT;
            return false;
        }
        else
        {
            if(remouveDirectories(client, loc))
            {
                if(remove(loc.c_str()) == 0)
                    std::cout << "folder remouved\n";
                std::string res = "HTTP/1.1 204 No Content \r\n\r\n";
                if (send(client.getFd(), res.c_str(), res.length(), 0) <= 0)
                    state = DROP_CLIENT;
                else state = SENT;
                return true;
            }
        }
    }
    else
    {
        rq.ft_error(404, client, " NOT FOUND");
        state = SENT;
        return false;
    }
    return false;
}