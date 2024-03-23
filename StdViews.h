#ifndef STDVIEWS_H
#define STDVIEWS_H

#include "WebDebugConfig.h"
#include "IView.h"

// IMPORTANT! Must ensure LARGEST_STD_VIEW_SIZE (near end of file) is kept up to date.

// View for returning HTTP status codes.
class HttpStatusCodeView : public IView
{
public:
    HttpStatusCodeView(uint16_t statusCode, const char *description = NULL, HttpRequest *request = getDefaultRequest());

    uint16_t getStatusCode() const;
    const char *getDescription() const;

    HttpResponseHeader *getHttpResponseHeader();

protected:
    uint16_t statusCode;
    const char *description;

    void onRender();
};


// View for rendering content files stored in program memory.
class PContentFileView : public IView
{
public:
    PContentFileView(const char *content_P, uint32_t contentLen, const char *contentType_P, const char *encoding_P, HttpRequest *request = getDefaultRequest());

    const char *getContent() const;

    HttpResponseHeader *getHttpResponseHeader();
protected:
    const char *content_P;
    const char *contentType_P;
    const char *encoding_P;
    uint32_t contentLen;

    const char *dataIdx;

    void onRender();
};



// Determine largest size for standard views.
static const size_t LARGEST_STD_VIEW_SIZE =
    STATIC_MAX(sizeof(PContentFileView),
    STATIC_MAX(sizeof(HttpStatusCodeView),
     0)
    );


#endif