#ifndef IWEB_SERVER_H
#define IWEB_SERVER_H

#include <Arduino.h>
#include "IBindableModel.h"

class IWebServer
{
public:
    // Gets a reference to the client.
    virtual Print *getResponse() const = 0;

    // Binds URL encoded http post data to model. 
    virtual void bindModel(IBindableModel *model, bool trimValues = true) = 0;

    // Sets the auth ticket.
    // Setting to NULL will result in the authentication cookie being cleared, and the
    // authentication status of the current HttpRequest is also cleared.
    virtual void setAuthTicket(const char *authTicketStr) = 0;

    // Gets the auth ticket. Returns NULL or an empty string if none set.
    virtual const char *getAuthTicket() const = 0;

    // Sets the authenticated user.
    virtual void setAuthenticatedUser(const char *authUserStr) = 0;

};

#endif