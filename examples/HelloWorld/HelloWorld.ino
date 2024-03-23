/*

Please visit https://www.cohesivecomputing.co.uk/arduino-web-development-pushing-the-limits/ to download
necessary libraries and helpful utilities.

To get the best from the MVC Web Server, buy the ebook 'Arduino Web Development: Pushing the Limits'.
It is accompanied by numerous coding examples and coding exercises to help the reader build the knowledge
and confidence to become a skilled Arduino web developer.

*/

#if defined(ESP8266) || defined(ARDUINO_UNOWIFIR4) || defined(ESP32)
    #if defined(ESP8266)                // Check if ESP8266 board
        // SD card reader select pin
        #define SD_SELECT_PIN SS
        #include <ESP8266WiFi.h>
    #elif defined(ARDUINO_UNOWIFIR4)    // Check if UNO R4 WiFi
        // SD card reader select pin
        #define SD_SELECT_PIN 4
        #include "WiFiS3.h"
    #elif defined(ESP32)
        #define SD_SELECT_PIN SS
        #include "WiFi.h"
    #endif
    
    char ssid[] = "guest";          // your network SSID (name)
    char pass[] = "guest123";       // your network password

    WiFiServer server(80);
    WiFiClient client;

#else                               // Otherwise assume AVR with Ethernet shield (UNO or MEGA2560)
    // SD card reader select pin
    #define SD_SELECT_PIN 4
    #include <Ethernet.h>

    byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    EthernetServer server(80);
    IPAddress ip(192, 168, 1, 10);  // Use fixed IP for Ethernet
    EthernetClient client;

#endif

#include <MvcWebServerLib.h>
#include "Controller.h"


StringBuilder<64> requestBuf;       // May need to increase size for processing longer request headers,
                                    // or for binding large HTTP POST data. Increase if 413 status code.
StringBuilder<32> urlBuf;           // Increase the size if processing longer URLs

Controller controller;
MvcWebServer MvcWebServerInstance(&client, &controller, &requestBuf, &urlBuf);
MvcWebServer *webServer = &MvcWebServerInstance;


void setup()
{

#if defined(ESP8266) || defined(ESP32)
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
#elif defined(ARDUINO_UNOWIFIR4)
    WiFi.begin(ssid, pass);
#else
    Ethernet.begin(mac, ip);

    // If SD card connected to SPI, disable it. Is's not being used in this sketch.
    pinMode(SD_SELECT_PIN, OUTPUT);
    digitalWrite(SD_SELECT_PIN, HIGH);
#endif
    
    server.begin();
    controller.setWebServer(webServer);
}


void loop()
{
    // perform your code starting here
    
    #if defined(ESP8266) || defined(ARDUINO_UNOWIFIR4) || defined(ESP32)
        // If WiFi not connected, no point processing request.
        if (WiFi.status() != WL_CONNECTED) return;
    #endif 

    if (webServer->isProcessingRequest())
    {
        // Continue processing existing request.
        webServer->processRequest();
    }
    else
    {
        client = server.available();

        if (client)
        {
            // Start processing new request.
            webServer->processRequest();
        }
    }
    
}
