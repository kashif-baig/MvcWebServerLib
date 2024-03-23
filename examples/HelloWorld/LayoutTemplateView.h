#ifndef LAYOUTTEMPLATEVIEW_H
#define LAYOUTTEMPLATEVIEW_H

/*
	Generated using Arduino.MVC.CodeGen view builder.
	For more information and help, visit https://www.cohesivecomputing.co.uk/arduino-web-development-pushing-the-limits/

	Looking to be an expert in Arduino web development? Make sure to purchase the book 'Arduino Web Development: Pushing the Limits' 
*/

#include "IView.h"
#include "ILayoutContentView.h"


class LayoutTemplateView: public IView {
public:
LayoutTemplateView (ILayoutContentView* model, HttpRequest *request = getDefaultRequest()) : IView(ViewIdNotSpecified, NULL, request) {
this->model = model;
this->viewMimeType = TextHtml;
}
ILayoutContentView* getModel () const { return model;}
protected:
ILayoutContentView* model;

void onRender () {
writeStr(PSTR("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\" /><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" /><title>"));
 model->renderTitle(); 
writeStr(PSTR(" - HelloWorld\n</title><link href=\"/Content/normal.css\" rel=\"stylesheet\"><link href=\"/Content/ham-menu.css\" rel=\"stylesheet\"><style type=\"text/css\">#container {margin-right: 1em; margin-left: 1em;}\n</style></head><body><nav id=\"hamnav\"><label for=\"hamburger\">&#9776;</label><input type=\"checkbox\" id=\"hamburger\" /><div id=\"hamitems\">"));
 // if (request->isAuthenticated()) { 
writeStr(PSTR("<a href=\"/\">Home</a>"));
 // } 
writeStr(PSTR("</div></nav><div id=\"container\"><h2>"));
 model->renderTitle(); 
writeStr(PSTR("</h2><div>"));
 model->renderBody(); 
writeStr(PSTR("</div></div>"));
 model->renderScript(); 
writeStr(PSTR("</body></html>"));
}

};

#endif
