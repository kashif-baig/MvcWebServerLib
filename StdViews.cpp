#include "StdViews.h"

#define BLOCK_SIZE  20

PROGMEM const char FoundStatusStr[] = "302 Found";
PROGMEM const char BadRequestStatusStr[] = "400 Bad Request";
PROGMEM const char ForbiddenStatusStr[] = "403 Forbidden";
PROGMEM const char UnsupportedMediaTypeStatusStr[] = "415 Unsupported Media Type";
PROGMEM const char InternalServerErrorStatusStr[] = "500 Internal Server Error";


HttpStatusCodeView::HttpStatusCodeView(uint16_t statusCode, const char *description, HttpRequest *request) : IView(HttpStatusCodeViewId, NULL, request)
{
    this->statusCode = statusCode;
    this->description = description;
}

//--------------------------------------------------------------------------------------------

uint16_t HttpStatusCodeView::getStatusCode() const
{
    return statusCode;
}

const char *HttpStatusCodeView::getDescription() const
{
    return description;
}

HttpResponseHeader *HttpStatusCodeView::getHttpResponseHeader()
{
    if (headerGeneratedInd)
    {
        return &HttpHeaders; 
    }

    HttpHeaders.clear();
    HttpHeaders.contentType_P = TextHtmlString;

    switch (statusCode)
    {
        case 200:
        {
            HttpHeaders.status_P = OkStatusStr;
            break;
        }
        case 302:
        {
            HttpHeaders.status_P = FoundStatusStr;
            HttpHeaders.locationUrl = description;
            break;
        }
        case 400:
        {
            HttpHeaders.status_P = BadRequestStatusStr;
            break;
        }
        case 403:
        {
            HttpHeaders.status_P = ForbiddenStatusStr;
            break;
        }
        case 415:
        {
            HttpHeaders.status_P = UnsupportedMediaTypeStatusStr;
            break;
        }
        case 500:
        {
            HttpHeaders.status_P = InternalServerErrorStatusStr;
            break;
        }
    }
    headerGeneratedInd = true;
    return &HttpHeaders;
}

//--------------------------------------------------------------------------------------------

void HttpStatusCodeView::onRender()
{

}

// ------------------------------------------------------------------------------------------

PContentFileView::PContentFileView(const char *content_P, uint32_t contentLen, const char *contentType_P, const char *encoding_P, HttpRequest *request)
: IView (PContentFileViewId, NULL, request)

{
    dataIdx = content_P;
    
    this->content_P = content_P;
    this->contentType_P = contentType_P;
    this->encoding_P = encoding_P;
    this->contentLen = contentLen;
}

const char *PContentFileView::getContent() const
{
    return this->content_P;
}

HttpResponseHeader *PContentFileView::getHttpResponseHeader()
{
    if (headerGeneratedInd)
    {
        return &HttpHeaders; 
    }

    HttpHeaders.clear();
    HttpHeaders.status_P = OkStatusStr;
    HttpHeaders.cacheAge = CacheControlMaxAge;
    HttpHeaders.contentEncoding_P = encoding_P;
    HttpHeaders.contentType_P = contentType_P;
    HttpHeaders.contentLen = contentLen;

    headerGeneratedInd = true;
    return &HttpHeaders;
}

void PContentFileView::onRender()
{
    // Write content.
    const char *endPtr = content_P + contentLen;

    if (endPtr - dataIdx > BLOCK_SIZE)
    {
        writeBytes_P(response, dataIdx, BLOCK_SIZE);
        dataIdx += BLOCK_SIZE;
        setRenderCompleted(false);
    }
    else
    {
        writeBytes_P(response, dataIdx, endPtr - dataIdx);
        setRenderCompleted(true);
    }
}

// ------------------------------------------------------------------------------------------
