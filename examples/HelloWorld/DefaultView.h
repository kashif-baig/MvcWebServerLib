#ifndef DEFAULTVIEW_H
#define DEFAULTVIEW_H

/*
	Generated using Arduino.MVC.CodeGen view builder.
	For more information and help, visit https://www.cohesivecomputing.co.uk/arduino-web-development-pushing-the-limits/

	Looking to be an expert in Arduino web development? Make sure to purchase the book 'Arduino Web Development: Pushing the Limits' 
*/

#include "ILayoutContentView.h"
#include "DefaultModel.h"


class DefaultView: public ILayoutContentView {
public:
DefaultView (DefaultModel* model, HttpRequest *request = getDefaultRequest()) : ILayoutContentView(ViewIdNotSpecified, NULL, request) {
this->model = model;
this->viewMimeType = TextHtml;
}
DefaultModel* getModel () const { return model;}
protected:
DefaultModel* model;

void renderTitle () {
writeStr(PSTR("Home page"));
}

void renderBody () {
writeStr(PSTR("<p>Hello World!</p>"));
}

};

#endif
