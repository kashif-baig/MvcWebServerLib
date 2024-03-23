#include "HttpPostArgs.h"

    HttpPostArgs::HttpPostArgs(HttpRequest *httpRequest, Stream *stream, uint32_t contentLength)
    {
        request = httpRequest;
        client = stream;
        this->contentLength = contentLength;
    }

    HttpRequest *HttpPostArgs::getRequest() const { return request;}
    
    Stream *HttpPostArgs::getClient() const { return client; }

    uint32_t HttpPostArgs::getContentLength () const { return contentLength; }