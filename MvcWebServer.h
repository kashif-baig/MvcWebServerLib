#ifndef MVC_WEB_SERVER_H
#define MVC_WEB_SERVER_H

#define MVC_WEBSERVER_V3_0

#include "WebDebugConfig.h"
#include "WebUtility.h"
#include <Client.h>
#include <StringLib.h>
#include "IView.h"
#include "HttpRequest.h"
#include "IController.h"
#include "IBindableModel.h"

#include "IWebServer.h"

enum MvcRequestState : uint8_t
{
    MvcBeginRequest,
    MvcReadingLine,
    MvcProcessingLine,
    MvcReadingPostData,
    MvcServingView,
    MvcRenderingView,
    MvcEndRequest
};

// MvcWebServer class for processing http requests.
class MvcWebServer : public IWebServer
{
public:
    MvcWebServer(Client *client, IController *controller, IStringBuilder *requestBuffer, IStringBuilder *urlBuffer);

    // Request processing happens in this method.
    void processRequest();

    // Returns true if a request is currently being processed.
    bool isProcessingRequest() const ;

    // Gets a reference to the client.
    Print *getResponse() const ;

    // Binds URL encoded http post data to model. 
    void bindModel(IBindableModel *model, bool trimValues = true);

    // Sets the auth ticket.
    // Setting to NULL will result in the authentication cookie being cleared, and the
    // authentication status of the current HttpRequest is also cleared.
    void setAuthTicket(const char *authTicketStr);

    // Gets the auth ticket. Returns NULL or an empty string if none set.
    const char *getAuthTicket() const;

    // Sets the authenticated user.
    void setAuthenticatedUser(const char *authUserStr);

private:

    // Clears and resets the web server processing state.
    void clear();
    
    // resets web server request processing state.
    void resetState();

    IController *controller;
    const char *authUser;
    const char *authTicket;
    bool isAuthenticatedInd;

    HttpRequest *httpRequest;

    Client *client;

    bool isHttpPost = false;
    bool isAjax = false;

    IStringBuilder *requestBuffer;
    IStringBuilder *urlBuffer;

    MvcRequestState requestState = MvcBeginRequest;

    uint8_t requestLineCtr = 0;             // request line counter.

    uint32_t responseCompleteTime = 0;
    uint32_t httpPostReadTime = 0;
    uint32_t renderViewStartTime = 0;
    uint32_t contentLength = 0;             // content length obtained from request header.
    uint32_t contentLengthCtr = 0;          // current content length counter.

    IView *currentView;

    bool payLoadTooLarge = false;
    bool formUrlEncoded = false;

    HttpResponseHeader HttpHeaders;
};


#endif