#ifndef SDCONTENTFILEVIEW_H
#define SDCONTENTFILEVIEW_H

#include "WebDebugConfig.h"
#include "IView.h"

#ifndef SDCONTENTFILEVIEW_USE_OTHER_SD
    #include <SD.h>
    #define MyFile File
#endif


// View for rendering content files stored on an SD card.
// The following file types are supported: htm, png, gif, jpg, css, js, ico, txt, csv, xml.
class SDContentFileView : public IView
{
public:
    SDContentFileView(const char *filePath, HttpRequest *request = getDefaultRequest());

    SDContentFileView(const char *filePath, const char *attachmentName, HttpRequest *request = getDefaultRequest());

    // Gets the path of the content file.
    const char *getFilePath() const;

    // Gets the response header instance.
    HttpResponseHeader *getHttpResponseHeader();

    // Returns true only if the file is found and is of supported type.
    bool fileFound() const;

protected:
    void onRender();

private:
    const char *filePath;
    const char *attachmentName;

    MyFile file;
};

#define SD_CONTENT_FILE_VIEW_SIZE   sizeof(SDContentFileView)

#ifndef SDCONTENTFILEVIEW_BLOCK_SIZE
    #define SDCONTENTFILEVIEW_BLOCK_SIZE 20
#endif


PROGMEM const KeyValueStrPair extToTypeMap[] = {
    {HtmExtStr, TextHtmlString},
    {PngExtStr, ImagePngStr},
    {GifExtStr, ImageGifStr},
    {JpgExtStr, ImageJpegStr},
    {CssExtStr, TextCssStr},
    {JsExtStr, ApplicationJavascriptStr},
    {IcoExtStr, ImagePngStr},
    {TxtExtStr, TextPlainString},
    {CsvExtStr, TextPlainString},
    {XmlExtStr, ApplicationXmlString}};

SDContentFileView::SDContentFileView(const char *filePath, HttpRequest *request)
    : SDContentFileView(filePath, NULL, request)
{
}

SDContentFileView::SDContentFileView(const char *filePath, const char *attachmentName, HttpRequest *request)
    : IView(SDContentFileViewId, NULL, request)
{
    this->filePath = filePath;
    this->attachmentName = attachmentName;
}


bool SDContentFileView::fileFound() const
{
    return headerGeneratedInd;
}

HttpResponseHeader *SDContentFileView::getHttpResponseHeader()
{
    if (headerGeneratedInd)
    {
        return &HttpHeaders; 
    }

    const char *fname = filePath;
    file = SD.open(fname);

    if (!file)
    {
        // Serve 404 if file not found.
        HttpHeaders.set404NotFound();
        return &HttpHeaders;
    }

    uint32_t fileSize = file.size();

    bool contentTypeMatched = false;
    uint8_t contentTypeIdx = 0;

    for (; contentTypeIdx < keyValueStrPairArrayLen(extToTypeMap); contentTypeIdx++)
    {
        if (endsWith_P(fname, (char *)pgm_read_ptr(&(extToTypeMap[contentTypeIdx].key))))
        {
            contentTypeMatched = true;
            break;
        }
    }

    HttpHeaders.clear();
    HttpHeaders.status_P = OkStatusStr;
    HttpHeaders.contentLen = fileSize;

    if (attachmentName != NULL && strlen(attachmentName) > 0)
    {
        HttpHeaders.attachmentName = attachmentName;
    }
    else
    {
        HttpHeaders.cacheAge = CacheControlMaxAge;
    }

    if (contentTypeMatched)
    {
        HttpHeaders.contentType_P = (char *)pgm_read_ptr(&(extToTypeMap[contentTypeIdx].value));
    }
    else
    {
        file.close();
        // Serve 404 if content type not supported.
        HttpHeaders.set404NotFound();
        return &HttpHeaders;
    }
    headerGeneratedInd = true;

    return &HttpHeaders;
}


const char *SDContentFileView::getFilePath() const
{
    return this->filePath;
}

void SDContentFileView::onRender()
{
    if (headerGeneratedInd)
    {
        // Once headers have been written to the response, the file is served a chunk
        // at a time; one chunk for every invocation of the render method.
        uint8_t buf[SDCONTENTFILEVIEW_BLOCK_SIZE];

        if (file.available())
        {
            uint32_t bytesRead = file.read(buf, sizeof(buf));
            if (bytesRead)
            {
                response->write(buf, bytesRead);
            }
            setRenderCompleted(false);
        }
        if (!file.available())
        {
            setRenderCompleted(true);
            file.close();
        }
        return;
    }
    // Header not generated, so complete render.
    setRenderCompleted(true);
}


#endif
