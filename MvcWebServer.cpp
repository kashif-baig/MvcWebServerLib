#include "MvcWebServer.h"
#include <new>

#define URL_ARGV_LEN                8
#define RENDER_TIMEROUT_MS          30000
#define HTTP_POST_DEFAULT_TIMEOUT   5000

#if defined(ESP8266) || defined(ESP32)
    #define DEFAULT_CLIENT_TIMEOUT      2000
#elif defined(ARDUINO_UNOWIFIR4)
    #define DEFAULT_CLIENT_TIMEOUT      5000
#else
    #define DEFAULT_CLIENT_TIMEOUT      1000
#endif

PROGMEM const char getMethodStr[] = "get";
PROGMEM const char CookieStr[] = "Cookie";
PROGMEM const char XRequestedWithStr[] = "X-Requested-With";
PROGMEM const char XMLHttpRequestStr[] = "XMLHttpRequest";
PROGMEM const char FetchRequestStr[] = "Fetch";
PROGMEM const char ContentTypeStr[] = "Content-type";
PROGMEM const char ContentLengthStr[] = "Content-Length";
PROGMEM const char ApplFormUrlEncodedStr[] = "application/x-www-form-urlencoded";
PROGMEM const char contentPathStr[] = "Content";
PROGMEM const char AuthTicketNameStr[] = "auth_ticket";

char emptyStr[] = "";
char *argv[URL_ARGV_LEN];


union
{
    char Memory[sizeof(HttpRequest)];
    uint32_t dummy;
} HttpRequestUnion;

static char* HttpRequestMemory = HttpRequestUnion.Memory;

union
{
    char Memory[sizeof(HttpPostArgs)];
    uint32_t dummy;
} HttpPostArgsUnion;

static char* HttpPostArgsMemory = HttpPostArgsUnion.Memory;

HttpPostArgs *httpPostArgs;

//---------------------------------------------------------------------------------------

MvcWebServer::MvcWebServer(Client *client, IController *controller,
                              IStringBuilder *requestBuffer, IStringBuilder *urlBuffer)
{
    this->client = client;
    this->controller = controller;

    this->requestBuffer = requestBuffer;
    this->urlBuffer = urlBuffer;

    clear();
}

//---------------------------------------------------------------------------------------

// Clears and resets the web server processing state.
void MvcWebServer::clear()
{
    resetState();
    isAjax = false;
    isHttpPost = false;
    requestLineCtr = 0;
    contentLength = 0;
    contentLengthCtr = 0;
    authTicket = NULL;
    authUser = NULL;
    isAuthenticatedInd = false;
    payLoadTooLarge = false;
    formUrlEncoded = false;
    requestBuffer->clear();
    urlBuffer->clear();
}

// resets web server request processing state.
void MvcWebServer::resetState() { requestState = MvcBeginRequest; }

//---------------------------------------------------------------------------------------

