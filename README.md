## A Model-View-Controller Web Server and Framework

Get a head start developing Arduino IOT and web applications using the MVC Web Framework and build utilities. The Arduino MVC Web Framework offers numerous advantages for building sophisticated web server applications on the Arduino platform:

**1\. Model-View-Controller (MVC) Design:**

The framework follows the MVC pattern, which separates concerns into three components:

- Model: Represents the data and business logic.
- View: Handles the presentation layer (HTML, XML, or JSON).
- Controller: Manages user input and orchestrates interactions between the model and view.

This separation enhances code organization, testability and maintainability.

**2\. RESTful URLs and Parameter Processing:**

- The framework supports RESTful URLs, simplifying parameter handling in web requests.
- You can define clean and meaningful URLs for different actions.

**3\. Model Binding for HTTP POST Requests:**

- Easily bind data from HTTP POST requests to model objects.
- Streamlines form submissions, input validation and processing.

**4\. UI Consistency with Layout (Master) Pages:**

- Create consistent layouts for your web pages using master pages.
- Centralize common elements like headers, footers, and navigation menus.
- Expose view helper methods to simplify UI rendering logic.

**5\. AJAX Requests and Partial Rendering:**

- Straight-forward implementation of asynchronous updates using AJAX.
- Load specific parts of a page without refreshing the entire content.
- Improve user experience with responsive web pages.

**6\. JSON or XML Web APIs:**

- Create IOT and web APIs that return data in JSON or XML format.
- Ideal for home automation and integrating with other services or devices.

**7\. Cookie-Based Authentication:**

- Secure your web pages and APIs using cookie-based authentication.
- Control access to specific resources.

**8\. File Upload and Attachments Download Support:**

- Handle file uploads from clients and allow users to download attachments.
- Useful for scenarios like uploading content in different formats.

**9\. Static Content from SD Card or ROM:**

- Serve static files (CSS, JavaScript, images) directly from an SD card or read-only memory (ROM).
- Reduces memory usage on the Arduino.

**10\. Browser Caching:**

- Optimize performance by caching CSS, JavaScript, and image files in the user’s browser.
- Reduces redundant requests and speeds up page loading.

The Arduino MVC Web Framework is compatible with various Arduino boards, including Uno R3/R4, Mega 2560, ESP32, and ESP8266. Use with either the Ethernet Shield, or native WiFi capabilities. It empowers developers to create dynamic web and IOT applications while leveraging the productivity of server-side scripting and the efficiency of compiled C++ code. You can explore more by referring to the book “Arduino Web Development: Pushing the Limits” by Kashif Baig.

<https://www.cohesivecomputing.co.uk/arduino-web-development-pushing-the-limits/>

## Dependencies

The Arduino libraries below will need to be installed for the MVC Web Framework projects to build.

- [StringLib](https://github.com/kashif-baig/StringLib) – efficient string building and reading
- [MessagingLib](https://github.com/kashif-baig/MessagingLib) – serial communication used by some of the example web projects
- Ethernet (use Arduino IDE to install)

## Using Build Tools and Visual Studio Code

Watch a short demonstration of the benefits of using Arduino web project and view builder [utilities](https://files.cohesivecomputing.co.uk/mvc/Arduino.MVC.zip). Click the image below.

[![Arduino Web Development - getting a head start](https://img.youtube.com/vi/F2KRuxAnosU/0.jpg)](https://www.youtube.com/watch?v=F2KRuxAnosU)

Arduino Server Pages, or ASPs, are HTML markup files that have C++ scripting tags. When integrated with Visual Studio Code, the view builder utility converts the ASP files to native C++ before building and deploying the project to an Arduino.

Follow the readme instructions that accompany the utilities to learn how to install and use them. Additional information can be found at the [project website](https://www.cohesivecomputing.co.uk/arduino-web-development-pushing-the-limits/#vscode).

## Example Arduino Web Projects

Watch the short video clip below demonstrating [web projects](https://www.cohesivecomputing.co.uk/arduino-web-projects/#beginadvance) of varying complexity built using the Arduino MVC Web Framework library:

[![Arduino Web Projects - highlights](https://img.youtube.com/vi/sBDyV_gpgEA/0.jpg)](https://www.youtube.com/watch?v=)
