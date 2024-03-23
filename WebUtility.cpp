#include "WebUtility.h"

#define BLOCK_SIZE  20

PROGMEM const char ContentTypeStr[] = "Content-Type: ";
PROGMEM const char ContentEncodingStr[] = "Content-Encoding: ";
PROGMEM const char ContentLengthStr[] = "Content-Length: ";
PROGMEM const char ContentDispositionStr[] = "Content-Disposition: attachment; filename=";
PROGMEM const char LocationStr[] = "Location: ";
PROGMEM const char AuthTicketCookieStr[] = "Set-Cookie: auth_ticket=";
PROGMEM const char CacheControlAgeStr[] = "Cache-Control: public, max-age=";

PROGMEM const char HttpStr[] = "HTTP/1.1 ";

//---------------------------------------------------------------------------------------

void HttpResponseHeader::clear()
{
    status_P = NotFoundStatusStr;
    locationUrl = NULL;
    contentType_P = NULL;
    authTicket = NULL;
    attachmentName = NULL;
    contentLen = 0;
    contentEncoding_P = NULL;
    cacheAge = 0;
}

//---------------------------------------------------------------------------------------

void HttpResponseHeader::set404NotFound()
{
    clear();
    status_P = NotFoundStatusStr;
    contentType_P = TextHtmlString;
}

//---------------------------------------------------------------------------------------

void HttpResponseHeader::writeHeader(Print *stream)
{
    writeHeader(stream, true);
}

//---------------------------------------------------------------------------------------

void HttpResponseHeader::writeHeader(Print *stream, bool writeConnectionClose)
{
    writeStr_P(stream, HttpStr);
    writeStr_P(stream, status_P);
    stream->println();

    if (locationUrl != NULL)
    {
        writeStr_P(stream, LocationStr);
        stream->println(locationUrl);
    }

    if (contentType_P != NULL)
    {
        writeStr_P(stream, ContentTypeStr);
        writeStr_P(stream, contentType_P);
        stream->println();
    }

    if (authTicket != NULL)
    {
        writeStr_P(stream, AuthTicketCookieStr);
        stream->println(authTicket);
    }

    if (contentLen > 0)
    {
        if (contentEncoding_P != NULL)
        {
            writeStr_P(stream, ContentEncodingStr);
            writeStr_P(stream, contentEncoding_P);
            stream->println();
        }
        writeStr_P(stream, ContentLengthStr);
        stream->println(contentLen);
    }

    if (cacheAge)
    {
        writeStr_P(stream, CacheControlAgeStr);
        stream->println(cacheAge);
    }

    if (attachmentName)
    {
        writeStr_P(stream, ContentDispositionStr);
        stream->write('\"');
        stream->print(attachmentName);
        stream->println("\"");
    }
    if (writeConnectionClose)
    {
        writeStr_P(stream, ConnCloseStr);
    }
}

//---------------------------------------------------------------------------------------

char *urlDecode(char *data)
{
    // Taken and modified from stack exchange http://arduino.stackexchange.com/questions/18007/simple-url-decoding

    if (data == NULL)
    {
        return NULL;
    }

    // Create two pointers that point to the start of the data
    char *leader = data;
    char *follower = leader;

    // While we're not at the end of the string (current character not NULL)
    while (*leader)
    {
        // Check to see if the current character is a %
        if (*leader == '%')
        {

            // Grab the next two characters and move leader forwards
            leader++;
            char high = *leader;
            leader++;
            char low = *leader;

            // Convert ASCII 0-9A-F to a value 0-15
            if (high > 0x39)
                high -= 7;
            high &= 0x0f;

            // Same again for the low byte:
            if (low > 0x39)
                low -= 7;
            low &= 0x0f;

            // Combine the two into a single byte and store in follower:
            *follower = (high << 4) | low;
        }
        else if (*leader == '+')
        {
            *follower = ' ';
        }
        else
        {
            // All other characters copy verbatim
            *follower = *leader;
        }

        // Move both pointers to the next character:
        leader++;
        follower++;
    }
    // Terminate the new string with a NULL character to trim it off
    *follower = 0;
    return data;
}


//---------------------------------------------------------------------------------------

void writeStr_P(Print *stream, const char *str_P)
{
    if (!str_P)
        return;

    uint8_t i;
    char chr = pgm_read_byte(str_P++);

    do
    {
        uint8_t buf[BLOCK_SIZE];

        for (i = 0; i < sizeof(buf) && chr != (char)0; i++)
        {
            buf[i] = chr;
            chr = pgm_read_byte(str_P++);
        }

        if (i > 0)
        {
            stream->write(buf, i);
        }
    } while (i > 0);
}

//---------------------------------------------------------------------------------------

void writeBytes_P(Print *stream, const char *bytes_P, uint16_t length)
{
    if (!bytes_P)
        return;

    uint8_t i;
    uint16_t byteIdx = 0;

    do
    {
        uint8_t buf[20];

        for (i = 0; i < sizeof(buf) && byteIdx < length; i++, byteIdx++)
        {
            char chr = pgm_read_byte(bytes_P++);
            buf[i] = chr;
        }

        if (i > 0)
        {
            stream->write(buf, i);
        }
    } while (i > 0);
}

//---------------------------------------------------------------------------------------

bool endsWith_P(const char *str, const char *suffix_P)
{
    if (!str || !suffix_P)
        return 0;
    uint16_t lenstr = strlen(str);
    uint16_t lensuffix = strlen_P(suffix_P);
    if (lensuffix > lenstr)
        return 0;

    return strncmp_P(str + lenstr - lensuffix, suffix_P, lensuffix) == 0;
}

//---------------------------------------------------------------------------------------

char *trim(char *str)
{
    char *p = str;
    int l = strlen(p);

    if (l == 0)
        return str;

    while (isspace(p[l - 1]))
        p[--l] = 0;
    while (*p && isspace(*p))
        ++p, --l;

    memmove(str, p, l + 1);
    return str;
}

//---------------------------------------------------------------------------------------

// PROGMEM const char EncChars[] = {'<', '>', '&', '%', '/r', '/n'};

// void writeUrlEncoded(Stream *stream, const char *str
// {
//     char encoded[4];
//     const char *start = str;
//     const char *end = start;

//     for (; *end; end++)
//     {
//         if (!memchr_P(EncChars, *end, sizeof(EncChars)))
//         {
//             continue;
//         }
//         const char matched = *end;

//         stream->write(start, end - start);

//         char loNybble = matched & 0x0F;
//         char hiNybble = (matched & 0xF0) >> 4;

//         char *encodedStr = encoded;

//         // Convert matched char to hex string prefixed with '%'.
//         *(encodedStr++) = '%';
//         *(encodedStr++) = hiNybble + (hiNybble <= 9 ? 48 : 55);
//         *(encodedStr++) = loNybble + (loNybble <= 9 ? 48 : 55);
//         *encodedStr = 0;

//         stream->write(encoded);

//         start = end + 1;
//     }
//     stream->write(start);
// }