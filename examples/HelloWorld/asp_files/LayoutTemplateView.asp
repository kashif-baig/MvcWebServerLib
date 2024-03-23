@rem Not using Arduino.MVC.CodeGen? Select all text then copy/paste in to panel at https://mvc-view-builder.cohesivecomputing.co.uk/.
@rem Build view code, then copy/paste resulting code back in to LayoutTemplateView.h
@rem Do not leave any blank lines at top or between directives.
@viewname LayoutTemplate
@minify true
@mimetype TextHtml
@modeltype ILayoutContentView*
@include "ILayoutContentView.h"

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>
        <% model->renderTitle(); %> - HelloWorld
    </title>
	<link href="/Content/normal.css" rel="stylesheet">
    <link href="/Content/ham-menu.css" rel="stylesheet">
    <style type="text/css">
        #container {margin-right: 1em; margin-left: 1em;}
    </style>
</head>

<body>
    <nav id="hamnav">
        <label for="hamburger">&#9776;</label>
        <input type="checkbox" id="hamburger" />

        <div id="hamitems">
            <% // if (request->isAuthenticated()) { %>
                <a href="/">Home</a>

            <% // } %>
        </div>
    </nav>
    <div id="container">
        <h2><% model->renderTitle(); %></h2>

        <div>
            <% model->renderBody(); %>
        </div>
    </div>
<% model->renderScript(); %>
</body>
</html>
