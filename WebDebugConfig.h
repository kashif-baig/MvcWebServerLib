#ifndef WEB_DEBUG_CONFIG_H
#define WEB_DEBUG_CONFIG_H

#ifndef WEB_DEBUG_ENABLED
  #define WEB_DEBUG_ENABLED 0
#endif


// #if not WEB_DEBUG_ENABLED
//   #ifndef debugInit
//     #define debugInit(m) ;
//     #define debugln(m) ;
//     #define debuglnbin(m) ;
//     #define debughex(m) ;
//     #define debug(m)  ;
//     #define debugbuf(m,i) ;
//   #endif
// #else
//   #ifndef debugInit
//     #define debugInit(m)    Serial.begin(m);
//     #define debugln(m)      Serial.println(m);
//     #define debuglnbin(m)   Serial.println(m, BIN);
//     #define debughex(m)     Serial.print(m, HEX);
//     #define debug(m)        Serial.print(m);
//     #define debugbuf(m,i)   Serial.write(m,(int)i);
//   #endif
// #endif


#endif