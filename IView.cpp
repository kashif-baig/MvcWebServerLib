#include "IView.h"

HttpRequest *IView::defaultRequest;

IView::IView(uint8_t viewId, const char *viewFilePtr, HttpRequest *httpRequest)
    : viewId(viewId), request(httpRequest), response(httpRequest->getResponse()), viewFile(viewFilePtr)
{
    renderCompletedInd = false;
    isRenderingInd = false;
    headerGeneratedInd = false;
    renderState = 0;
}

//---------------------------------------------------------------------------------------

const char *IView::getMimeType() const
{
    switch (viewMimeType)
    {
    case ApplicationJson:
        return ApplicationJsonString;
    case ApplicationXml:
        return ApplicationXmlString;
    case TextHtml:
        return TextHtmlString;
    default:
        return TextPlainString;
    }
}

//---------------------------------------------------------------------------------------

uint8_t IView::getViewId() const
{
    return viewId;
}

//---------------------------------------------------------------------------------------

void IView::render()
{
    if (!isRenderingInd)
    {
        isRenderingInd = true;
        renderCompletedInd = true;
    }
    if (request)
    {
        onRender();
    }

    isRenderingInd = !renderCompletedInd;
}

//---------------------------------------------------------------------------------------

uint8_t IView::getRenderState () const
{
    return renderState;
}

void IView::setRenderState(uint8_t state)
{
    renderState = state;
}

//---------------------------------------------------------------------------------------

void IView::setRenderCompleted(bool value)
{
    renderCompletedInd = value;
}

bool IView::renderCompleted() const
{
    return renderCompletedInd;
}

//---------------------------------------------------------------------------------------

HttpResponseHeader *IView::getHttpResponseHeader()
{
    if (headerGeneratedInd)
    {
        return &HttpHeaders; 
    }
    
    const char *contentType = getMimeType();

    HttpHeaders.clear();
    HttpHeaders.status_P = OkStatusStr;
    HttpHeaders.contentType_P = contentType;

    headerGeneratedInd = true;
    return &HttpHeaders;
}

//---------------------------------------------------------------------------------------

// Write view markup (stored in program memory) to response stream
void IView::writeViewContent(uint16_t startPos, uint16_t endPos)
{
    uint8_t buf[32];

    for (; startPos <= endPos;)
    {
        uint16_t bytesToRead = (endPos - startPos) + 1;

        if (bytesToRead > sizeof(buf))
        {
            bytesToRead = sizeof(buf);
        }

        for (uint8_t i = 0; i < bytesToRead; i++)
        {
            buf[i] = pgm_read_byte(&(viewFile[startPos + i]));
        }
        response->write(buf, bytesToRead);
        startPos += bytesToRead;
    }
}

//---------------------------------------------------------------------------------------

void IView::writeStr(const char *str_P)
{
    if (!str_P)
        return;

    uint8_t i;
    char chr = pgm_read_byte(str_P++);

    do
    {
        uint8_t buf[32];

        for (i = 0; i < sizeof(buf) && chr != (char)0; i++)
        {
            buf[i] = chr;
            chr = pgm_read_byte(str_P++);
        }

        if (i > 0)
        {
            response->write(buf, i);
        }
    } while (i > 0);
}

//---------------------------------------------------------------------------------------

void IView::writeStrIf(bool expr, const char *str_P)
{
    if (expr) writeStr(str_P);
}