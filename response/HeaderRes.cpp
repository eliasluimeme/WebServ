#include "HeaderRes.hpp"


HeaderRes::HeaderRes()
{
}

HeaderRes::~HeaderRes()
{
}


std::string HeaderRes::getHeader(size_t size, const std::string &path, int code, std::string type, const std::string &contentLocation, const std::string &lang)
{
    std::string header;
    resetValue();
    setValue(size, path, code, type, contentLocation, lang);
    header = "HTTP/1.1" + to_string(code), " " + getstatusMssg(code) + "\r\n";
    header += writeHeader();

    return header;
}

std::string HeaderRes::Notallowed(std::set<std::string> methods, const std::string &path, int code, const std::string &lang)
{
    std::string header;
    resetValue();
    setValue(0, path, code, "", path, lang);
    setAllow(methods);
    if (code == 405)
        header = "HTTP/1.1 405 method not allowed\r\n";
    else if (code == 413)
    {
        header = "HTTP/1.1 413 payload too Large\r\n";
    }
    header += writeHeader();
    return header;
}

std::string HeaderRes::writeHeader()
{
    std::string header = "";

    if (_allow != "")
        header += "allow: " + _allow + "\r\n";
    if (_contentLanguage != "")
        header += "Content-Language: " + _contentLanguage + "\r\n";
    if (_contentLength != "")
        header += "Content-length: " + _contentLength + "\r\n";
    if (_contentLocation != "")
        header += "Content-Location: " + _contentLocation + "\r\n";
    if (_contentType != "")
        header += "content-type: " + _contentType + "\r\n";
    if (_date != "")
        header += "Date: " + _date + "\r\n";
    if (_lastModified != "")
        header += "Last-Modified: " + _lastModified + "\r\n";
   if (_location != "")
		header += "Location: " + _location + "\r\n";
	if (_retryAfter != "")
		header += "Retry-After: " + _retryAfter + "\r\n";
	if (_server != "")
		header += "Server: " + _server + "\r\n";
	if (_transferEncoding != "")
		header += "Transfer-Encoding: " + _transferEncoding + "\r\n";
	if (_wwwAuthenticate != "")
		header += "WWW-Authenticate: " + _wwwAuthenticate + "\r\n";
}


std::string HeaderRes::getstatusMssg(int code)
{
    if (_errors.find(code) != _errors.end())
        return _errors[code];
    return "Unknown code";
}

void HeaderRes::initErrorMap()
{
   _errors[100] = "Continue";
	_errors[200] = "OK";
	_errors[201] = "Created";
	_errors[204] = "No Content";
	_errors[400] = "Bad Request";
	_errors[403] = "Forbidden";
	_errors[404] = "Not Found";
	_errors[405] = "Method Not Allowed";
	_errors[413] = "Payload Too Large";
	_errors[500] = "Internal Server Error";
}
void HeaderRes::setValue(size_t size, const std::string &path, int code, std::string type, const std::string &contentLocation, const std::string &lang)
{
    setAllow();
    setContentLanguage(lang);
    setContentLength(size);
    setContentLocation(contentLocation, code);
    setContentType(type, path);
    setDate();
    setLastModified(path);
    setLocation(code, path);
    setRetryAfter(code, 3);
    setServer();
    setTransferEncoding();
    setWwwAuthenticate(code);
}
void HeaderRes::resetValue()
{
    _allow = "";
	_contentLanguage = "";
	_contentLength = "";
	_contentLocation = "";
	_contentType = "";
	_date = "";
	_lastModified = "";
	_location = "";
	_retryAfter = "";
	_server = "";
	_transferEncoding = "";
	_wwwAuthenticate = "";
	initErrorMap();
}

void HeaderRes::setAllow(std::set<std::string> method)
{

}

void HeaderRes::setAllow(const std::string &allow)
{

}

void HeaderRes::setContentLanguage(const std::string &lang)
{
    
}

void HeaderRes::setContentLength(size_t size)
{
}

void HeaderRes::setContentLocation(const std::string &path, int code)
{
}

void HeaderRes::setContentType(std::string type, std::string path)
{
}

void HeaderRes::setDate()
{
}

void HeaderRes::setLastModified(const std::string &path)
{
}

void HeaderRes::setLocation(int code, const std::string &redirect)
{
}

void HeaderRes::setRetryAfter(int code, int sec)
{
}

void HeaderRes::setServer(void)
{
}

void HeaderRes::setTransferEncoding(void)
{
}

void HeaderRes::setWwwAuthenticate(int code)
{
}
