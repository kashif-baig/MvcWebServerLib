#include "HttpRequest.h"

HttpRequest::HttpRequest(uint8_t argc, char const* const *argv, bool isHttpPost, bool isAjax,
        const char *authUserStr, bool isAuthenticated, Print *response)
{
    this->argc = argc;
    this->argv = argv;
    this->isHttpPostInd = isHttpPost;
    this->isAjaxInd = isAjax;

    this->authUser = authUserStr;
    this->isAuthenticatedInd = isAuthenticated;
    this->response = response;
}

HttpRequest::HttpRequest(uint8_t argc, char const* const *argv, Print *response)
    :HttpRequest(argc, argv, false, false, NULL, false, response)
{

}

HttpRequest::HttpRequest(uint8_t argc, char const* const *argv, bool isHttpPost, bool isAjax, Print *response)
    :HttpRequest(argc, argv, isHttpPost, isAjax, NULL, false, response)
{

}

// Clears the authentication status of the request.
void HttpRequest::clearAuthStatus() { authUser = NULL; isAuthenticatedInd = false; }

// Returns true if the request is Http Post, false otherwise.
bool HttpRequest::isHttpPost() const { return isHttpPostInd; }

// Returns true if the request was made by client script, false otherwise.
bool HttpRequest::isAjax() const { return isAjaxInd; }

// Returns the number of arguments.
uint8_t HttpRequest::getArgc() const { return argc; }

// Returns arguments as an array of pointers.
char const* const *HttpRequest::getArgv() const { return argv; }

// Gets authenticated user name.
const char *HttpRequest::getAuthenticatedUser() const { return authUser; }

// Returns true if request is authenticated.
bool HttpRequest::isAuthenticated() const { return isAuthenticatedInd; }

// Returns the response stream for the request.
Print *HttpRequest::getResponse() const { return response; }