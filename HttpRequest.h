#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <Arduino.h>

// Represents an Http request.
class HttpRequest
{
public:
    HttpRequest(uint8_t argc, char const* const *argv, Print *response);

    HttpRequest(uint8_t argc, char const* const *argv, bool isHttpPost, bool isAjax,
            Print *response);

    HttpRequest(uint8_t argc, char const* const *argv, bool isHttpPost, bool isAjax,
            const char *authUserStr, bool isAuthenticated, Print *response);

    // Clears the authentication status of the request.
    void clearAuthStatus() ;

    // Returns true if the request is Http Post, false otherwise.
    bool isHttpPost() const ;

    // Returns true if the request was made by client script, false otherwise.
    bool isAjax() const ;

    // Returns the number of arguments.
    uint8_t getArgc() const ;

    // Returns arguments as an array of pointers.
    char const* const *getArgv() const ;

    // Gets authenticated user name.
    const char *getAuthenticatedUser() const ;

    // Returns true if request is authenticated.
    bool isAuthenticated() const ;

    // Returns the response stream for the request.
    Print *getResponse() const ;

private:
    int8_t argc;
    char const* const *argv;
    bool isHttpPostInd;
    bool isAjaxInd;

    const char *authUser;
    bool isAuthenticatedInd;
    Print *response;
};

#endif