void MvcWebServer::processRequest()
{
    if ((controller == NULL || requestBuffer == NULL || urlBuffer == NULL))
    {
        return;
    }
    if (client->connected())
    {
        bool exitLoop = false;

        while (!exitLoop)
        {
            switch (requestState)
            {
            case MvcBeginRequest: // Request begin
            {
                //debugln(F("-Begin request-"));
                clear();
                requestState = MvcReadingLine;
                controller->onBeginRequest();
                break;
            }
            case MvcReadingLine: // Reading request line
            {
                exitLoop = true;

                while (client->available())
                {
                    const char chr = client->read();
                    if (chr == '\r')
                    {
                    }
                    else if (chr == '\n')
                    {
                        // debugln(requestBuffer->toString());
                        requestState = MvcProcessingLine;

                        exitLoop = false;
                        break;
                    }
                    else
                    {
                        requestBuffer->append(chr);
                    }
                }
                break;
            }
            case MvcReadingPostData: // Reading post data
            {
                //debugln(F("-MvcReadingPostData-"));

                exitLoop = true;

                if (contentLength && (contentLengthCtr >= contentLength))
                {
                    requestState = MvcServingView;
                }
                else if (millis() - httpPostReadTime < HTTP_POST_DEFAULT_TIMEOUT /* httpPostArgs->getTimeout()*/)
                {
                    if (client->available())
                    {
                        httpPostReadTime = millis();
                        
                        uint8_t postDataConsumed = controller->onReceivingPostData(httpPostArgs);

                        if (!postDataConsumed)
                        {
                            while (client->available())
                            {
                                const char chr = client->read();
                                contentLengthCtr++;

                                if (!requestBuffer->append(chr))
                                {
                                    payLoadTooLarge = true;
                                }
                            }
                        }
                        else
                        {
                            contentLengthCtr += postDataConsumed;
                        }
                    }
                }
                else if (!payLoadTooLarge)
                {
                    //debugln(requestBuffer->toString());
                    requestState = MvcServingView;
                }
                else
                {
                    HttpHeaders.clear();
                    HttpHeaders.status_P = PayloadTooLargeStatusStr;
                    HttpHeaders.contentType_P = TextHtmlString;
                    HttpHeaders.writeHeader(client, false);
                    controller->onPreRender();
                    writeStr_P(client, ConnCloseStr);
                    writeStr_P(client, PSTR("Payload Too Large"));
                    
                    requestState = MvcEndRequest;
                }

                if (requestState == MvcServingView)
                {
                    client->setTimeout(DEFAULT_CLIENT_TIMEOUT);
                }
                break;
            }
            case MvcProcessingLine: // Processing a line that was read
            {
                if (requestBuffer->length() == 0)
                {
                    //debugln(F("-Blank line-"));
                    // Blank line has been read.


                    // Construct Request object.
                    uint8_t argc = 0;
                    //char *argv[URL_ARGV_LEN];

                    urlBuffer->trimEnd("/"); // trim trailing '/'

                    StringReader urlReader = StringReader(urlBuffer->toString());
                    if (urlReader.peekChar() == '/')
                    {
                        urlReader.readChar(); // skip leading '/'
                    }

                    // Split URL by '/', and ignore empty strings.
                    while (urlReader.available() && urlReader.peekChar() != 0)
                    {
                        argv[argc] = urlDecode(urlReader.readUntil("/"));
                        argc++;
                    }

                    // For all unused pointers in argv[], set them to empty string.
                    for (uint8_t i = argc; i < URL_ARGV_LEN; i++)
                    {
                        argv[i] = emptyStr;//"";
                    }

                    //debugln(F("httpRequest set"));
                    httpRequest = new (HttpRequestMemory) HttpRequest(argc, argv, isHttpPost, isAjax, authUser, isAuthenticatedInd, client);

                    IView::setDefaultRequest(httpRequest);

                    if (isHttpPost)
                    {
                        client->setTimeout(0); // prevent busy waiting on incoming stream.
                        requestBuffer->clear();
                        httpPostReadTime = millis();
                        httpPostArgs = new (HttpPostArgsMemory) HttpPostArgs(httpRequest, client, contentLength/*, HTTP_POST_DEFAULT_TIMEOUT*/);

                        requestState = MvcReadingPostData;
                    }
                    else
                    {
                        requestState = MvcServingView;
                        exitLoop = true;
                    }
                }
                else    // must be request header, so process it.
                {
                    StringReader reader = StringReader(requestBuffer->toString());
                    char *headerName = reader.readUntil(" ");

                    // Trim header end of ':' character.
                    char* trimEnd = headerName + strlen(headerName)-1;
                    while (trimEnd >= headerName && *trimEnd == ':')
                    {
                        *trimEnd = 0;
                        trimEnd--;
                    }

                    char *headerValue = reader.peekString();
                    controller->onInspectHeader(headerName, headerValue);

                    //  If this is the first line, check http method, and extract URL.
                    if (requestLineCtr == 0)
                    {
                        if (strcasecmp_P(headerName, getMethodStr) != 0)
                        {
                            isHttpPost = true;
                        }
                        // copy url to url buffer.
                        urlBuffer->append(reader.readUntil(" "));
                        //debugln(urlBuffer->toString());
                    }
                    else if (strcasecmp_P(headerName, XRequestedWithStr) == 0)
                    {
                        //debugln(F("-Ajax-"));
                        if (strcasecmp_P(headerValue, XMLHttpRequestStr) == 0 || strcasecmp_P(headerValue, FetchRequestStr)==0)
                        {
                            //debugln(F("-Ajax confirmed-"));
                            isAjax = true;
                        }
                    }
                    else if (strcasecmp_P(headerName, CookieStr) == 0)
                    {
                        //debugln(F("-Cookie-"));
                        // look for auth ticket, and authenticate.
                        while (reader.available())
                        {
                            StringReader cookieReader = StringReader(reader.readUntil("; "));
                            const char *cookieName = cookieReader.readUntil("=");

                            //debugln(cookieName);

                            if (strcasecmp_P(cookieName, AuthTicketNameStr) == 0)
                            {
                                //debugln(F("-Auth cookie found-"));
                                const char *authTicketVal = cookieReader.readToEnd();
                                //debugln(authTicketVal);

                                if (authTicketVal != NULL && *authTicketVal != 0)
                                {
                                    if (!isAuthenticatedInd)
                                    {
                                        isAuthenticatedInd = controller->onAuthenticate(authTicketVal);
                                    }
                                }
                            }
                        }
                    }
                    else if (strcasecmp_P(headerName, ContentTypeStr) == 0)
                    {
                        if (strcasecmp_P(headerValue, ApplFormUrlEncodedStr) == 0)
                        {
                            formUrlEncoded = true;
                        }
                    }
                    else if (strcasecmp_P(headerName, ContentLengthStr) == 0)
                    {
                        contentLength = atoi(headerValue);
                    }
                    requestBuffer->clear();
                    requestState = MvcReadingLine;
                }
                requestLineCtr++;
                break;
            }
            case MvcServingView: // Serve a view
            {
                //debugln(F("-MvcServingView-"));
                // Serve the view
                currentView = NULL;

                uint8_t argc = httpRequest->getArgc();
                char const* const *argv = httpRequest->getArgv();

                // Check if a static file is requested.
                if (
                    (argc >= 2 && (strcasecmp_P(argv[0], contentPathStr) == 0)) ||
                    (argc == 1 && (strcasecmp_P(argv[0], FaviconStr) == 0)))
                {
                    // Serve file.
                    currentView = controller->onServeContent(argv[argc == 1 ? 0 : 1]);
                    if (currentView)
                    {
                        HttpResponseHeader *header = currentView->getHttpResponseHeader();
                        if (header)
                        {
                            header->writeHeader(client, false);
                            controller->onPreRender();
                            writeStr_P(client, ConnCloseStr);
                        }
                        //debugln(F("-file served-"));
                    }
                }
                // Otherwise, it must be a page request.
                else
                {
                    // Serve page.
                    currentView = controller->onProcessRequest(httpRequest);

                    if (currentView)
                    {
                        HttpResponseHeader *header = currentView->getHttpResponseHeader();
                        if (header)
                        {
                            header->authTicket = authTicket;
                            header->writeHeader(client, false);
                            controller->onPreRender();
                            writeStr_P(client, ConnCloseStr);
                        }
                        //debugln(F("-view served-"));
                    }
                }

                if (!currentView)
                {
                    // No view to serve? Send 404 response.
                    HttpHeaders.set404NotFound();
                    HttpHeaders.writeHeader(client, false);
                    controller->onPreRender();
                    writeStr_P(client, ConnCloseStr);

                    responseCompleteTime = millis();
                    requestState = MvcEndRequest;
                }
                else
                {
                    requestState = MvcRenderingView;
                    renderViewStartTime = millis();
                }
                //debugln(F("-ending request-"));
                exitLoop = true;
                break;
            }
            case MvcRenderingView:    // Render view
            {
                if (millis() - renderViewStartTime > RENDER_TIMEROUT_MS) // Need to end request if rendering timeout exceeded.
                {
                    requestState = MvcEndRequest;
                }
                else
                {
                    currentView->render();

                    if (currentView->renderCompleted())
                    {
                        responseCompleteTime = millis();
                        requestState = MvcEndRequest;
                    }
                }
                exitLoop = true;
                break;
            }
            case MvcEndRequest: // Request completion
            {
                // If content size is included in response header, the client connection automatically disconnects
                // and this case statement is skipped.
                if (millis() - responseCompleteTime < 5)
                {
                    while (client->available())
                    {
                        client->read();
                    }
                }
                else
                {
                    //debugln(F("-client stop-"));
                    client->stop();
                    controller->onEndRequest();
                    resetState();
                }

                IView::setDefaultRequest(NULL);
                exitLoop = true;
                break;
            }
            default:
            {
                //debugln(F("-Default-"));
                break;
            }
            }
        }
    }
    else
    {
        //debugln(F("-Client disconnected-"));

        if (isProcessingRequest())
        {
            //debugln(F("-reset in case still processing request-"));
            client->stop();

            controller->onEndRequest();
            resetState();
            //debugln(F("-client stop-"));
        }
    }
}

