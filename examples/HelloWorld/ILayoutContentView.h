#ifndef ILAYOUTCONTENTVIEW_H
#define ILAYOUTCONTENTVIEW_H

#include "IView.h"

// Layout view interface for use by LayoutTemplateView
class ILayoutContentView : public IView
{
public:
    // Renders page title to response stream (required).
    virtual void renderTitle () = 0;
    // Renders body markup to response stream (required).
    virtual void renderBody() = 0;
    // Render optional script block
    virtual void renderScript() {}
protected:

   ILayoutContentView(uint8_t viewId, const char *viewFilePtr, HttpRequest *request = getDefaultRequest()) : IView(viewId, viewFilePtr, request)
   {

   }
   void onRender() {}
};

#endif
