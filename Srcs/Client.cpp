#include "../Includes/Includes.hpp"

Client::Client() {
    index = -1;
    clientFd = -1;
    memset(&addr, 0, sizeof(addr));
}

Client::Client(int fd): clientFd(fd) {
    index = -1;
    memset(&addr, 0, sizeof(addr));
}

Client::~Client() {}

void Client::setFd(int fd) { clientFd = fd; }

int Client::getFd() { return clientFd; }

void Client::setIndex(int index) { index = index; }

int Client::getIndex() { return index; }

void Client::setAddr(struct sockaddr_in &socketAddr) { addr = socketAddr; }

sockaddr_in &Client::getAddr() { return addr; }

void Client::setAddrLen(int addrL) {addrLen = addrL; }

size_t Client::getAddrLen() { return addrLen; }

void Client::setRequestMsg(std::string msg) { request = msg; }

std::string Client::getRequestMsg() { return request; }

void Client::setHeaders(std::map<std::string, std::string> header) { headers = header; }

std::map<std::string, std::string> &Client::getHeaders() { return headers; }

void Client::setHeaderValue(std::string &key, std::string &value) { headers[key] = value; }

std::string Client::getHeaderValue(std::string &key) { return headers[key]; }