//---------------------------------------------------------------------------------------

void MvcWebServer::bindModel(IBindableModel *model, bool trimValues)
{
    if (isHttpPost && formUrlEncoded)
    {
        model->onBindInit();
        
        // Http Post data should be in the request buffer.
        StringReader reader = StringReader(requestBuffer->toString());

        while (reader.available())
        {
            char *formDataStr = reader.readUntil("&");

            StringReader formDataItem = StringReader(formDataStr);
            const char *formKey = formDataItem.readUntil("=");
            char *formVal = formDataItem.readToEnd();

            formVal = urlDecode(formVal ? formVal : emptyStr);

            model->onBind(formKey, trimValues ? trim(formVal) : formVal);
        }
        model->onBindComplete();
    }
}

//---------------------------------------------------------------------------------------

void MvcWebServer::setAuthTicket(const char *authTicketStr)
{
    //debug(F("-Setting auth ticket to ..."));

    if (authTicketStr != NULL && strlen(authTicketStr) > 0)
    {
        authTicket = authTicketStr;
        //debugln(authTicketStr);
    }
    else
    {
        authTicket = emptyStr;
        authUser = emptyStr;
        isAuthenticatedInd = false;

        if (httpRequest)
        {
            httpRequest->clearAuthStatus();
        }
        //debugln(F("empty/NULL"));
    }
}

//---------------------------------------------------------------------------------------

const char *MvcWebServer::getAuthTicket() const { return authTicket; }

void MvcWebServer::setAuthenticatedUser(const char *authUserStr) { authUser = authUserStr; }

// Returns true if a request is currently being processed.
bool MvcWebServer::isProcessingRequest() const { return requestState != MvcBeginRequest; }

// // Gets a reference to the client.
Print *MvcWebServer::getResponse() const { return client; };


//---------------------------------------------------------------------------------------