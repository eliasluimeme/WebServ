#include "../Includes/Includes.hpp"

Client::Client() {
    clientFd = -1;
    memset(&addr, 0, sizeof(addr));
    received = false;
    header = false;
    toRead = 0;
    readed = 0;
    left = 0;
    startTime.tv_sec = 0;
    send_headers = false;
    responseSent = 0;
    bytesSent = 0;
    headerSent = false;
    state = READY;
    offset = 0;
    pos = 0;
    reIndex = false;
}

Client& Client::operator=(const Client &cl) {
    if (this != &cl) {
        clientFd = cl.clientFd;
        addr = cl.addr;
        headers = cl.headers;
        method = cl.method;
        uri = cl.uri;
        http = cl.http;
        encoding = cl.encoding;
        delimiter = cl.delimiter;
        received = cl.received;
        header = cl.header;
        toRead = cl.toRead;
        readed = cl.readed;
        // request = cl.request;
    }
    return *this;
}

Client::~Client() {}

void Client::cleanup() {
    received = false;
    header = false;
    toRead = 0;
    readed = 0;
}

void Client::setFd(int fd) { clientFd = fd; }

int Client::getFd() { return clientFd; }

void Client::setAddr(struct sockaddr_in &socketAddr) { addr = socketAddr; }

sockaddr_in &Client::getAddr() { return addr; }

void Client::setHeaders(std::map<std::string, std::string> &header) { headers = header; }

std::map<std::string, std::string> &Client::getHeaders() { return headers; }

void Client::setHeaderValue(std::string &key, std::string &value) { headers[key] = value; }

std::string Client::getHeaderValue(std::string &key) { return headers[key]; }

void Client::setMethod(std::string &meth) { method = meth; }

std::string Client::getMethod() { return method; }

void Client::setURI(std::string &URI) { uri = URI; }

std::string Client::getURI() { return uri; }

void Client::setHTTP(std::string &HTTP) { http = HTTP; }

std::string Client::getHTTP() { return http; }

void Client::setEncoding(std::string &enco) { encoding = enco; }

std::string Client::getEncoding() { return encoding; }

void Client::setDelimiter(std::string &deli) { delimiter = deli; }

std::string Client::getDelimiter() { return delimiter; }

void Client::setQuery(std::string &q) { query = q; }

std::string Client::getQuery() { return query; }

void Client::setConfData(Data &confData) { data = confData; }

Data &Client::getConfData() { return data; }

void Client::setPos(std::streampos a)
{
    pos = a;
}

std::streampos& Client::getPos()
{
    return pos;
}

void Client::setSend(bool a)
{
    send_headers = a;
}

bool& Client::getSend()
{
    return send_headers;
}