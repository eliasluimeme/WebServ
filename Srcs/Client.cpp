#include "../Includes/Includes.hpp"

Client::Client() {
    clientFd = -1;
    memset(&addr, 0, sizeof(addr));
    received = false;
    header = false;
    toRead = 0;
    readed = 0;
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

void Client::setAddrLen(int addrL) {addrLen = addrL; }

size_t Client::getAddrLen() { return addrLen; }

void Client::setRequestMsg(std::string msg) { request = msg; }

std::string Client::getRequestMsg() { return request; }

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

void Client::setReqFile(std::string file) { reqFile = file; }

std::string &Client::getReqFile() { return reqFile; }

void Client::setBody(std::string msg) { body = msg; }

std::string &Client::getBody() { return body; }