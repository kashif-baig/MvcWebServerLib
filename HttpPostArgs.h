#ifndef HTTP_POST_ARGS_H
#define HTTP_POST_ARGS_H

#include <Arduino.h>
#include "HttpRequest.h"

// Represents the request context of raw posted data.
class HttpPostArgs
{
    public:

    HttpPostArgs(HttpRequest *httpRequest, Stream *stream, uint32_t contentLength);

    HttpRequest *getRequest() const ;
    Stream *getClient() const ;

    uint32_t getContentLength () const ;
    
private:
    HttpRequest *request;
    Stream *client;

    uint32_t contentLength;
};

#endif
