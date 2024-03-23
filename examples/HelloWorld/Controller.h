#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "StdViews.h"
#include "LayoutTemplateView.h"

#include "DefaultView.h"

#include "IController.h"
#include "IWebServer.h"

#include "Content.h"

#include <new>

// Determine amount of memory to allocate for views and models.
static const size_t LARGEST_VIEW_SIZE =
    STATIC_MAX(LARGEST_STD_VIEW_SIZE, /* always need this one. */
    STATIC_MAX(sizeof(DefaultView),

    0));

static const size_t LARGEST_MODEL_SIZE =
    STATIC_MAX(sizeof(DefaultModel),

    0);

// Pre-allocated memory for views, models and layout template. Align to 4 byte boundary.

union
{
    char Memory[LARGEST_VIEW_SIZE];
    uint32_t dummy;
} ViewUnion;

union
{
    char Memory[LARGEST_MODEL_SIZE];
    uint32_t dummy;
} ModelUnion;

union
{
    char Memory[sizeof(LayoutTemplateView)];
    uint32_t dummy;
} LayoutUnion;

static char *View = ViewUnion.Memory;
static char *Model = ModelUnion.Memory;
static char *Layout = LayoutUnion.Memory;


// Implementation of an MVC controller.
class Controller : public IController
{
public:
    void setWebServer(IWebServer *webServer)
    {
        this->webServer = webServer;
    }


    // Performs requested action, then serves a view to the requesting client.
    // Returns NULL if there is no view to serve.
    IView *onProcessRequest(HttpRequest *request)
    {
        // Get URL parameter count.
        const uint8_t argc = request->getArgc();
        // Get array of URL parameters
        char const *const *argv = request->getArgv();
        
        bool isHttpPost = request->isHttpPost();


        if (argc == 0 || strcasecmp_P(argv[0], PSTR("Default")) == 0)
        {
            DefaultModel *model = new (Model) DefaultModel();
            DefaultView *defaultView =  new (View) DefaultView(model);
            return new (Layout) LayoutTemplateView(defaultView);
        }

        return NULL;
    }


    // Serves a static file that has been requested as /Content/<filename>
    IView *onServeContent(const char *fileName)
    {
        if (strcasecmp_P(fileName, PSTR("ham-menu.css")) == 0)
        {
            return new (View) PContentFileView(file_ham_menu_min_css_gz, sizeof(file_ham_menu_min_css_gz), TextCssStr, GzipEncodingStr);
        }
        // The 'if' statement below is optional. Can remove if more ROM space is required.
        else if (strcasecmp_P(fileName, PSTR("normal.css")) == 0)
        {
            return new (View) PContentFileView(file_normal_min_css_gz, sizeof(file_normal_min_css_gz), TextCssStr, GzipEncodingStr);
        }
        return NULL;
    }

private:
    IWebServer *webServer;
};

#endif
