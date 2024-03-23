#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "IView.h"
#include "HttpPostArgs.h"


// Interface specifying MVC controller class for implementing the Http Request lifecycle.
class IController
{
    public:

    // Signals the start of a new request.
    virtual void onBeginRequest() {}

    // Allows each http request header to be inspected.
    virtual void onInspectHeader (const char *name, const char *value) {}

    // Returns true if the supplied ticket successfully authenticates, false otherwise.
    virtual bool onAuthenticate (const char *ticket) { return false; }

    // Serves a static file that has been requested as /Content/<filename>.
    // Must return NULL if no such file exists.
    virtual IView *onServeContent (const char *fileName) { return NULL; }
    
    // Allows raw HTTP Post data to be processed. Must return the number
    // of bytes consumed from the stream.
    // Method is invoked repeatedly until no more Post data available.
    virtual uint8_t onReceivingPostData(HttpPostArgs *httpPostArgs) { return 0; }

    // Performs requested action, then serves a (page) view to the requesting client.
    // Must return NULL if there is no view to serve.
    virtual IView *onProcessRequest(HttpRequest *request) = 0;

    // Occurs before a view is rendered, but after headers have been written.
    // This is an opportunity to write any additional headers before rendering the view.
    // This method is still invoked even if there is no view to render, e.g. 404 not found.
    virtual void onPreRender() { }

    // Signals the end of the current request.
    virtual void onEndRequest() { }
};

#endif
