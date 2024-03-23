#ifndef IVIEW_H
#define IVIEW_H

#include "WebDebugConfig.h"
#include "WebUtility.h"
#include "HttpRequest.h"

PROGMEM const char ApplicationJsonString[] = "application/json";
PROGMEM const char ApplicationXmlString[] = "application/xml";
PROGMEM const char TextPlainString[] = "text/plain";

PROGMEM const char GzipEncodingStr[] = "gzip";
PROGMEM const char ImagePngStr[] = "image/png";
PROGMEM const char ImageGifStr[] = "image/gif";
PROGMEM const char ImageJpegStr[] = "image/jpeg";
PROGMEM const char TextCssStr[] = "text/css";
PROGMEM const char ApplicationJavascriptStr[] = "application/javascript";

PROGMEM const char PngExtStr[] = ".png";
PROGMEM const char GifExtStr[] = ".gif";
PROGMEM const char JpgExtStr[] = ".jpg";
PROGMEM const char CssExtStr[] = ".css";
PROGMEM const char JsExtStr[]  = ".js";
PROGMEM const char IcoExtStr[] = ".ico";

PROGMEM const char HtmExtStr[] = ".htm";
PROGMEM const char TxtExtStr[] = ".txt";
PROGMEM const char CsvExtStr[] = ".csv";
PROGMEM const char XmlExtStr[] = ".xml";

const uint32_t CacheControlMaxAge = 3600; // content caching in browser measured in seconds.

enum MvcViewId : uint8_t
{
    HttpStatusCodeViewId = 240,
    PContentFileViewId,
    SDContentFileViewId,
    ViewIdNotSpecified = 255
};

enum ViewMimeType : uint8_t
{
    TextHtml,
    ApplicationJson,
    ApplicationXml,
    TextPlain
};


// Base class for implementing views. Sub classes must set 'webServer' and 'response' to valid instances.
class IView
{
public:
    // Gets the mime type string (stored in program memory) of the view.
    const char *getMimeType() const;

    // Gets the id of the view. All views should have a unique ID.
    // When not set, the value should be ViewIdNotSpecified.
    uint8_t getViewId() const;

    // Render view to response stream..
    void render();
    
    // Returns true when rendering is complete. Returns false if
    // successive calls to render() are required.
    bool renderCompleted() const;

    // Get default response headers for MvcWebServer to render.
    // When overriding this method, set headerGeneratedInd to true when the
    // header has been generated, and do not regenerate on subsequent calls.
    virtual HttpResponseHeader *getHttpResponseHeader();

    static void setDefaultRequest (HttpRequest *defaultHttpRequest) { defaultRequest = defaultHttpRequest; }
    static HttpRequest *getDefaultRequest () { return defaultRequest; }

protected:
    // Parameters for contructing a View.
    IView(uint8_t viewId, const char *viewFilePtr, HttpRequest *httpRequest);

    // Render view body.
    virtual void onRender() = 0;
    
    // Set render complete indicator.
    void setRenderCompleted(bool value);

    // Write view markup (stored in program memory) to response stream
    void writeViewContent(uint16_t startPos, uint16_t endPos);

    // Write view markup string (stored in program memory)
    void writeStr(const char *str_P);
    
    // Write view markup string (stored in program memory), if expr is true.
    void writeStrIf(bool expr, const char *str_P);

    ViewMimeType viewMimeType = TextHtml;

    // The HTTP request instance.
    HttpRequest * const request;
    // Response stream for serving the request.
    Print * const response;

    // Response headers.
    HttpResponseHeader HttpHeaders;

    // Set to true once the header has been generated.
    bool headerGeneratedInd;

    // For users wanting to implement a finite state machine.
    void setRenderState(uint8_t state);
    uint8_t getRenderState () const;
    
private:
    static HttpRequest *defaultRequest;
    bool renderCompletedInd;
    bool isRenderingInd;
    const char *viewFile;
    uint8_t viewId;

    uint8_t renderState;
};
#endif
