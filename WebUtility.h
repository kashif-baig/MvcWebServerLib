#ifndef WEB_UTILITY_H
#define WEB_UTILITY_H

#include "WebDebugConfig.h"
#include <Arduino.h>
#include <ctype.h>

#ifndef STATIC_MAX
  #define STATIC_MAX(a, b)  ((a) > (b) ? (a) : (b))
#endif

PROGMEM const char FaviconStr[] = "favicon.ico";
PROGMEM const char OkStatusStr[] = "200 OK";
PROGMEM const char NotFoundStatusStr[] = "404 Not Found";
PROGMEM const char PayloadTooLargeStatusStr[] = "413 Payload Too Large";
PROGMEM const char TextHtmlString[] = "text/html";
PROGMEM const char ConnCloseStr[] = "Connection: close\r\n\r\n";

// Write to response stream a string stored in program memory.
extern void writeStr_P(Print *stream, const char *str_P);

// Write to response stream bytes stored in program memory.
extern void writeBytes_P(Print *stream, const char *bytes_P, uint16_t length);

// Returns true if a string ends with suffix stored in program memory.
extern bool endsWith_P(const char *str, const char *suffix_P);


// Performs in-place url decode.
extern char *urlDecode(char *str);

// In place trim of string. Shifts non-space part to beginning.
extern char *trim(char *str);

// Http headers for writing to a stream.
class HttpResponseHeader {
public:
  const char *status_P;
  const char *locationUrl;
  const char *contentType_P;
  const char *authTicket;
  const char *attachmentName;
  uint32_t contentLen;
  const char *contentEncoding_P;
  uint32_t cacheAge;

  // Clears the instance.
  void clear();
  // Writes headers including the 'Connection: Close' header.
  void writeHeader(Print *stream);
  // Writes headers and optionally includes the 'Connection: Close' header.
  void writeHeader(Print *stream, bool writeConnectionClose);
  // Writes not found header. 
  void set404NotFound();

};

typedef struct KeyValueStrPair {
  const char *key;
  const char *value;
} KeyValueStrPair;

#define keyValueStrPairArrayLen(m)  (sizeof(m) / sizeof(KeyValueStrPair))

#endif
